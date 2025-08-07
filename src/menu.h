#ifndef MENU_H
#define MENU_H

#include <raylib.h>

typedef struct {
	char* title;
	bool* option;
	int options;
	char** option_name;
	Rectangle main_box;
	Rectangle* option_box;
	int select;
} Menu;

Menu spawn_menu(int options, const char* title);
void set_menu_option(Menu* menu, int option, const char* name);
void update_menu(Menu* menu);

void menu_tick(Menu* menu);
void menu_draw(Menu menu);

void destroy_menu(Menu* menu);

#endif
