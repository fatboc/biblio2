#include <iostream>
#include <ncurses.h>
#include <menu.h>
#include <form.h>
#include <vector>
#include <fstream>
#include <limits>
#include <cstring>
#include <string>
#include <cstdlib>
#include <typeinfo>
#include <algorithm>

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
    void print(WINDOW *);
};

struct Klient{
    string imie;
    string nazwisko;
    string adres;
    string telefon;
    int id;
    vector <ksiazka*> pozyczone;
    void print(WINDOW*);
};

struct Kategoria{
    string symbol;
    string nazwa;
    vector <ksiazka*> nalezace;
    void print(WINDOW*);
    bool cat_find(string);
};


//user.cpp
int init();
int endall();
int menu_create(char*, int, char*);
int menu_main(vector <kategoria*>&, vector <klient*>&, vector <ksiazka*>&);
int zapisz();
int list_view(char*, char *, int, char *);
int dialog(char *, int, char *, char *);
int sort_menu(int);
int menu_kategorie(WINDOW * window, vector <kategoria*>&);
void clear_guide();
int find_longest(char **, int);
int menu_klienci(WINDOW * window, vector <klient*>&);
int menu_ksiazki(WINDOW * window, vector <ksiazka*>&);
template <typename T>
int item_details(WINDOW * window, T *item, char *, int mode);
int zapisz();
int usun();
int item_form(int, char*, char**);
int sdialog();
int item_search();
int add_item();

//data.cpp
vector<kategoria> * import_cat(vector <kategoria>*);
vector<klient> * import_client(vector <klient>*);
vector<ksiazka> * import_book(vector <ksiazka>*);
int import(vector <kategoria*> &, vector <klient*> &, vector <ksiazka*> &);
char ** cat_choices(vector <kategoria*>&);
char ** book_choices(vector <ksiazka*>);
char ** client_choices(vector <klient*>);
void add_book();
