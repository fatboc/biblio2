#include "header.hpp"

int import(deque <kategoria> &kategorie, deque <klient> klienci, deque <ksiazka> ksiazki){
    ifstream in1("kategorie");
    ifstream in2("ksiazki");
    ifstream in3("klienci");
    int c;
    ksiazka new_book;
    klient new_client;

    char ** tablica;

    if(!in1.is_open()||!in2.is_open()||!in3.is_open())
        return -1;

    while (!in1.eof()){
        //string nazwa, symbol;
       /* while(!isalpha(c = in1.peek())){
            if (c==EOF) break;
            else in1.get();
        } ;*/
        kategoria *new_cat = new kategoria;
        getline(in1, new_cat->nazwa);
        getline(in1, new_cat->symbol);
        in1.ignore(numeric_limits<streamsize>::max(), '\n');
        ;
        kategorie.push_back(*new_cat);
    }
    tablica = cat_choices(kategorie);
    for(int i=0; i<kategorie.size(); i++) printf("%s\n", tablica[i]);
    in1.close();

    while (!in2.eof()){
        in2 >> new_book.id >> new_book.rok_wydania;
        getline(in2, new_book.tytul);
        getline(in2, new_book.autor);
        in2.ignore(numeric_limits<streamsize>::max(), '\n');
        ksiazki.push_back(new_book);
    }
    in2.close();

    while (!in3.eof()){
        in3 >> new_client.imie >> new_client.nazwisko >> new_client.id >> new_client.telefon;
        getline(in3, new_client.adres);
        in3.ignore(numeric_limits<streamsize>::max(), '\n');
        klienci.push_back(new_client);
    }
    in3.close();

    return 1;
}

char ** cat_choices(deque <kategoria> kategorie){
    string tmp;
    char n[4], **elo;

    for (int i=0; i<kategorie.size(); i++){
        tmp = "   ";
        sprintf(n, "%d", i+1);
        tmp+=n;
        tmp+="   ";
        tmp+=kategorie[i].symbol;
        tmp+="      ";
        tmp+=kategorie[i].nazwa;
        *(elo+i) = (char *)malloc(tmp.length()+1);
        strcpy(*(elo+i), tmp.c_str());
    }

    return elo;
}
