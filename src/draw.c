#include "draw.h"
#include "input.h"
#include "world.h"
#include "texture.h"
#include "entity.h"
#include "blocks.h"

#include <raylib.h>
#include <raymath.h>
#include <rlgl.h>

void DrawCubeTextureRec(Texture2D texture, Rectangle source, Vector3 position, float width, float height, float length, Color color) {
	float x = position.x;
	float y = position.y;
	float z = position.z;
	float texWidth = (float)texture.width;
	float texHeight = (float)texture.height;

	rlSetTexture(texture.id);

	rlBegin(RL_QUADS);
	rlColor4ub(color.r, color.g, color.b, color.a);

	rlNormal3f(0.0f, 0.0f, 1.0f);
	rlTexCoord2f(source.x / texWidth, (source.y + source.height) / texHeight);
	rlVertex3f(x, y, z + 1);
	rlTexCoord2f((source.x + source.width) / texWidth, (source.y + source.height) / texHeight);
	rlVertex3f(x + 1, y, z + 1);
	rlTexCoord2f((source.x + source.width) / texWidth, source.y / texHeight);
	rlVertex3f(x + 1, y + 1, z + 1);
	rlTexCoord2f(source.x / texWidth, source.y / texHeight);
	rlVertex3f(x, y + 1, z + 1);

	rlNormal3f(0.0f, 0.0f, -1.0f);
	rlTexCoord2f((source.x + source.width) / texWidth, (source.y + source.height) / texHeight);
	rlVertex3f(x, y, z);
	rlTexCoord2f((source.x + source.width) / texWidth, source.y / texHeight);
	rlVertex3f(x, y + 1, z);
	rlTexCoord2f(source.x / texWidth, source.y / texHeight);
	rlVertex3f(x + 1, y + 1, z);
	rlTexCoord2f(source.x / texWidth, (source.y + source.height) / texHeight);
	rlVertex3f(x + 1, y, z);

	rlNormal3f(0.0f, 1.0f, 0.0f);
	rlTexCoord2f(source.x / texWidth, source.y / texHeight);
	rlVertex3f(x, y + 1, z);
	rlTexCoord2f(source.x / texWidth, (source.y + source.height) / texHeight);
	rlVertex3f(x, y + 1, z + 1);
	rlTexCoord2f((source.x + source.width) / texWidth, (source.y + source.height) / texHeight);
	rlVertex3f(x + 1, y + 1, z + 1);
	rlTexCoord2f((source.x + source.width) / texWidth, source.y / texHeight);
	rlVertex3f(x + 1, y + 1, z);

	rlNormal3f(0.0f, -1.0f, 0.0f);
	rlTexCoord2f((source.x + source.width) / texWidth, source.y / texHeight);
	rlVertex3f(x, y, z);
	rlTexCoord2f(source.x / texWidth, source.y / texHeight);
	rlVertex3f(x + 1, y, z);
	rlTexCoord2f(source.x / texWidth, (source.y + source.height) / texHeight);
	rlVertex3f(x + 1, y, z + 1);
	rlTexCoord2f((source.x + source.width) / texWidth, (source.y + source.height) / texHeight);
	rlVertex3f(x, y, z + 1);

	rlNormal3f(1.0f, 0.0f, 0.0f);
	rlTexCoord2f((source.x + source.width) / texWidth, (source.y + source.height) / texHeight);
	rlVertex3f(x + 1, y, z);
	rlTexCoord2f((source.x + source.width) / texWidth, source.y / texHeight);
	rlVertex3f(x + 1, y + 1, z);
	rlTexCoord2f(source.x / texWidth, source.y / texHeight);
	rlVertex3f(x + 1, y + 1, z + 1);
	rlTexCoord2f(source.x / texWidth, (source.y + source.height) / texHeight);
	rlVertex3f(x + 1, y, z + 1);

	rlNormal3f(-1.0f, 0.0f, 0.0f);
	rlTexCoord2f(source.x / texWidth, (source.y + source.height) / texHeight);
	rlVertex3f(x, y, z);
	rlTexCoord2f((source.x + source.width) / texWidth, (source.y + source.height) / texHeight);
	rlVertex3f(x, y, z + 1);
	rlTexCoord2f((source.x + source.width) / texWidth, source.y / texHeight);
	rlVertex3f(x, y + 1, z + 1);
	rlTexCoord2f(source.x / texWidth, source.y / texHeight);
	rlVertex3f(x, y + 1, z);

	rlEnd();

	rlSetTexture(0);
}

void draw_world() {
	for_world {
		int block = get_block(i, j, t);
		if (block > B_AIR) {
			DrawCubeTextureRec(texture[block - 1], (Rectangle){0, 0, texture[block - 1].width, texture[block - 1].height}, (Vector3){i, j, t}, 1, 1, 1, WHITE);
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
		ClearBackground((Color){50, 150, 200});

		BeginMode3D(*cam);
			draw_world();
		EndMode3D();

		DrawTexturePro(texture[*block - 1], (Rectangle){0, 0, texture[*block - 1].width, texture[*block - 1].height}, (Rectangle){GetScreenWidth() - 32, 0, 32, 32}, (Vector2){0, 0}, 0, WHITE);

		DrawLineEx((Vector2){GetScreenWidth() / 2.f - 8, GetScreenHeight() / 2.f - 8}, (Vector2){GetScreenWidth() / 2.f + 8, GetScreenHeight() / 2.f + 8}, 1, WHITE);
		DrawLineEx((Vector2){GetScreenWidth() / 2.f + 8, GetScreenHeight() / 2.f - 8}, (Vector2){GetScreenWidth() / 2.f - 8, GetScreenHeight() / 2.f + 8}, 1, WHITE);

		DrawFPS(0, 0);
	EndDrawing();
}
