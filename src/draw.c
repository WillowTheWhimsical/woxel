#include "draw.h"
#include "input.h"
#include "world.h"
#include "menu.h"
#include "texture.h"
#include "entity.h"
#include "blocks.h"

#include <rlgl.h>

void draw_block(float x, float y, float z, float width, float height, float length, int block, bool cull[6]) {
	Texture* tex = &texture[block];
	Rectangle source = {0, 0, tex->width, tex->height};
	float texWidth = (float)tex->width;
	float texHeight = (float)tex->height;

	rlSetTexture(tex->id);

	rlBegin(RL_QUADS);
	rlColor4ub(255, 255, 255, 255);

	if (!cull[0]) {
		rlNormal3f(0.0f, 0.0f, 1.0f);
		rlTexCoord2f(source.x / texWidth, (source.y + source.height) / texHeight);
		rlVertex3f(x, y, z + length);
		rlTexCoord2f((source.x + source.width) / texWidth, (source.y + source.height) / texHeight);
		rlVertex3f(x + width, y, z + length);
		rlTexCoord2f((source.x + source.width) / texWidth, source.y / texHeight);
		rlVertex3f(x + width, y + height, z + length);
		rlTexCoord2f(source.x / texWidth, source.y / texHeight);
		rlVertex3f(x, y + height, z + length);
	}

	if (!cull[1]) {
		rlNormal3f(0.0f, 0.0f, -1.0f);
		rlTexCoord2f((source.x + source.width) / texWidth, (source.y + source.height) / texHeight);
		rlVertex3f(x, y, z);
		rlTexCoord2f((source.x + source.width) / texWidth, source.y / texHeight);
		rlVertex3f(x, y + height, z);
		rlTexCoord2f(source.x / texWidth, source.y / texHeight);
		rlVertex3f(x + width, y + height, z);
		rlTexCoord2f(source.x / texWidth, (source.y + source.height) / texHeight);
		rlVertex3f(x + width, y, z);
	}

	if (!cull[2]) {
		rlNormal3f(0.0f, 1.0f, 0.0f);
		rlTexCoord2f(source.x / texWidth, source.y / texHeight);
		rlVertex3f(x, y + height, z);
		rlTexCoord2f(source.x / texWidth, (source.y + source.height) / texHeight);
		rlVertex3f(x, y + height, z + length);
		rlTexCoord2f((source.x + source.width) / texWidth, (source.y + source.height) / texHeight);
		rlVertex3f(x + width, y + height, z + length);
		rlTexCoord2f((source.x + source.width) / texWidth, source.y / texHeight);
		rlVertex3f(x + width, y + height, z);
	}

	if (!cull[3]) {
		rlNormal3f(0.0f, -1.0f, 0.0f);
		rlTexCoord2f((source.x + source.width) / texWidth, source.y / texHeight);
		rlVertex3f(x, y, z);
		rlTexCoord2f(source.x / texWidth, source.y / texHeight);
		rlVertex3f(x + width, y, z);
		rlTexCoord2f(source.x / texWidth, (source.y + source.height) / texHeight);
		rlVertex3f(x + width, y, z + length);
		rlTexCoord2f((source.x + source.width) / texWidth, (source.y + source.height) / texHeight);
		rlVertex3f(x, y, z + length);
	}

	if (!cull[4]) {
		rlNormal3f(1.0f, 0.0f, 0.0f);
		rlTexCoord2f((source.x + source.width) / texWidth, (source.y + source.height) / texHeight);
		rlVertex3f(x + width, y, z);
		rlTexCoord2f((source.x + source.width) / texWidth, source.y / texHeight);
		rlVertex3f(x + width, y + height, z);
		rlTexCoord2f(source.x / texWidth, source.y / texHeight);
		rlVertex3f(x + width, y + height, z + length);
		rlTexCoord2f(source.x / texWidth, (source.y + source.height) / texHeight);
		rlVertex3f(x + width, y, z + length);
	}


	if (!cull[5]) {
		rlNormal3f(-1.0f, 0.0f, 0.0f);
		rlTexCoord2f(source.x / texWidth, (source.y + source.height) / texHeight);
		rlVertex3f(x, y, z);
		rlTexCoord2f((source.x + source.width) / texWidth, (source.y + source.height) / texHeight);
		rlVertex3f(x, y, z + length);
		rlTexCoord2f((source.x + source.width) / texWidth, source.y / texHeight);
		rlVertex3f(x, y + height, z + length);
		rlTexCoord2f(source.x / texWidth, source.y / texHeight);
		rlVertex3f(x, y + height, z);
	}

	rlEnd();

	rlSetTexture(0);
}

