#include "header.hpp"

int init(){
//inicjalizacja curses

    if(!initscr())
        return -1;
    start_color();
    raw();
    noecho();
    curs_set(0);
    keypad(stdscr, TRUE);

    return 1;
}

int endall(){
//czyszczenie zasobow, zamykanie programu
    endwin();
    return 0;
}

int find_longest(char ** tab, int n){
    int len=0;

    for (int i=0; i<n; i++)
        if (strlen(tab[i])>len)
            len = strlen(tab[i]);

    return len;
}

int menu_create(WINDOW * menu_window, char * choices[], int n, char * header){
//tworzenie dowolnego menu programu
    ITEM **items;
	MENU *this_menu;

	items = (ITEM **)calloc(n + 1, sizeof(ITEM *));
	for(int i = 0; i < n; ++i)
	{
        items[i] = new_item(choices[i], "");
    }
	items[n] = (ITEM *)NULL;

	keypad(menu_window, TRUE);
	this_menu = new_menu((ITEM **)items);

	init_pair(1, COLOR_BLUE, COLOR_WHITE);
	init_pair(2, COLOR_BLACK, COLOR_CYAN);

	wbkgd(menu_window, COLOR_PAIR(2));

    set_menu_win(this_menu, menu_window);
    set_menu_sub(this_menu, derwin(menu_window, LINES-5, COLS-1, 3, 1));

    set_menu_mark(this_menu, " * ");

    wattron(menu_window, A_BOLD);
    int len = strlen(header);
	mvwprintw(menu_window, 1, (COLS-len)/2,"%s", header);
	wattroff(menu_window, A_BOLD);

    box(menu_window, 0, 0);
	mvwaddch(menu_window, 2, 0, ACS_LTEE);
	mvwhline(menu_window, 2, 1, ACS_HLINE, COLS-2);
	mvwaddch(menu_window, 2, COLS - 1, ACS_RTEE);

	mvprintw(LINES - 1, 1, "Nawigacja: strzalki w gore i w dol, ENTER by dokonac wyboru (F1 by wyjsc)");
	refresh();

	set_menu_fore(this_menu, COLOR_PAIR(1) | A_REVERSE);
	set_menu_back(this_menu, COLOR_PAIR(2));
	post_menu(this_menu);
	wrefresh(menu_window);

	int c, x=-1;

	while((c = wgetch(menu_window)) != KEY_F(1))
	{   switch(c)
	    {	case KEY_DOWN:
		        menu_driver(this_menu, REQ_DOWN_ITEM);
				break;
			case KEY_UP:
				menu_driver(this_menu, REQ_UP_ITEM);
				break;
            case 10:
			{
                x = item_index(current_item(this_menu));
				break;
			}
		}
		if (x!=-1) break;
		wrefresh(menu_window);
	}

	for (int i=0; i<n+1; i++) free_item(items[i]);
	free_menu(this_menu);

	return x;
}

int menu_main(vector <kategoria*> &kategorie, vector <klient*> &klienci, vector <ksiazka*> &ksiazki){
    WINDOW * menu_window;
    char * choices[] = {
                        "Przegladaj ksiazki",
                        "Przegladaj klientow",
                        "Przegladaj kategorie",
                        "Zapisz",
                        "Zakoncz",
        };
    menu_window = newwin(LINES-1, COLS, 0, 0);
    int x = menu_create(menu_window, choices, 5, "Witaj w Bibliotece!");

    switch (x){
        case 0:
        menu_ksiazki(menu_window, ksiazki);
        break;

        case 1:
        menu_klienci(menu_window, klienci);
        break;

        case 2:
        menu_kategorie(menu_window, kategorie);
        break;

        case 3:
        zapisz();
        break;


        case 4:
        break;

        case -1:
        break;

        default:
        menu_main(kategorie, klienci, ksiazki);
        break;
    }
    delwin(menu_window);
    return x;
}

