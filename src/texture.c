#include "texture.h"

#include <malloc.h>

Texture* texture;

void load_textures() {
	texture = malloc(sizeof(Texture) * TEXTURES);
	for (int i = 0; i < TEXTURES; i++)
		texture[i] = LoadTexture(TextFormat("res/%d.png", i));
}

void unload_textures() {
	for (int i = 0; i < TEXTURES; i++)
		UnloadTexture(texture[i]);
	free(texture);
}
