#include "header.hpp"


void kategoria::print(WINDOW * window)
{
    mvwprintw(window, 5, 2, "Symbol:\t%s\n", this->symbol.c_str());
    mvwprintw(window, 7, 2, "%s\t%s\n", "Nazwa:", this->nazwa.c_str());
    mvwprintw(window, 9, 2, "%s\n", "Nalezace tytuly:", this->nazwa.c_str());

    for (int i=0; i<nalezace.size(); i++)
        mvwprintw(window, 11+i*2, 2, "%d:\t%s\n", i+1, nalezace[i]->tytul.c_str());
}

void ksiazka::print(WINDOW * window)
{
    mvwprintw(window, 5, 2, "Autor:\t%s\n", autor.c_str());
    mvwprintw(window, 7, 2, "%s\t%s\n", "Tytul:", tytul.c_str());
    mvwprintw(window, 9, 2, "Rok wydania:\t%s\n", rok_wydania.c_str());
    mvwprintw(window, 11, 2, "%s\t%03d\n", "Numer katalogowy:", id);
    mvwprintw(window, 13, 2, "Kategoria:\t%s\n", kat->nazwa.c_str());
    if (!dostepnosc)
    {
        mvwprintw(window, 15, 2, "%s %s %s(%d)\n", "Wypozycza:", this->wypozyczajacy->imie.c_str(), this->wypozyczajacy->nazwisko.c_str(), this->wypozyczajacy->id);
        struct tm * timeinfo;
        timeinfo = localtime (&pozyczona);
        mvwprintw(window, 17, 2, "Wypozyczona: %s", asctime(timeinfo));
    }
}

void klient::print(WINDOW * window)
{
    mvwprintw(window, 5, 2, "Imie:\t%s\n", imie.c_str());
    mvwprintw(window, 7, 2, "%s\t%s\n", "Nazwisko:", nazwisko.c_str());
    mvwprintw(window, 9, 2, "Numer karty:\t%d\n", id);
    mvwprintw(window, 11, 2, "Adres:\t%s\n", adres.c_str());
    mvwprintw(window, 13, 2, "%s\t%s\n", "Telefon:", telefon.c_str());

    mvwprintw(window, 15, 2, "%s", "Wypozyczone tytuly:");

    for (int i=0; i<pozyczone.size(); i++)
        mvwprintw(window, 17+i*2, 2, "%d:\t%s\n", i+1, pozyczone[i]->tytul.c_str());
}

void kategoria::modify(vector<char*> data)
{
    symbol = data[0];
    nazwa = data[1];
}

void klient::modify(vector<char*> data)
{
    imie = data[0];
    nazwisko = data[1];
    adres = data[2];
    telefon = data [3];
}

void ksiazka::modify(vector<char*> data)
{
    autor = data[0];
    tytul = data[1];
    rok_wydania = data[2];
}

bool kategoria::cat_find(string test)
{
    if (symbol==test)
        return true;
    else
        return false;
}
char ** cat_choices(vector <kategoria*> &kategorie)
{
    string tmp;
    char n[4];
    char ** elo = new char*[kategorie.size()];

    for (int i=0; i<kategorie.size(); i++)
    {
        tmp = "   ";
        sprintf(n, "%d", i+1);
        tmp+=n;
        tmp+="   ";
        tmp+=kategorie[i]->symbol;
        tmp+="      ";
        tmp+=kategorie[i]->nazwa;
        elo[i] = new char[tmp.length()+1];
        strcpy((elo[i]), tmp.c_str());
    }

    return elo;
}

char ** book_choices(vector <ksiazka*>& ksiazki)
{
    string tmp;
    char n[4];
    char ** elo = new char*[ksiazki.size()];

    for (int i=0; i<ksiazki.size(); i++)
    {
        tmp = "   ";
        sprintf(n, "%d", ksiazki[i]->id);
        tmp+=n;
        tmp += "  ";
        tmp += ksiazki[i]->autor;
        string autor = ksiazki[i]->autor;
        for (int j=0; j<15-ksiazki[i]->autor.length(); j++)
            tmp+=" ";
        tmp += ksiazki[i]->tytul;
        *(elo+i) = (char *)malloc(tmp.length()+1);
        strcpy(*(elo+i), tmp.c_str());
    }

    return elo;
}