int dialog(char * choices[], int n, char * header, char * text){
    WINDOW *pop_up;
    MENU* menu;
    ITEM ** items;

    int len = find_longest(choices, n);

    items = (ITEM **)calloc(n+1, sizeof(ITEM *));

    for (int i=0; i<n; i++)
        items[i]=new_item(choices[i], "");
    items[n] = (ITEM *)NULL;

    menu = new_menu(items);
    int height = 7+n, width = 25+len, starty = (LINES-height)/2, startx = (COLS-width)/2;

    init_pair(1, COLOR_WHITE, COLOR_RED);
    init_pair(2, COLOR_BLUE, COLOR_WHITE);

    pop_up = newwin(height, width, starty, startx);
    set_menu_win(menu, pop_up);
    set_menu_sub(menu, derwin(pop_up, 2+n, 5+len, height-(2+n), width/2-len));
    keypad(pop_up, TRUE);
    refresh();

    wbkgd(stdscr, COLOR_PAIR(2));
    wbkgd(pop_up, COLOR_PAIR(1));
    box(pop_up, 0, 0);
	mvwaddch(pop_up, 2, 0, ACS_LTEE);
	mvwhline(pop_up, 2, 1, ACS_HLINE, width-2);
	mvwaddch(pop_up, 2, width-1, ACS_RTEE);
	set_menu_back(menu, COLOR_PAIR(1));

	mvwprintw(pop_up, 1, (width-5)/2,"%s", header);
	mvwprintw(pop_up, 3, 2, "%s", text);

    post_menu(menu);
	wrefresh(pop_up);

	int x=-1, c;

	while((c = wgetch(pop_up)) != KEY_F(1))
	{   switch(c)
	    {	case KEY_UP:
		        menu_driver(menu, REQ_PREV_ITEM);
				break;
			case KEY_DOWN:
				menu_driver(menu, REQ_NEXT_ITEM);
				break;
            case 10:
			{
                x = item_index(current_item(menu));
				break;
			}
		}
		if (x!=-1) break;
		wrefresh(pop_up);
	}

	for (int i=0; i<n+1; i++) free_item(items[i]);
	free_menu(menu);
	delwin(pop_up);

	return x;

}

int zapisz(){
//wywoluje funkcje "" i wyswietla dialog o pomyslnosci zapisu

    char *opcje[] = {"OK"};
    dialog(opcje, 1, "ZAPISZ", "Zapisano pomyslnie.");

   return 0;
}

int usun(){
//wywoluje funkcje "" i wyswietla dialog o pomyslnosci zapisu

    int x;

    char *opcje1[] = {"OK"};
    char *opcje2[] = {"TAK", "NIE"};
    x = dialog(opcje2, 2, "USUN", "Czy chcesz usunac rekord?");
    if (x==0)
        dialog(opcje1, 1, "USUN", "Rekord usuniety.");

   return x;
}

int sort_menu(int choice){
    char *opcje1[] = {"Numeru", "Symbolu", "Nazwy"};

    if (choice==1)
        dialog(opcje1, 3, "SORTUJ", "Sortuj wedlug:");

    return 0;
}

