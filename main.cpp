#include "header.hpp"

int main()
{

    vector <kategoria*> kategorie;
    vector <ksiazka*> ksiazki;
    vector <klient*> klienci;
    int x=0;
    if(init()&&(import(kategorie, klienci, ksiazki)))
        /*vector <kategoria> *kategorie = import_cat(kategorie);
        vector <ksiazka> *ksiazki = import_book(ksiazki);
        vector <klient> *klienci  = import_client(klienci);*/
        do
            x = menu_main(kategorie, klienci, ksiazki);
        while(x!=4&&x!=-1);

    endall();

    return 0;
}
