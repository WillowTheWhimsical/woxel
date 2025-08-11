#include "draw.h"
#include "input.h"
#include "world.h"
#include "menu.h"
#include "texture.h"
#include "entity.h"

#include <raymath.h>
#include <rlgl.h>

const int block_face_tex[][6] = {
	{T_DIRT, T_DIRT, T_DIRT, T_DIRT, T_DIRT, T_DIRT},
	{T_GRASS, T_GRASS, T_GRASS_TOP, T_DIRT, T_GRASS, T_GRASS},
	{T_STONE, T_STONE, T_STONE, T_STONE, T_STONE, T_STONE},
	{T_WOOD, T_WOOD, T_WOOD, T_WOOD, T_WOOD, T_WOOD},
	{T_RED, T_RED, T_RED, T_RED, T_RED, T_RED},
	{T_ORANGE, T_ORANGE, T_ORANGE, T_ORANGE, T_ORANGE, T_ORANGE},
	{T_YELLOW, T_YELLOW, T_YELLOW, T_YELLOW, T_YELLOW, T_YELLOW},
	{T_GREEN, T_GREEN, T_GREEN, T_GREEN, T_GREEN, T_GREEN},
	{T_CYAN, T_CYAN, T_CYAN, T_CYAN, T_CYAN, T_CYAN},
	{T_BLUE, T_BLUE, T_BLUE, T_BLUE, T_BLUE, T_BLUE},
	{T_PURPLE, T_PURPLE, T_PURPLE, T_PURPLE, T_PURPLE, T_PURPLE},
	{T_MAGENTA, T_MAGENTA, T_MAGENTA, T_MAGENTA, T_MAGENTA, T_MAGENTA},
	{T_PSB, T_PSB, T_PSB, T_PSB, T_PSB, T_PSB},
	{T_RED, T_YELLOW, T_GREEN, T_BLUE, T_PURPLE, T_MAGENTA}
};

void draw_block(float x, float y, float z, float w, float h, float l, int block_id, bool cull_face[6]) {
	rlBegin(RL_QUADS);
	rlColor4ub(255, 255, 255, 255);

	if (!cull_face[0]) {
		rlSetTexture(texture[block_face_tex[block_id][0]].id);
		rlNormal3f(0.0f, 0.0f, 1.0f);
		rlTexCoord2f(0, 1);
		rlVertex3f(x, y, z + l);
		rlTexCoord2f(1, 1);
		rlVertex3f(x + w, y, z + l);
		rlTexCoord2f(1, 0);
		rlVertex3f(x + w, y + h, z + l);
		rlTexCoord2f(0, 0);
		rlVertex3f(x, y + h, z + l);
	}

	if (!cull_face[1]) {
		rlSetTexture(texture[block_face_tex[block_id][1]].id);
		rlNormal3f(0.0f, 0.0f, -1.0f);
		rlTexCoord2f(1, 1);
		rlVertex3f(x, y, z);
		rlTexCoord2f(1, 0);
		rlVertex3f(x, y + h, z);
		rlTexCoord2f(0, 0);
		rlVertex3f(x + w, y + h, z);
		rlTexCoord2f(0, 1);
		rlVertex3f(x + w, y, z);
	}

	if (!cull_face[2]) {
		rlSetTexture(texture[block_face_tex[block_id][2]].id);
		rlNormal3f(0.0f, 1.0f, 0.0f);
		rlTexCoord2f(0, 0);
		rlVertex3f(x, y + h, z);
		rlTexCoord2f(0, 1);
		rlVertex3f(x, y + h, z + l);
		rlTexCoord2f(1, 1);
		rlVertex3f(x + w, y + h, z + l);
		rlTexCoord2f(1, 0);
		rlVertex3f(x + w, y + h, z);
	}

	if (!cull_face[3]) {
		rlSetTexture(texture[block_face_tex[block_id][3]].id);
		rlNormal3f(0.0f, -1.0f, 0.0f);
		rlTexCoord2f(1, 0);
		rlVertex3f(x, y, z);
		rlTexCoord2f(0, 0);
		rlVertex3f(x + w, y, z);
		rlTexCoord2f(0, 1);
		rlVertex3f(x + w, y, z + l);
		rlTexCoord2f(1, 1);
		rlVertex3f(x, y, z + l);
	}

	if (!cull_face[4]) {
		rlSetTexture(texture[block_face_tex[block_id][4]].id);
		rlNormal3f(1.0f, 0.0f, 0.0f);
		rlTexCoord2f(1, 1);
		rlVertex3f(x + w, y, z);
		rlTexCoord2f(1, 0);
		rlVertex3f(x + w, y + h, z);
		rlTexCoord2f(0, 0);
		rlVertex3f(x + w, y + h, z + l);
		rlTexCoord2f(0, 1);
		rlVertex3f(x + w, y, z + l);
	}


	if (!cull_face[5]) {
		rlSetTexture(texture[block_face_tex[block_id][5]].id);
		rlNormal3f(-1.0f, 0.0f, 0.0f);
		rlTexCoord2f(0, 1);
		rlVertex3f(x, y, z);
		rlTexCoord2f(1, 1);
		rlVertex3f(x, y, z + l);
		rlTexCoord2f(1, 0);
		rlVertex3f(x, y + h, z + l);
		rlTexCoord2f(0, 0);
		rlVertex3f(x, y + h, z);
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
	for (int z = 0; z < world.l; z++) {
		for (int y = 0; y < world.h; y++) {
			for (int x = 0; x < world.w; x++) {
				int block = get_block(x, y, z);
				if (block >= 0) {
					bool cull[6] = {
						get_block(x, y, z + 1) >= 0,
						get_block(x, y, z - 1) >= 0,
						get_block(x, y + 1, z) >= 0,
						get_block(x, y - 1, z) >= 0,
						get_block(x + 1, y, z) >= 0,
						get_block(x - 1, y, z) >= 0,
					};
					draw_block(x, y, z, 1, 1, 1, block, cull);
				}
			}
		}
	}
}

void draw_entities(Camera cam) {
	for_entities(e)
		if (e->tex >= 0) {
			Texture* tex = &texture[e->tex];
			Vector2 pdir = Vector2Normalize(Vector2Subtract((Vector2){e->pos.x, e->pos.z}, (Vector2){cam.position.x, cam.position.z}));
			Vector2 edir = Vector2Normalize(e->dir);
			float angle = 180 / PI * (atan2f(-pdir.y, pdir.x) + atan2f(-edir.y, -edir.x)) + 22;
			if (angle < 0) angle += 360;
			int dir = angle / 45;
			int offset = dir;
			bool flip = false;
			if (dir > 4) {
				flip = true;
				offset = 8 - offset;
			}
			Rectangle src = {offset * e->frame_size.x + (flip ? e->frame_size.x : 0), 0, e->frame_size.x * (flip ? -1 : 1), e->frame_size.y};
			DrawBillboardPro(cam, *tex, src, (Vector3){e->pos.x, e->pos.y, e->pos.z}, cam.up, (Vector2){1, 1}, (Vector2){0.5, 0}, 0, WHITE);
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
			draw_entities(*cam);
		EndMode3D();

		DrawTexturePro(texture[block_face_tex[*block][0]], (Rectangle){0, 0, texture[*block].width, texture[*block].height}, (Rectangle){GetScreenWidth() - 32, 0, 32, 32}, (Vector2){0, 0}, 0, WHITE);

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
