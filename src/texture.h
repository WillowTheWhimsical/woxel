#ifndef TEXTURE_H
#define TEXTURE_H

#include <raylib.h>

extern Texture* texture;

enum Textures {
	T_DIRT,
	T_GRASS,
	T_STONE,

	TEXTURES
};

void load_textures();
void unload_textures();

#endif
