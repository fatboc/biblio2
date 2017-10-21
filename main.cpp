#include "header.hpp"

int main()
{
    deque <kategoria> kategorie;
    deque <klient> klienci;
    deque <ksiazka> ksiazki;

    int x=0;
    if(init()&&import(kategorie, klienci, ksiazki))
        do
            x = menu_main(kategorie, klienci, ksiazki);
        while(x!=4&&x!=-1);

    endall();

    return 0;
}
