#include "texture.h"
#include "blocks.h"

#include <libwif.h>
#include <malloc.h>

Texture* texture;

Image convert_wif(wif_image wif) {
	Image image = GenImageChecked(wif.width, wif.height, 5, 5, (Color){255, 0, 255, 255}, BLACK);
	for (int y = 0; y < wif.height; y++) {
		for (int x = 0; x < wif.width; x++) {
			int windex = (x + y * wif.width) * 3;
			Color color = {wif.data[windex], wif.data[windex + 1], wif.data[windex + 2], 255};
			ImageDrawPixel(&image, x, y, color);
		}
	}
	return image;
}

void load_textures() {
	texture = malloc(sizeof(Texture) * TEXTURES);
	for (int i = 0; i < TEXTURES; i++) {
		Image image;
		wif_image wif;

		wif_parsef(&wif, TextFormat("res/img/%s.wif", block_name[i]));
		image = convert_wif(wif);
		texture[i] = LoadTextureFromImage(image);

		wif_free(&wif);
		UnloadImage(image);
	}
}

void unload_textures() {
	for (int i = 0; i < TEXTURES; i++)
		UnloadTexture(texture[i]);
	free(texture);
}