// not implemented
void draw_titlescreen() {
	Texture tex = texture[T_DIRT];
	DrawTexturePro(tex, (Rectangle){0, 0, tex.width, tex.height}, (Rectangle){0, 0, GetScreenWidth(), GetScreenHeight()}, (Vector2){0, 0}, 0, WHITE);

	Vector2 size = {64, 64};
	DrawRectangle(0, 0, size.x, size.y, GRAY);
}

void draw_world() {
	for (int t = 0; t < world.l; t++) {
		for (int j = 0; j < world.h; j++) {
			for (int i = 0; i < world.w; i++) {
				int block = get_block(i, j, t);
				if (block >= 0) {
					bool cull[6] = {
						get_block(i, j, t + 1) >= 0,
						get_block(i, j, t - 1) >= 0,
						get_block(i, j + 1, t) >= 0,
						get_block(i, j - 1, t) >= 0,
						get_block(i + 1, j, t) >= 0,
						get_block(i - 1, j, t) >= 0,
					};
					draw_block(i, j, t, 1, 1, 1, block, cull);
				}
			}
		}
	}
}

void render() {
	Camera3D* cam = 0;
	int* block = 0;
	int* in_menu = 0;
	Menu* inventory_menu = 0;
	Menu* pause_menu = 0;
	for_entities(e)
		if (e->type == E_PLAYER) {
			cam = e->var[0];
			block = e->var[1];
			in_menu = e->var[2];
			inventory_menu = e->var[3];
			pause_menu = e->var[4];
			break;
		}
	}
	if (cam == 0 || block == 0 || inventory_menu == 0 || pause_menu == 0) return;

	UpdateCameraPro(cam, (Vector3){0, 0, 0},
		(Vector3){input.mdx * input.sensitivity * IsCursorHidden(),
							input.mdy * input.sensitivity * IsCursorHidden(), 0}, 0
	);
	
	BeginDrawing();
		ClearBackground((Color){50, 150, 200});

		BeginMode3D(*cam);
			draw_world();
		EndMode3D();

		DrawTexturePro(texture[*block], (Rectangle){0, 0, texture[*block].width, texture[*block].height}, (Rectangle){GetScreenWidth() - 32, 0, 32, 32}, (Vector2){0, 0}, 0, WHITE);

		if (!*in_menu) {
			DisableCursor();
			DrawLineEx((Vector2){GetScreenWidth() / 2.f - 8, GetScreenHeight() / 2.f - 8}, (Vector2){GetScreenWidth() / 2.f + 8, GetScreenHeight() / 2.f + 8}, 1, WHITE);
			DrawLineEx((Vector2){GetScreenWidth() / 2.f + 8, GetScreenHeight() / 2.f - 8}, (Vector2){GetScreenWidth() / 2.f - 8, GetScreenHeight() / 2.f + 8}, 1, WHITE);
		}
		else {
			EnableCursor();
			switch (*in_menu) {
				case 1:
					menu_draw(*inventory_menu);
					break;
				case 2:
					menu_draw(*pause_menu);
					break;
			}
		}

		DrawFPS(0, 0);
	EndDrawing();
}
