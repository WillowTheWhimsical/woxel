#ifndef TEXTURE_H
#define TEXTURE_H

#include <raylib.h>

extern Texture* texture;

enum Textures {
	T_DIRT,
	T_GRASS, T_GRASS_TOP,
	T_STONE,
	T_WOOD,
	T_RED,
	T_ORANGE,
	T_YELLOW,
	T_GREEN,
	T_CYAN,
	T_BLUE,
	T_PURPLE,
	T_MAGENTA,
	T_PSB,
	T_GOOBERT,

	TEXTURES
};

extern const char* texture_name[];

void load_textures();
void unload_textures();

#endif
