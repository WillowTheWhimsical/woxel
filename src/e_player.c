#include "entity.h"
#include "input.h"
#include "world.h"
#include "blocks.h"

#include <malloc.h>
#include <raylib.h>
#include <raymath.h>

Vector3 move(Vector2 dir);
Vector3 get_look_block(Camera3D);
void place_block(Camera3D cam, int block);

void E_PLAYER_INIT(Entity* this) {
	this->data = malloc(sizeof(Camera3D) + sizeof(int));
	this->var = malloc(sizeof(void*) * 2);
	this->var[0] = this->data;
	this->var[1] = this->data + sizeof(Camera3D);

	Camera3D* cam = this->var[0];
	cam->projection = CAMERA_PERSPECTIVE;
	cam->position = (Vector3){10, 12, 10};
	cam->target = (Vector3){10, 12, 9};
	cam->up = (Vector3){0, 1, 0};
	cam->fovy = 90;

	int* block = this->var[1];
	*block = B_DIRT;
}

void E_PLAYER_TICK(Entity* this) {
	Camera3D* cam = this->var[0];
	int* block = this->var[1];

	Vector3 vel = {0, 0, 0};
	float speed = 0.1;

	Vector3 lookdir = Vector3Normalize(Vector3Subtract(cam->target, cam->position));
	Vector2 movedir = Vector2Normalize((Vector2){lookdir.x, lookdir.z});

	vel = move(movedir);

	if (input.crouch) vel.y -= 1;
	if (input.jump)   vel.y += 1;

	if (input.hit) {
		Vector3 v = get_look_block(*cam);
		if (v.x != 0 || v.y != 0 || v.z != 0)
			set_block(v.x, v.y, v.z, B_AIR);
	}
	if (input.use) {
		place_block(*cam, *block);
	}

	vel.x *= speed;
	vel.y *= speed;
	vel.z *= speed;

	cam->position = Vector3Add(cam->position, vel);
	cam->target = Vector3Add(cam->target, vel);

	if (input.nextslot && *block < B_STONE) *block += 1;
	if (input.prevslot && *block > B_DIRT)  *block -= 1;

	if (input.inventory) {
		if (IsCursorHidden())
			EnableCursor();
		else
			DisableCursor();
	}

	if (input.save)
		save_world("world.wwf");
}

void __attribute__((constructor)) _construct_player() {
	ENTITY_INIT[E_PLAYER] = E_PLAYER_INIT;
	ENTITY_TICK[E_PLAYER] = E_PLAYER_TICK;
}

Vector3 move(Vector2 dir) {
	Vector3 vel = {0, 0, 0};
	if (input.forward) {
		vel.x += dir.x;
		vel.z += dir.y;
	}
	if (input.backward) {
		vel.x -= dir.x;
		vel.z -= dir.y;
	}
	if (input.left) {
		vel.x += dir.y;
		vel.z -= dir.x;
	}
	if (input.right) {
		vel.x -= dir.y;
		vel.z += dir.x;
	}
	return Vector3Normalize(vel);
}

Vector3 get_look_block(Camera3D cam) {
	Vector3 dir = Vector3Normalize(Vector3Subtract(cam.target, cam.position));
	for (float i = 0; i < 5; i += 0.1) {
		int x = roundf(cam.position.x + dir.x * i);
		int y = roundf(cam.position.y + dir.y * i);
		int z = roundf(cam.position.z + dir.z * i);
		if (get_block(x, y, z) > B_AIR)
			return (Vector3){x, y, z};
	}
	return (Vector3){0, 0, 0};
}

void place_block(Camera3D cam, int block) {
	Vector3 dir = Vector3Normalize(Vector3Subtract(cam.target, cam.position));
	for (float i = 0; i < 5; i += 0.1) {
		int x = roundf(cam.position.x + dir.x * i);
		int y = roundf(cam.position.y + dir.y * i);
		int z = roundf(cam.position.z + dir.z * i);
		if (get_block(x, y, z) > B_AIR) {
			x = roundf(cam.position.x + dir.x * (i - 0.1));
			y = roundf(cam.position.y + dir.y * (i - 0.1));
			z = roundf(cam.position.z + dir.z * (i - 0.1));
			set_block(x, y, z, block);
			return;
		}
	}
}