int list_view(char * header, char * list_choices[], int n, char * guide){
//tworzy liste dla dowolnych danych i menu edycji
    WINDOW *window, *menu_window, *list_window;
    MENU* menu, * lista;
    ITEM ** items;
    ITEM ** list_items;
    char * menu_choices[] = {"Sortuj","Filtruj", "Szukaj", "Nowy", "Wroc"};
    int x=-1, c;
    int height = LINES-4, width = COLS-10, starty = (LINES-height)/2, startx = (COLS-width)/2;

    bool upper_menu = false;
	MENU * which_menu;
	WINDOW * which_window;

    init_pair(1, COLOR_WHITE, COLOR_BLUE);
    init_pair(2, COLOR_WHITE, COLOR_RED);
    init_pair(3, COLOR_BLUE, COLOR_WHITE);
    init_pair(4, COLOR_BLACK, COLOR_CYAN);
    init_pair(5, COLOR_BLACK, COLOR_WHITE);

    window = newwin(height, width, starty, startx);
    wbkgd(stdscr, COLOR_PAIR(3));
    wbkgd(window, COLOR_PAIR(1));
    box(window, 0, 0);
    clear_guide();
	mvprintw(LINES - 1, 1, "Nawigacja: strzalki, ENTER by dokonac wyboru, 'z' by zmienic menu");
    mvwprintw(window, 1, (width-5)/2,"%s", header);
	mvwprintw(window, 4, startx, "%s", guide);

    //gorne menu

    items = (ITEM **)calloc(6, sizeof(ITEM *));
    for(int i = 0; i < 5; ++i)
	{
        items[i] = new_item(menu_choices[i], "");
    }
	items[5] = (ITEM *)NULL;

    menu = new_menu(items);

    menu_window = subwin(window, 1, width-2, starty+2, startx+1);
    set_menu_win(menu, menu_window);
    set_menu_sub(menu, derwin(menu_window, 1, width-2, starty+2, startx+1));

    wbkgd(menu_window, COLOR_PAIR(3));
    set_menu_back(menu, COLOR_PAIR(3));
    set_menu_fore(menu, COLOR_PAIR(5));
    set_menu_format(menu, 1, 5);
    set_menu_mark(menu, "  ");
    set_menu_spacing(menu, 0, 0, 3);

    //menu listy

    list_items = (ITEM **)calloc(n+1, sizeof(ITEM *));
    for(int i = 0; i < n; ++i)
	{
        list_items[i] = new_item(list_choices[i], "");
    }
	list_items[n] = (ITEM *)NULL;

	lista = new_menu(list_items);

    list_window = subwin(window, height-6, width-2, starty+6, startx+1);
    set_menu_win(lista, list_window);
    set_menu_sub(lista, derwin(list_window, height-6, width-2, starty+6, startx+1));

    wbkgd(list_window, COLOR_PAIR(1));
    set_menu_back(lista, COLOR_PAIR(1));
    set_menu_fore(lista, COLOR_PAIR(4));
    set_menu_mark(lista, " ");

    keypad(window, TRUE);
    refresh();

    post_menu(lista);
    post_menu(menu);
	wrefresh(window);

    //sterowanie

	while((c = wgetch(window)) != KEY_F(1)){
        if (upper_menu){
            which_menu = menu;
            which_window = menu_window;
        }
        else{
            which_menu = lista;
            which_window = list_window;
        }
        switch(c)
	    {	case KEY_UP:
                if(!upper_menu)
                    menu_driver(which_menu, REQ_PREV_ITEM);
                break;
            case KEY_LEFT:
		        if(upper_menu)
                    menu_driver(which_menu, REQ_PREV_ITEM);;
				break;
			case KEY_DOWN:
                if(!upper_menu)
                    menu_driver(which_menu, REQ_NEXT_ITEM);
			case KEY_RIGHT:
                if(upper_menu)
                    menu_driver(which_menu, REQ_NEXT_ITEM);
				break;
            case '\t':
            {
                if (upper_menu){
                    set_menu_fore(menu, COLOR_PAIR(5));
                    set_menu_fore(lista, COLOR_PAIR(4));
                    wrefresh(menu_window);
                    wrefresh(list_window);
                }
                else{
                    set_menu_fore(menu, COLOR_PAIR(2));
                    set_menu_fore(lista, COLOR_PAIR(5));
                    wrefresh(menu_window);
                    wrefresh(list_window);
                }
                upper_menu = !upper_menu;
                break;
            }
            case 10:
			{
                x = item_index(current_item(which_menu))+1;
				break;
			}
		}
		if (x!=-1) break;
        wrefresh(which_window);
	}

	for (int i=0; i<5; i++)
        free_item(items[i]);

    for (int i=0; i<n+1; i++)
        free_item(list_items[i]);

	free_menu(menu);
	free_menu(lista);

	delwin(window);

    if (upper_menu)
        return -1*x;
    else
        return x;
}

int menu_kategorie(WINDOW * window, vector <kategoria*>& kategorie){
    char ** list_choices = cat_choices(kategorie);
    char * fields[] = {"Symbol", "Nazwa"};
    int result, x=-1, check;

    result = list_view("KATEGORIE", list_choices, (kategorie).size(), "#   Symbol   Nazwa");

    do{
    check = 0;
    if(result>0){
        do{
            x = item_details<kategoria>(window, kategorie[result-1], "KATEGORIA", 3);
                switch (x){
                    case 0:
                        item_form(2, "EDYTUJ", fields);
                        x=-1;
                        break;
                    case 1:{
                        if(usun()==1)
                            x = -1;
                        else
                            menu_kategorie(window, kategorie);
                        break;
                        }
                    case 2:
                        menu_kategorie(window, kategorie);
                        break;
                }
        }while (x<0);
    }

    else
        switch (result){
            case -1:
            {
                char * choices[] = {"Numeru", "Symbolu", "Nazwy"};
                dialog(choices, 3, "SORTUJ", "Sortuj wedlug:");
                menu_kategorie(window, kategorie);
            }
            break;
            case -2:
            {
                char * choices[] = {"OK"};
                dialog(choices, 1, "FILTRUJ", "Brak dostepnych filtrow.");
                menu_kategorie(window, kategorie);
            }
            case -3:
            {
                result = item_search();
                check++;
                break;
            }
            case -4:
                menu_kategorie(window, kategorie);
                break;
        }
        }while (check!=0);

    return 0;
}