char ** client_choices(vector <klient*>& klienci)
{
    string tmp;
    char n[4];
    char ** elo = new char*[klienci.size()];

    for (int i=0; i<klienci.size(); i++)
    {
        tmp = "   ";
        sprintf(n, "%d  ", i+1);
        tmp+=n;
        sprintf(n, "%d", klienci[i]->id);
        tmp+=n;
        for (int j=0; j<4-strlen(n); j++)
            tmp+=" ";
        tmp += klienci[i]->imie;
        for (int j=0; j<13-klienci[i]->imie.length(); j++)
            tmp+=" ";
        tmp += klienci[i]->nazwisko;
        *(elo+i) = (char *)malloc(tmp.length()+1);
        strcpy(*(elo+i), tmp.c_str());
    }

    return elo;
}

int data_import(vector <kategoria*> &kategorie, vector <klient*> &klienci, vector <ksiazka*> &ksiazki)
{
    ifstream in1("kategorie");
    ifstream in2("ksiazki");
    ifstream in3("klienci");
    int c;

    char ** tablica;

    if(!in1.is_open()||!in2.is_open()||!in3.is_open())
        return -1;

    while (!in1.eof())
    {
        kategoria *new_cat = new kategoria;
        getline(in1, new_cat->nazwa);
        getline(in1, new_cat->symbol);
        in1.ignore(numeric_limits<streamsize>::max(), '\n');
        ;
        kategorie.push_back(new_cat);
    }

    in1.close();

    while (!in2.eof())
    {
        ksiazka *new_book = new ksiazka;
        string kat, id;
        bool s;
        getline(in2, new_book->tytul);
        getline(in2, new_book->autor);
        getline(in2, id);
        getline(in2, new_book->rok_wydania);
        getline(in2, kat);
        new_book->id = atoi(id.c_str());
        new_book->dostepnosc = true;
        int j=0;
        for (vector<kategoria*>::iterator n=kategorie.begin(); n!=kategorie.end(); ++n, ++j)
            if(kategorie[j]->cat_find(kat))
            {
                new_book->kat = kategorie[j];
                kategorie[j]->nalezace.push_back(new_book);
                break;
            }
        new_book->pozyczona = 0;
        new_book->wypozyczajacy = NULL;
        in2.ignore(numeric_limits<streamsize>::max(), '\n');
        if(new_book->tytul!=""&&new_book->autor!=""&&new_book->id!=0)
            ksiazki.push_back(new_book);
        else delete new_book;
    }

    in2.close();


    while (!in3.eof())
    {
        klient *new_client  = new klient;
        getline(in3, new_client->imie);
        getline(in3, new_client->nazwisko);
        string id;
        getline(in3, id);
        new_client->id = atoi(id.c_str());
        getline(in3, new_client->telefon);
        getline(in3, new_client->adres);
        getline(in3, id);
        if(new_client->imie!="")
        for(int i=0; i<atoi(id.c_str()); i++){
            string tmp;
            getline(in3, tmp);
            int n = atoi(tmp.c_str());
            new_client->pozyczone.push_back(ksiazki[n]);
            ksiazki[n]->dostepnosc = false;
            ksiazki[n]->wypozyczajacy = new_client;
            string czas;
            getline(in3, czas);
            ksiazki[n]->pozyczona = atoi(czas.c_str());
        }
        in3.ignore(numeric_limits<streamsize>::max(), '\n');
        if(new_client->imie!=""&&new_client->nazwisko!=""&&new_client->id!=0)
            klienci.push_back(new_client);
        else
         delete new_client;
    }
    in3.close();

    return 1;
}

