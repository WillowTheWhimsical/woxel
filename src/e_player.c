#include "entity.h"
#include "input.h"
#include "world.h"
#include "blocks.h"

#include <malloc.h>
#include <raylib.h>
#include <raymath.h>

Vector2 get_movedir(Camera3D);
Vector3 get_look_block(Camera3D);
void place_block(Camera3D cam, int block);

void E_PLAYER_INIT(Entity* this) {
	this->data = malloc(sizeof(Camera3D) + sizeof(Vector3) + sizeof(Vector3) + sizeof(Vector3) + sizeof(int));
	this->var = malloc(sizeof(void*) * 5);
	this->var[0] = this->data;
	this->var[1] = this->data + sizeof(Camera3D);
	this->var[2] = this->data + sizeof(Camera3D) + sizeof(Vector3);
	this->var[3] = this->data + sizeof(Camera3D) + sizeof(Vector3) + sizeof(Vector3);
	this->var[4] = this->data + sizeof(Camera3D) + sizeof(Vector3) + sizeof(Vector3) + sizeof(Vector3);

	Camera3D* cam = this->var[0];
	cam->projection = CAMERA_PERSPECTIVE;
	cam->position = (Vector3){0, 0, 0};
	cam->target = (Vector3){0, 0, -1};
	cam->up = (Vector3){0, 1, 0};
	cam->fovy = 90;

	Vector3* pos = this->var[1];
	pos->x = pos->z = 10;
	pos->y = 14;

	cam->position = Vector3Add(cam->position, *pos);
	cam->position.y += 1.5;
	cam->target = Vector3Add(cam->target, cam->position);

	Vector3* vel = this->var[2];
	vel->x = vel->y = vel->z = 0;

	Vector3* size = this->var[3];
	size->x = size->z = 0.2;
	size->y = 1.5;

	int* block = this->var[4];
	*block = B_DIRT;
}

void E_PLAYER_TICK(Entity* this) {
	static bool flying;
	static bool grounded;

	const float speed = 0.1;
	const float gravity = 0.01;
	const float terminal_vel = -1;

	Camera3D* cam = this->var[0];
	Vector3* pos = this->var[1];
	Vector3* vel = this->var[2];
	Vector3* size = this->var[3];
	int* block = this->var[4];

	Vector2 dir = get_movedir(*cam);
	vel->x = dir.x * speed;
	vel->z = dir.y * speed;

	if (!flying) {
		if (vel->y > terminal_vel)
			vel->y -= gravity;

		if (get_block(pos->x + vel->x - size->x, pos->y, pos->z - size->z) > B_AIR
		 || get_block(pos->x + vel->x - size->x, pos->y, pos->z + size->z) > B_AIR
		 || get_block(pos->x + vel->x + size->x, pos->y, pos->z - size->z) > B_AIR
		 || get_block(pos->x + vel->x + size->x, pos->y, pos->z + size->z) > B_AIR
		 || get_block(pos->x + vel->x - size->x, pos->y + size->y, pos->z - size->z) > B_AIR
		 || get_block(pos->x + vel->x - size->x, pos->y + size->y, pos->z + size->z) > B_AIR
		 || get_block(pos->x + vel->x + size->x, pos->y + size->y, pos->z - size->z) > B_AIR
		 || get_block(pos->x + vel->x + size->x, pos->y + size->y, pos->z + size->z) > B_AIR)
			vel->x = 0;

		if (get_block(pos->x - size->x, pos->y + vel->y, pos->z - size->z) > B_AIR
		 || get_block(pos->x - size->x, pos->y + vel->y, pos->z + size->z) > B_AIR
		 || get_block(pos->x + size->x, pos->y + vel->y, pos->z - size->z) > B_AIR
		 || get_block(pos->x + size->x, pos->y + vel->y, pos->z + size->z) > B_AIR
		 || get_block(pos->x - size->x, pos->y + vel->y + size->y, pos->z - size->z) > B_AIR
		 || get_block(pos->x - size->x, pos->y + vel->y + size->y, pos->z + size->z) > B_AIR
		 || get_block(pos->x + size->x, pos->y + vel->y + size->y, pos->z - size->z) > B_AIR
		 || get_block(pos->x + size->x, pos->y + vel->y + size->y, pos->z + size->z) > B_AIR) {
			vel->y = 0;
			grounded = true;
		}

		if (get_block(pos->x - size->x, pos->y, pos->z + vel->z - size->z) > B_AIR
		 || get_block(pos->x - size->x, pos->y, pos->z + vel->z + size->z) > B_AIR
		 || get_block(pos->x + size->x, pos->y, pos->z + vel->z - size->z) > B_AIR
		 || get_block(pos->x + size->x, pos->y, pos->z + vel->z + size->z) > B_AIR
		 || get_block(pos->x - size->x, pos->y + size->y, pos->z + vel->z - size->z) > B_AIR
		 || get_block(pos->x - size->x, pos->y + size->y, pos->z + vel->z + size->z) > B_AIR
		 || get_block(pos->x + size->x, pos->y + size->y, pos->z + vel->z - size->z) > B_AIR
		 || get_block(pos->x + size->x, pos->y + size->y, pos->z + vel->z + size->z) > B_AIR)
			vel->z = 0;

		if (input.jump && grounded) {
			vel->y = 0.17;
			grounded = false;
		}

		if (input.crouch && grounded) {
			if (get_block(pos->x + vel->x - size->x, pos->y - 1, pos->z - size->z) == B_AIR
			 && get_block(pos->x + vel->x - size->x, pos->y - 1, pos->z + size->z) == B_AIR
			 && get_block(pos->x + vel->x + size->x, pos->y - 1, pos->z - size->z) == B_AIR
			 && get_block(pos->x + vel->x + size->x, pos->y - 1, pos->z + size->z) == B_AIR)
				vel->x = 0;

			if (get_block(pos->x - size->x, pos->y - 1, pos->z + vel->z - size->z) == B_AIR
			 && get_block(pos->x - size->x, pos->y - 1, pos->z + vel->z + size->z) == B_AIR
			 && get_block(pos->x + size->x, pos->y - 1, pos->z + vel->z - size->z) == B_AIR
			 && get_block(pos->x + size->x, pos->y - 1, pos->z + vel->z + size->z) == B_AIR)
				vel->z = 0;
		}
	}
	else {
		if (input.jump)
			vel->y = speed;
		else if (input.crouch)
			vel->y = -speed;
		else
			vel->y = 0;
	}

	if (input.hit) {
		Vector3 v = get_look_block(*cam);
		if (v.x != 0 || v.y != 0 || v.z != 0)
			set_block(v.x, v.y, v.z, B_AIR);
	}
	if (input.use) {
		place_block(*cam, *block);
	}

	*pos = Vector3Add(*pos, *vel);
	cam->position = Vector3Add(cam->position, *vel);
	cam->target = Vector3Add(cam->target, *vel);

	if (input.nextslot && *block < B_STONE) *block += 1;
	if (input.prevslot && *block > B_DIRT)  *block -= 1;

	if (input.inventory) {
		if (IsCursorHidden())
			EnableCursor();
		else
			DisableCursor();
	}

	if (input.fly)
		flying = !flying;

	if (input.save)
		save_world("world.wwf");
}