int menu_klienci(WINDOW * window, vector <klient*> &klienci){
    char ** list_choices= client_choices(klienci);
    char * fields[] = {"Imie", "Nazwisko", "Adres", "Telefon", "Nr karty"};
    int result, x=-1, check;

    result = list_view("KLIENCI", list_choices, klienci.size(), "#  Nr karty  Imie         Nazwisko");

    do{
    check=0;
    if(result>0){
        do{
            x = item_details<Klient>(window, klienci[result-1], "KLIENCI", 2);
                switch (x){
                    case 0:
                        item_form(5, "EDYTUJ", fields);
                        x=-1;
                        break;
                    case 1:{
                        if(usun()==1)
                            x = -1;
                        else
                            menu_klienci(window, klienci);
                        break;
                        }
                    case 2:
                        menu_klienci(window, klienci);
                        break;
                }
        }while (x<0);
    }

    else
        check=0;
        switch (result){
            case -1:
            {
                char * choices[] = {"Numeru", "Nr karty", "Imienia", "Nazwiska"};
                dialog(choices, 4, "SORTUJ", "Sortuj wedlug:");
                menu_klienci(window, klienci);
            }
            break;
            case -2:
            {
                char * choices[] = {"Tylko wypozyczajacy", "Tylko zadluzeni"};
                dialog(choices, 2, "FILTRUJ", "Filtruj:");
                menu_klienci(window, klienci);
            }
            case -3:
            {
                result = item_search();
                check++;
                break;
            }
            case -4:
                menu_klienci(window, klienci);
                break;
        }
    }while(check!=0);

    return 0;
}

int menu_ksiazki(WINDOW * window, vector <ksiazka*> &ksiazki){
    char ** list_choices = book_choices(ksiazki);
    char * fields[] = {"Autor", "Tytul", "Nr kat.", "Rok wydania"};
    int result, x=-1, check;

    result = list_view("Ksiazki", list_choices, 2, "#  Autor          Tytul");

    do{
    check = 0;
    if(result>0){
        do{
            x = item_details<ksiazka>(window, ksiazki[result-1], "KSIAZKI", 1);
                switch (x){
                    case 0:
                        item_form(4, "EDYTUJ", fields);
                        x=-1;
                        break;
                    case 1:{
                        if(usun()==1)
                            x = -1;
                        else
                            menu_ksiazki(window, ksiazki);
                        break;
                        }
                    case 2:
                        menu_ksiazki(window, ksiazki);
                        break;
                }
        }while (x<0);
    }

    else
        switch (result){
            case -1:
            {
                char * choices[] = {"Numeru", "Autora", "Tytulu"};
                dialog(choices, 3, "SORTUJ", "Sortuj wedlug:");
                menu_ksiazki(window, ksiazki);
            }
            break;
            case -2:
            {
                char * choices[] = {"Tylko wypozyczone", "Tylko zadluzone", "Tylko dostepne"};
                dialog(choices, 3, "FILTRUJ", "Filtruj:");
                menu_ksiazki(window, ksiazki);
            }
            case -3:
            {
                result = item_search();
                check++;
                break;
            }
            case -4:
                add_item();
                menu_ksiazki(window, ksiazki);
                break;
        }
        }while(check!=0);



    return 0;
}

void clear_guide(){
    for(int i = 0; i<COLS-1; i++)
        mvprintw(LINES-1, i+1, " ");
}