char * trim(char * text)
{
    int n = strlen(text);
    for(int i=0; i<n-1; i++)
        if(text[i]==' '&&(text[i+1]==' '||text[i+1]=='\0'))
            text[i] = '\0';

    return (text);
}

void sort_nazwa(vector<kategoria *>& kategorie)
{
    for(int i=0; i<kategorie.size(); i++)
    {
        for(int j=0; j<kategorie.size()-1; j++)
        {
            if(kategorie[j]->nazwa > kategorie[j+1]->nazwa)
                swap(kategorie[j], kategorie[j+1]);
        }
    }
}

void sort_symbol(vector<kategoria *>& kategorie)
{
    for(int i=0; i<kategorie.size(); i++)
    {
        for(int j=0; j<kategorie.size()-1; j++)
        {
            if(kategorie[j]->symbol > kategorie[j+1]->symbol)
                swap(kategorie[j], kategorie[j+1]);
        }
    }
}

void sort_cid(vector<klient*>& klienci)
{
    for(int i=0; i<klienci.size(); i++)
    {
        for(int j=0; j<klienci.size()-1; j++)
        {
            if(klienci[j]->id > klienci[j+1]->id)
                swap(klienci[j], klienci[j+1]);
        }
    }
}

void sort_imie(vector<klient*>& klienci)
{
    for(int i=0; i<klienci.size(); i++)
    {
        for(int j=0; j<klienci.size()-1; j++)
        {
            if(klienci[j]->imie > klienci[j+1]->imie)
                swap(klienci[j], klienci[j+1]);
        }
    }
}

void sort_nazwisko(vector<klient*>& klienci)
{
    for(int i=0; i<klienci.size(); i++)
    {
        for(int j=0; j<klienci.size()-1; j++)
        {
            if(klienci[j]->nazwisko > klienci[j+1]->nazwisko)
                swap(klienci[j], klienci[j+1]);
        }
    }
}

void sort_bid(vector<ksiazka*>& ksiazki)
{
    for(int i=0; i<ksiazki.size(); i++)
    {
        for(int j=0; j<ksiazki.size()-1; j++)
        {
            if(ksiazki[j]->id > ksiazki[j+1]->id)
                swap(ksiazki[j], ksiazki[j+1]);
        }
    }
}

void sort_autor(vector<ksiazka*>& ksiazki)
{
    for(int i=0; i<ksiazki.size(); i++)
    {
        for(int j=0; j<ksiazki.size()-1; j++)
        {
            if(ksiazki[j]->autor > ksiazki[j+1]->autor)
                swap(ksiazki[j], ksiazki[j+1]);
        }
    }
}

void sort_tytul(vector<ksiazka*>& ksiazki)
{
    for(int i=0; i<ksiazki.size(); i++)
    {
        for(int j=0; j<ksiazki.size()-1; j++)
        {
            if(ksiazki[j]->tytul > ksiazki[j+1]->tytul)
                swap(ksiazki[j], ksiazki[j+1]);
        }
    }
}

size_t kategoria::wyszukaj(string text)
{
    size_t pos = symbol.find(text);
    if(pos == string::npos)
        pos = nazwa.find(text);

    return pos;
}

size_t klient::wyszukaj(string text)
{
    size_t pos = imie.find(text);
    if(pos == string::npos)
    {
        pos = nazwisko.find(text);
        if(pos == string::npos)
        {
            string numer;
            char tmp[4];
            sprintf(tmp, "%d", id);
            numer = tmp;
            pos = numer.find(text);
        }
    }


    return pos;
}

size_t ksiazka::wyszukaj(string text)
{
    size_t pos = autor.find(text);
    if(pos == string::npos)
    {
        pos = tytul.find(text);
        if(pos == string::npos)
        {
            string numer;
            char tmp[4];
            sprintf(tmp, "%d", id);
            numer = tmp;
            pos = numer.find(text);
        }
    }

    return pos;
}

