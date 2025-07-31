#include "draw.h"
#include "input.h"
#include "world.h"
#include "menu.h"
#include "texture.h"
#include "entity.h"
#include "blocks.h"

#include <rlgl.h>

void draw_block(float x, float y, float z, float width, float height, float length, int block, bool cull[6]) {
	Texture* tex = &texture[block - 1];
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
				if (block > B_AIR) {
					bool cull[6] = {
						get_block(i, j, t + 1) > B_AIR,
						get_block(i, j, t - 1) > B_AIR,
						get_block(i, j + 1, t) > B_AIR,
						get_block(i, j - 1, t) > B_AIR,
						get_block(i + 1, j, t) > B_AIR,
						get_block(i - 1, j, t) > B_AIR,
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
	bool* in_menu = 0;
	Menu* menu = 0;
	for_entities(e)
		if (e->type == E_PLAYER) {
			cam = e->var[0];
			block = e->var[1];
			in_menu = e->var[2];
			menu = e->var[3];
			break;
		}
	}
	if (cam == 0 || block == 0 || menu == 0) return;

	UpdateCameraPro(cam, (Vector3){0, 0, 0},
		(Vector3){input.mdx * input.sensitivity * IsCursorHidden(),
							input.mdy * input.sensitivity * IsCursorHidden(), 0}, 0
	);
	
	BeginDrawing();
		ClearBackground((Color){50, 150, 200});

		BeginMode3D(*cam);
			draw_world();
		EndMode3D();

		DrawTexturePro(texture[*block - 1], (Rectangle){0, 0, texture[*block - 1].width, texture[*block - 1].height}, (Rectangle){GetScreenWidth() - 32, 0, 32, 32}, (Vector2){0, 0}, 0, WHITE);

		if (!*in_menu) {
			DisableCursor();
			DrawLineEx((Vector2){GetScreenWidth() / 2.f - 8, GetScreenHeight() / 2.f - 8}, (Vector2){GetScreenWidth() / 2.f + 8, GetScreenHeight() / 2.f + 8}, 1, WHITE);
			DrawLineEx((Vector2){GetScreenWidth() / 2.f + 8, GetScreenHeight() / 2.f - 8}, (Vector2){GetScreenWidth() / 2.f - 8, GetScreenHeight() / 2.f + 8}, 1, WHITE);
		}
		else {
			EnableCursor();
			menu_draw(menu);
		}

		DrawFPS(0, 0);
	EndDrawing();
}