template <typename T>
int item_details(WINDOW * window, T *item, char * header, int mode){
    WINDOW *menu_window;
    MENU* menu;
    ITEM ** items;
    char * menu_choices[] = {"Edytuj", "Usun", "Wroc"};
    int x=-1, c;
    int height = LINES-4, width = COLS-10, starty = (LINES-height)/2, startx = (COLS-width)/2;

    init_pair(1, COLOR_WHITE, COLOR_BLUE);
    init_pair(2, COLOR_WHITE, COLOR_RED);
    init_pair(3, COLOR_BLUE, COLOR_WHITE);
    init_pair(4, COLOR_BLACK, COLOR_CYAN);

    window = newwin(height, width, starty, startx);
    wbkgd(stdscr, COLOR_PAIR(3));
    wbkgd(window, COLOR_PAIR(1));
    box(window, 0, 0);
    clear_guide();
	mvprintw(LINES - 1, 1, "Nawigacja: strzalki w bok, ENTER by dokonac wyboru");
    mvwprintw(window, 1, (width-5)/2,"%s", header);

    item->print(window);

    //gorne menu

    items = (ITEM **)calloc(4, sizeof(ITEM *));
    for(int i = 0; i < 3; ++i)
	{
        items[i] = new_item(menu_choices[i], "");
    }
	items[3] = (ITEM *)NULL;

    menu = new_menu(items);

    menu_window = subwin(window, 1, width-2, starty+2, startx+1);
    set_menu_win(menu, menu_window);
    set_menu_sub(menu, derwin(menu_window, 1, width-2, starty+2, startx+1));

    wbkgd(menu_window, COLOR_PAIR(3));
    set_menu_back(menu, COLOR_PAIR(3));
    set_menu_fore(menu, COLOR_PAIR(2));
    set_menu_format(menu, 1, 4);
    set_menu_mark(menu, "  ");
    set_menu_spacing(menu, 0, 0, 3);

    keypad(window, TRUE);
    refresh();

    post_menu(menu);
	wrefresh(window);

    //sterowanie

	while((c = wgetch(window)) != KEY_F(1)){
        switch(c)
	    {
            case KEY_LEFT:
                    menu_driver(menu, REQ_PREV_ITEM);;
				break;
			case KEY_RIGHT:
                    menu_driver(menu, REQ_NEXT_ITEM);
				break;
            case 10:
			{
                x = item_index(current_item(menu));
				break;
			}
		}
		if (x!=-1) break;
        wrefresh(menu_window);
	}

	for (int i=0; i<4; i++)
        free_item(items[i]);

	free_menu(menu);

        return x;

}