bool ksiazka::check()
{
    return dostepnosc;
}

bool kategoria::check(){
    return true;
}

bool klient::check(){
    if (pozyczone.size()==0)
        return true;
    else
        return  false;
}

int data_export(vector<kategoria*>& kategorie, vector<klient*>& klienci, vector<ksiazka*>& ksiazki){

    ofstream out1("kategorie");
    ofstream out2("ksiazki");
    ofstream out3("klienci");

    int j=0;

    if(!out1.is_open()||!out2.is_open()||!out3.is_open())
        return -1;

    for (vector<kategoria*>::iterator i=kategorie.begin(); i!=kategorie.end(); ++i, ++j){
        out1 << kategorie[j]->nazwa << endl;
        out1 << kategorie[j]->symbol << endl;
        out1 << endl;
    }

    /*while (!out2.eof())
    {
        ksiazka *new_book = new ksiazka;
        string kat, id;
        bool s;
        getline(out2, new_book->tytul);
        getline(out2, new_book->autor);
        getline(out2, id);
        getline(out2, new_book->rok_wydania);
        getline(out2, kat);
        new_book->id = atoi(id.c_str());
        new_book->dostepnosc = true;
        int j=0;
        for (vector<kategoria*>::iterator n=kategorie.begin(); n!=kategorie.end(); ++n, ++j)
            if(kategorie[j]->cat_find(kat))
            {
                new_book->kat = kategorie[j];
                kategorie[j]->nalezace.push_back(new_book);
                break;
            }
        new_book->pozyczona = 0;
        new_book->wypozyczajacy = NULL;
        out2.ignore(numeric_limits<streamsize>::max(), '\n');
        ksiazki.push_back(new_book);
    }
    out2.close();*/
j=0;
for (vector<ksiazka*>::iterator i=ksiazki.begin(); i!=ksiazki.end(); ++i, ++j){
        out2 << ksiazki[j]->tytul << endl;
        out2 << ksiazki[j]->autor << endl;
        out2 << ksiazki[j]->id << endl;
        out2 << ksiazki[j]->rok_wydania << endl;
        out2 << ksiazki[j]->kat->symbol << endl;

        out2 << endl;
    }
    out2.close();


   /* while (!out3.eof())
    {
        klient *new_client  = new klient;
        getline(out3, new_client->imie);
        getline(out3, new_client->nazwisko);
        string id;
        getline(out3, id);
        new_client->id = atoi(id.c_str());
        getline(out3, new_client->telefon);
        getline(out3, new_client->adres);
        getline(out3, id);
        for(int i=0; i<atoi(id.c_str()); i++){
            string tmp;
            getline(out3, tmp);
            int n = atoi(tmp.c_str());
            new_client->pozyczone.push_back(ksiazki[n]);
            ksiazki[n]->dostepnosc = false;
            ksiazki[n]->wypozyczajacy = new_client;
            string czas;
            getline(out3, czas);
            ksiazki[n]->pozyczona = atoi(czas.c_str());
        }
        out3.ignore(numeric_limits<streamsize>::max(), '\n');
        klienci.push_back(new_client);
    }
    out3.close();*/
    j=0;
for (vector<klient*>::iterator i=klienci.begin(); i!=klienci.end(); ++i, ++j){
        out3 << klienci[j]->imie << endl;
        out3 << klienci[j]->nazwisko<< endl;
        out3 << klienci[j]->id << endl;
        out3 << klienci[j]->pozyczone.size() << endl;
        int m=0;
        for (vector<ksiazka*>::iterator n=klienci[j]->pozyczone.begin(); n!=klienci[j]->pozyczone.end(); ++n, ++m){
            out3 << klienci[j]->pozyczone[m]->id << endl;
            out3 << (int) klienci[j]->pozyczone[m]->pozyczona << endl;
        }

        out3 << endl;
    }
    out3.close();


    return 1;
}
