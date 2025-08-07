#include "menu.h"
#include "sound.h"
#include "input.h"

#include <malloc.h>
#include <string.h>
#include <raylib.h>

const int fontsize = 20;
const int fontsize_title = fontsize * 1.5;

void draw_box(Rectangle rect, bool invert);

Menu spawn_menu(int options, const char* title) {
	Menu menu;

	menu.title = malloc(sizeof(char) * strlen(title) - 1);
	strcpy(menu.title, title);
	menu.options = options;
	menu.option = malloc(sizeof(bool*) * options);
	menu.option_name = malloc(sizeof(char*) * options);
	menu.option_box = malloc(sizeof(Rectangle) * options);
	menu.select = 0;

	for (int i = 0; i < options; i++)
		menu.option[i] = false;

	return menu;
}

void set_menu_option(Menu* menu, int option, const char* name) {
	menu->option_name[option] = malloc(sizeof(char) * strlen(name));
	strcpy(menu->option_name[option], name);
}

void update_menu(Menu* menu) {
	float width = 0;
	for (int i = 0; i < menu->options; i++) {
		int length = MeasureText(menu->option_name[i], fontsize);
		if (width < length) width = length;
	}
	float height = menu->options * fontsize + menu->options * 6 + 4;

	menu->main_box = (Rectangle){GetScreenWidth() * 0.5 - width * 0.5 - 4, GetScreenHeight() * 0.5 - height * 0.5, width + 15, height};

	for (int i = 0; i < menu->options; i++) {
		int length = MeasureText(menu->option_name[i], fontsize);
		menu->option_box[i] = (Rectangle){
			GetScreenWidth() * 0.5 - length * 0.5,
			menu->main_box.y + 4 + i * fontsize + i * 6,
			length + 7,
			fontsize + 2
		};
	}
}

void menu_tick(Menu* menu) {
	static bool confirming;

	if (input.mconfirm && !confirming) {
		confirming = true;
		PlaySound(sound[S_SELECT]);
	}

	if (!input.mconfirm && confirming) {
		input.mconfirm = false;
		confirming = false;
		menu->option[menu->select] = !menu->option[menu->select];
	}

	if (!confirming) {
		if (input.mup) {
			input.mup = false;
			if (menu->select > 0) {
				menu->select--;
				PlaySound(sound[S_BLIP]);
			}
		}
		if (input.mdown) {
			input.mdown = false;
			if (menu->select < menu->options - 1) {
				menu->select++;
				PlaySound(sound[S_BLIP]);
			}
		}
	}
}

void menu_draw(Menu menu) {
	const int fontsize = 20;
	const int fontsize_title = fontsize * 1.5;

	draw_box(menu.main_box, false);
	DrawText(menu.title, GetScreenWidth() * 0.5 - MeasureText(menu.title, fontsize_title) * 0.5, menu.main_box.y - fontsize_title, fontsize_title, WHITE);

	for (int i = 0; i < menu.options; i++) {
		draw_box(menu.option_box[i], menu.select == i && input.mconfirm);
		DrawText(menu.option_name[i], menu.option_box[i].x + 3, menu.option_box[i].y + 1, fontsize, WHITE);
	}

	DrawText(">", menu.option_box[menu.select].x - fontsize * 0.5, menu.option_box[menu.select].y, fontsize, YELLOW);
}

void destroy_menu(Menu* menu) {
	free(menu->option);
	for (int i = 0; i < menu->options; i++)
		free(menu->option_name[i]);
	free(menu->option_name);
}

void draw_box(Rectangle rect, bool invert) {
	const Color c0 = {75, 75, 75, 255};
	const Color c1 = {125, 125, 125, 255};
	const Color c2 = {25, 25, 25, 255};
	DrawRectangleRec(rect, c0);
	DrawLine(rect.x + rect.width, rect.y + rect.height, rect.x + rect.width, rect.y, invert ? c1 : c2);
	DrawLine(rect.x + rect.width, rect.y + rect.height, rect.x, rect.y + rect.height, invert ? c1 : c2);
	DrawLine(rect.x, rect.y, rect.x + rect.width, rect.y, invert ? c2 : c1);
	DrawLine(rect.x, rect.y, rect.x, rect.y + rect.height, invert ? c2 : c1);
}
