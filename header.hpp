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
#include <time.h>

#define KSIAZKA 1
#define KLIENT 2
#define KATEGORIA 3

using namespace std;

typedef struct Ksiazka ksiazka;
typedef struct Klient klient;
typedef struct Kategoria kategoria;

struct Ksiazka
{
    string tytul;
    string autor;
    string rok_wydania;
    int id;
    bool dostepnosc;
    klient * wypozyczajacy;
    kategoria * kat;
    time_t pozyczona;
    void print(WINDOW *);
    void modify(vector<char*> data);
    size_t wyszukaj (string text);
    bool check();
};

struct Klient
{
    string imie;
    string nazwisko;
    string adres;
    string telefon;
    int id;
    vector <ksiazka*> pozyczone;
    void print(WINDOW*);
    void modify(vector<char*> data);
    size_t wyszukaj(string text);
    bool check();
};

struct Kategoria
{
    string symbol;
    string nazwa;
    vector <ksiazka*> nalezace;
    void print(WINDOW*);
    bool cat_find(string);
    void modify(vector<char*> data);
    size_t wyszukaj(string text);
    bool check();

};




//user.cpp
int init();
int endall();
int menu_create(char*, int, char*);
int menu_main(vector <kategoria*>&, vector <klient*>&, vector <ksiazka*>&);
int zapisz(vector<kategoria*>&, vector<ksiazka*>&, vector<klient*>&);
int list_view(char*, char *, int, char *);
int dialog(char *, int, char *, char *);
int sort_menu(int);
int menu_kategorie(WINDOW * window, vector <kategoria*>&);
void clear_guide();
int find_longest(char **, int);
int menu_klienci(WINDOW * window, vector <klient*>&);
int menu_ksiazki(WINDOW * window, vector <ksiazka*>&, vector <kategoria*>&, vector <klient*>);
template <typename T>
int item_details(WINDOW * window, T *item, char *, int mode, char **);
int zapisz();
template <typename T>
int usun(vector<T*>&, int);
template <typename T>
int item_form(T *item, int, char*, char**, const char **);
string sdialog();
int cat_search(vector <kategoria*>&);
int book_search(vector<ksiazka*>&, vector <ksiazka*>&, int);
int client_search(vector <klient*>&, vector <klient*>&, int);
int add_book(vector <ksiazka*>&, vector <kategoria*>&);
int add_cat(vector <kategoria *>&);
int add_client(vector <klient*> &);

//data.cpp

int data_import(vector <kategoria*> &, vector <klient*> &, vector <ksiazka*> &);
char ** cat_choices(vector <kategoria*>&);
char ** book_choices(vector <ksiazka*>&);
char ** client_choices(vector <klient*>&);
char * trim(char *);
void sort_nazwa(vector<kategoria *>&);
void sort_symbol(vector<kategoria *>&);
void sort_cid(vector<klient*>& klienci);
void sort_imie(vector<klient*>& klienci);
void sort_nazwisko(vector<klient*>& klienci);
void sort_bid(vector<ksiazka*>& ksiazki);
void sort_autor(vector<ksiazka*>& ksiazki);
void sort_tytul(vector<ksiazka*>& ksiazki);
int data_export(vector<kategoria*>&, vector<klient*>&, vector<ksiazka*>&);
