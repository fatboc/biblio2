#include <iostream>
#include <ncurses.h>
#include <menu.h>
#include <form.h>
#include <vector>
#include <deque>
#include <fstream>
#include <limits>
#include <cstring>
#include <string>
#include <cstdlib>

#define KSIAZKA 1
#define KLIENT 2
#define KATEGORIA 3

using namespace std;

typedef struct Ksiazka ksiazka;
typedef struct Klient klient;
typedef struct Kategoria kategoria;

struct Ksiazka{
    string tytul;
    string autor;
    int rok_wydania;
    int id;
    bool dostepnosc;
    klient * wypozyczajacy;
    kategoria * kat;
    time_t pozyczona;
};

struct Klient{
    string imie;
    string nazwisko;
    string adres;
    string telefon;
    int id;
    vector <ksiazka> pozyczone;
};

struct Kategoria{
    string symbol;
    string nazwa;
    vector <ksiazka> nalezace;
};


//user.cpp
int init();
int endall();
int menu_create(char*, int, char*);
int menu_main(deque <kategoria>&, deque <klient>&, deque <ksiazka>&);
int zapisz();
int list_view(char*, char *, int, char *);
int dialog(char *, int, char *, char *);
int sort_menu(int);
int menu_kategorie(deque <kategoria>&);
void clear_guide();
int find_longest(char **, int);
int menu_klienci(deque <klient>&);
int menu_ksiazki(deque <ksiazka>&);
template <typename T>
int item_details(T item, char *, int mode);
int zapisz();
int usun();
int item_form(int, char*, char**);
int sdialog();
int item_search();
int add_item();

//data.cpp
int import(deque <kategoria>&, deque <klient>, deque <ksiazka>);
char ** cat_choices(deque <kategoria>);
