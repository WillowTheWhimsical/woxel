#include "draw.h"
#include "input.h"
#include "world.h"
#include "entity.h"
#include "blocks.h"

#include <raylib.h>
#include <raymath.h>

void draw_world() {
	for_world {
		int block = get_block(i, j, t);
		if (block > B_AIR) {
			Color color = WHITE;
			switch (block) {
				case B_DIRT:
					color = BROWN;
					break;
				case B_GRASS:
					color = GREEN;
					break;
				case B_STONE:
					color = GRAY;
					break;
			}
			DrawCube((Vector3){i, j, t}, 1, 1, 1, color);
		}
	}}}
}

void test1(Camera3D cam) {
	Vector3 dir = Vector3Normalize(Vector3Subtract(cam.target, cam.position));
	for (float i = 0; i < 5; i += 0.1) {
		int x = roundf(cam.position.x + dir.x * i);
		int y = roundf(cam.position.y + dir.y * i);
		int z = roundf(cam.position.z + dir.z * i);
		if (get_block(x, y, z) > B_AIR) {
			DrawCubeWires((Vector3){x, y, z}, 1, 1, 1, RED);
			return;
		}
	}
}

void test2(Camera3D cam) {
	Vector3 dir = Vector3Normalize(Vector3Subtract(cam.target, cam.position));
	for (float i = 0; i < 5; i += 0.1) {
		int x = roundf(cam.position.x + dir.x * i);
		int y = roundf(cam.position.y + dir.y * i);
		int z = roundf(cam.position.z + dir.z * i);
		if (get_block(x, y, z) > B_AIR) {
			x = roundf(cam.position.x + dir.x * (i - 0.1));
			y = roundf(cam.position.y + dir.y * (i - 0.1));
			z = roundf(cam.position.z + dir.z * (i - 0.1));
			DrawCubeWires((Vector3){x, y, z}, 1, 1, 1, BLUE);
			return;
		}
	}
}

void render() {
	Camera3D* cam;
	for_entity(e)
		if (e->type == E_PLAYER) {
			cam = e->var[0];
			break;
		}
	}

	UpdateCameraPro(cam, (Vector3){0, 0, 0},
		(Vector3){input.mdx * input.sensitivity * IsCursorHidden(),
							input.mdy * input.sensitivity * IsCursorHidden(), 0}, 0
	);
	
	BeginDrawing();
		ClearBackground(BLACK);

		BeginMode3D(*cam);
			draw_world();
			test1(*cam);
			test2(*cam);
		EndMode3D();

		DrawLineEx((Vector2){GetScreenWidth() / 2.f - 8, GetScreenHeight() / 2.f - 8}, (Vector2){GetScreenWidth() / 2.f + 8, GetScreenHeight() / 2.f + 8}, 1, WHITE);
		DrawLineEx((Vector2){GetScreenWidth() / 2.f + 8, GetScreenHeight() / 2.f - 8}, (Vector2){GetScreenWidth() / 2.f - 8, GetScreenHeight() / 2.f + 8}, 1, WHITE);

		DrawFPS(0, 0);
	EndDrawing();
}