int item_form(int n, char * header, char ** fields){


    FIELD *field[n+1];
	FORM  *form;
    WINDOW *window, *menu_window, *form_window, *which;
    MENU* menu;
    ITEM ** items;
	bool upper_menu = false;

    char * menu_choices[] = {"Zapisz", "Wroc"};
	int x=-1, c, cols, rows, m=2;
    int height = LINES-4, width = COLS-10, starty = (LINES-height)/2, startx = (COLS-width)/2;



    init_pair(1, COLOR_WHITE, COLOR_BLUE);
    init_pair(2, COLOR_WHITE, COLOR_RED);
    init_pair(3, COLOR_BLUE, COLOR_WHITE);
    init_pair(4, COLOR_BLACK, COLOR_CYAN);
    init_pair(5, COLOR_BLACK, COLOR_WHITE);

    window = newwin(height, width, starty, startx);
    wbkgd(stdscr, COLOR_PAIR(3));
    wbkgd(window, COLOR_PAIR(1));
    box(window, 0, 0);
    clear_guide();
	mvprintw(LINES - 1, 1, "Nawigacja: strzalki, ENTER by dokonac wyboru, 'z' by zmienic menu");
    mvwprintw(window, 1, (width-5)/2,"%s", header);

    /* Initialize the fields */
    for (int i=0; i<n; i++)
    {
        field[i] = new_field(1, 35, i*2, startx+20, 0, 0);
    }
	field[n] = NULL;

    for (int i=0; i<n; i++){
        set_field_back(field[i], A_UNDERLINE); 	//Print a line for the option
        field_opts_off(field[i], O_AUTOSKIP);
    }
	/* Create the form and post it */
	form = new_form(field);
	form_window = subwin(window, height-6, width-2, starty+4, startx+1);
	set_form_win(form, form_window);
	set_form_sub (form, form_window);
	box(form_window, 0, 0);

	post_form(form);

	for (int i=0; i<n; i++)
    {
        mvwprintw(form_window, i*2, startx+4, "%s:", fields[i]);
    }
	refresh();
	wrefresh(form_window);


    //gorne menu

    items = (ITEM **)calloc(m+1, sizeof(ITEM *));
    for(int i = 0; i < m; ++i)
	{
        items[i] = new_item(menu_choices[i], "");
    }
	items[m] = (ITEM *)NULL;

    menu = new_menu(items);

    menu_window = subwin(window, 1, width-2, starty+2, startx+1);
    set_menu_win(menu, menu_window);
    set_menu_sub(menu, derwin(menu_window, 1, width-2, starty+2, startx+1));

    wbkgd(menu_window, COLOR_PAIR(3));
    set_menu_back(menu, COLOR_PAIR(3));
    set_menu_fore(menu, COLOR_PAIR(5));
    set_menu_format(menu, 1, 4);
    set_menu_mark(menu, "  ");
    set_menu_spacing(menu, 0, 0, 3);



    keypad(window, TRUE);
    refresh();

    post_menu(menu);
	wrefresh(window);

    //sterowanie


    while((c = wgetch(window)) != KEY_F(1)){
        if (upper_menu){
            which = menu_window;
        }
        else{
            which = form_window;
            //echo();
        }
        switch(c)
	    {	case KEY_UP:
                if(!upper_menu){
                    /* Go to previous field */
                    form_driver(form, REQ_PREV_FIELD);
                    form_driver(form, REQ_END_LINE);
                    break;
                }
            case KEY_LEFT:
		        if(upper_menu)
                    menu_driver(menu, REQ_PREV_ITEM);
                else
                    form_driver(form, REQ_PREV_CHAR);
				break;
			case KEY_DOWN:
                if(!upper_menu){
                    /* Go to next field */
                    form_driver(form, REQ_NEXT_FIELD);
                    /* Go to the end of the present buffer */
                    /* Leaves nicely at the last character */
                    form_driver(form, REQ_END_LINE);
                    break;
                }
			case KEY_RIGHT:
                if(upper_menu)
                    menu_driver(menu, REQ_NEXT_ITEM);
                else
                    form_driver(form, REQ_NEXT_CHAR);
				break;
            case '\t':
            {
                if (upper_menu){
                    set_menu_fore(menu, COLOR_PAIR(5));
                   //set_menu_fore(lista, COLOR_PAIR(4));
                    wrefresh(menu_window);
                    //wrefresh(list_window);
                    }
               else{
                    set_menu_fore(menu, COLOR_PAIR(2));
                    //set_menu_fore(lista, COLOR_PAIR(5));
                    wrefresh(menu_window);
                   // wrefresh(list_window);
                   }
                upper_menu = !upper_menu;
                break;
            }
            case 10:
			{
                if (upper_menu)
                    x = item_index(current_item(menu))+1;
                else
                    {
                    form_driver(form, REQ_NEXT_FIELD);
                    }
				break;
			}
			case KEY_BACKSPACE:
			{
                if(!upper_menu)
                    form_driver(form, REQ_DEL_PREV);
                break;
			}
			default:
				/* If this is a normal character, it gets */
				/* Printed				  */
				form_driver(form, c);
				break;
		}
		if (x!=-1) break;
        wrefresh(which);
	}


	for (int i=0; i<m; i++)
        free_item(items[i]);

	free_menu(menu);

	/* Un post form and free the memory */
	unpost_form(form);
	free_form(form);

	for (int i=0; i<n; i++)
        free_field(field[i]);


	delwin(window);

        return x;


}