void __attribute__((constructor)) _construct_player() {
	ENTITY_INIT[E_PLAYER] = E_PLAYER_INIT;
	ENTITY_TICK[E_PLAYER] = E_PLAYER_TICK;
}

Vector2 get_movedir(Camera cam) {
	Vector3 lookdir = Vector3Normalize(Vector3Subtract(cam.target, cam.position));
	Vector2 movedir = Vector2Normalize((Vector2){lookdir.x, lookdir.z});
	Vector2 dir = {0, 0};
	if (input.forward) {
		dir.x += movedir.x;
		dir.y += movedir.y;
	}
	if (input.backward) {
		dir.x -= movedir.x;
		dir.y -= movedir.y;
	}
	if (input.left) {
		dir.x += movedir.y;
		dir.y -= movedir.x;
	}
	if (input.right) {
		dir.x -= movedir.y;
		dir.y += movedir.x;
	}
	return Vector2Normalize(dir);
}

Vector3 get_look_block(Camera3D cam) {
	Vector3 dir = Vector3Normalize(Vector3Subtract(cam.target, cam.position));
	for (float i = 0; i < 5; i += 0.1) {
		int x = cam.position.x + dir.x * i;
		int y = cam.position.y + dir.y * i;
		int z = cam.position.z + dir.z * i;
		if (get_block(x, y, z) > B_AIR)
			return (Vector3){x, y, z};
	}
	return (Vector3){0, 0, 0};
}

void place_block(Camera3D cam, int block) {
	Vector3 dir = Vector3Normalize(Vector3Subtract(cam.target, cam.position));
	for (float i = 0; i < 5; i += 0.1) {
		int x = cam.position.x + dir.x * i;
		int y = cam.position.y + dir.y * i;
		int z = cam.position.z + dir.z * i;
		if (get_block(x, y, z) > B_AIR) {
			x = cam.position.x + dir.x * (i - 0.1);
			y = cam.position.y + dir.y * (i - 0.1);
			z = cam.position.z + dir.z * (i - 0.1);
			set_block(x, y, z, block);
			return;
		}
	}
}
