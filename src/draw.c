#include "draw.h"
#include "input.h"
#include "world.h"
#include "entity.h"
#include "blocks.h"

#include <raylib.h>
#include <raymath.h>

Color get_block_color(int block) {
	Color color = WHITE;
	if (block > B_AIR) {
		switch (block) {
			case B_DIRT:
				color = (Color){100, 50, 0};
				break;
			case B_GRASS:
				color = (Color){50, 150, 50};
				break;
			case B_STONE:
				color = (Color){50, 50, 50};
				break;
		}
	}
	color.a = 255;
	return color;
}

void draw_world() {
	for_world {
		int block = get_block(i, j, t);
		if (block > B_AIR) {
			Color fc = get_block_color(block), bc = fc;
			bc.r /= 2;
			bc.g /= 2;
			bc.b /= 2;

			DrawCube((Vector3){i, j, t}, 1, 1, 1, fc);
			DrawCubeWires((Vector3){i, j, t}, 1, 1, 1, bc);
		}
	}}}
}

void render() {
	Camera3D* cam = 0;
	int* block = 0;
	for_entity(e)
		if (e->type == E_PLAYER) {
			cam = e->var[0];
			block = e->var[1];
			break;
		}
	}
	if (cam == 0 || block == 0) return;

	UpdateCameraPro(cam, (Vector3){0, 0, 0},
		(Vector3){input.mdx * input.sensitivity * IsCursorHidden(),
							input.mdy * input.sensitivity * IsCursorHidden(), 0}, 0
	);
	
	BeginDrawing();
		ClearBackground(BLACK);

		BeginMode3D(*cam);
			draw_world();
		EndMode3D();

		Color fc = get_block_color(*block), bc = fc;
		bc.r /= 2;
		bc.g /= 2;
		bc.b /= 2;
		DrawRectangle(GetScreenWidth() - 32, 0, 32, 32, fc);
		DrawRectangleLines(GetScreenWidth() - 32, 0, 32, 32, bc);

		DrawLineEx((Vector2){GetScreenWidth() / 2.f - 8, GetScreenHeight() / 2.f - 8}, (Vector2){GetScreenWidth() / 2.f + 8, GetScreenHeight() / 2.f + 8}, 1, WHITE);
		DrawLineEx((Vector2){GetScreenWidth() / 2.f + 8, GetScreenHeight() / 2.f - 8}, (Vector2){GetScreenWidth() / 2.f - 8, GetScreenHeight() / 2.f + 8}, 1, WHITE);

		DrawFPS(0, 0);
	EndDrawing();
}