int sdialog(){
    WINDOW *pop_up, *mwindow, *fwindow;
    MENU* menu;
    ITEM ** items;
    FIELD *field[2];
    FORM * form;

    char * choices[] = {"Szukaj", "Wroc"};

        items = (ITEM **)calloc(3, sizeof(ITEM *));

    for (int i=0; i<2; i++)
        items[i]=new_item(choices[i], "");
    items[2] = (ITEM *)NULL;

    menu = new_menu(items);
    int height = 10, width = 50, starty = (LINES-height)/2, startx = (COLS-width)/2;

    init_pair(1, COLOR_WHITE, COLOR_RED);
    init_pair(2, COLOR_BLUE, COLOR_WHITE);
    init_pair(3, COLOR_BLACK, COLOR_WHITE);

    pop_up = newwin(height, width, starty, startx);
    keypad(pop_up, TRUE);
    mwindow = subwin(pop_up, 1, 15, starty+7, startx+16);
    set_menu_win(menu, mwindow);
    set_menu_sub(menu, mwindow);
    keypad(mwindow, TRUE);
    refresh();

    wbkgd(stdscr, COLOR_PAIR(2));
    wbkgd(pop_up, COLOR_PAIR(1));
    box(pop_up, 0, 0);
	mvwaddch(pop_up, 2, 0, ACS_LTEE);
	mvwhline(pop_up, 2, 1, ACS_HLINE, width-2);
	mvwaddch(pop_up, 2, width-1, ACS_RTEE);
	set_menu_back(menu, COLOR_PAIR(1));
	set_menu_fore(menu, A_REVERSE | COLOR_PAIR(1));

    set_menu_mark(menu, "  ");
	set_menu_format(menu, 1, 2);

    post_menu(menu);

    mvwprintw(pop_up, 1, (width-6)/2,"%s", "SZUKAJ");
	mvwprintw(pop_up, 3, 2, "%s", "Wprowadz wyszukiwana fraze:");


    field[0] = new_field(1, 40, 0, 0, 0, 0);
	field[1] = NULL;
    field_opts_off(field[0], O_AUTOSKIP);

	/* Create the form and post it */
	form = new_form(field);
	fwindow = subwin(pop_up, 1, 40, starty+5, startx+5);
	wbkgd(fwindow, COLOR_PAIR(2));
	set_form_win(form, fwindow);
	set_form_sub (form, fwindow);
	keypad(fwindow, TRUE);

	post_form(form);
	refresh();


	wrefresh(mwindow);

	wrefresh(pop_up);
	wrefresh(fwindow);


    bool lower_menu = false;
    WINDOW * which;
	int x=-1, c;

	while((c = wgetch(pop_up)) != KEY_F(1)){
        if (lower_menu){
            which = mwindow;
        }
        else{
            which = fwindow;
            //echo();
        }
        switch(c)
	    {
            case KEY_LEFT:
		        if(lower_menu)
                    menu_driver(menu, REQ_PREV_ITEM);
                else
                    form_driver(form, REQ_PREV_CHAR);
				break;
			case KEY_RIGHT:
                if(lower_menu)
                    menu_driver(menu, REQ_NEXT_ITEM);
                else
                    form_driver(form, REQ_NEXT_CHAR);
				break;
            case '\t':
            {
                if (lower_menu)
                    set_menu_fore(menu, A_REVERSE | COLOR_PAIR(1));
                else
                    set_menu_fore(menu, COLOR_PAIR(3));
                lower_menu = !lower_menu;
                wrefresh(mwindow);
                break;
            }
            case 10:
			{
                if (lower_menu)
                    x = item_index(current_item(menu))+1;
                else{
                    lower_menu = true;
                    if (lower_menu)
                    set_menu_fore(menu, A_REVERSE | COLOR_PAIR(1));
                    wrefresh(mwindow);
                    }
				break;
			}
			case KEY_BACKSPACE:
			{
                if(!lower_menu)
                    form_driver(form, REQ_DEL_PREV);
                break;
			}
			default:
				if(!lower_menu)
				form_driver(form, c);
				break;
		}
		if (x!=-1) break;
        wrefresh(which);
	}


	for (int i=0; i<2; i++) free_item(items[i]);
    free_field(field[0]);
    free_form(form);
	free_menu(menu);
	delwin(pop_up);

	return x;

}

int item_search(){
//tylko dla kategorii, bo na razie mi sie nie chce
    if (sdialog()==1){
    char * list_choices[] = {"   1   NW1      Polska Literatura Wspolczesna",
                            "   2   NW2      Zagraniczna Literatura Wspolczesna"};
    char * fields[] = {"Symbol", "Nazwa"};
    int result, x=-1;

    result = list_view("WYNIKI", list_choices, 2, "#   Symbol   Nazwa");

    return result;

}
}

int add_item(){
//tez na razie tylko ksiazki

    char * fields[] = {"Autor", "Tytul", "Nr kat.", "Rok wydania"};
    char * ok[] = {"OK"};

    if(item_form(4, "NOWY", fields)==1)
        {
            item_search();
            dialog(ok, 1, "NOWY", "Zapisano pomyslnie.");
        }
}
