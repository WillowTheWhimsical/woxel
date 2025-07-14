#include "entity.h"
#include "input.h"
#include "world.h"
#include "blocks.h"

#include <malloc.h>
#include <raymath.h>

Vector2 get_movedir(Camera3D);
Vector3 get_look_block(Camera3D);
void place_block(Camera3D cam, int block);

void E_PLAYER_INIT(Entity* this) {
	this->data = malloc(sizeof(Camera3D) + sizeof(int));
	this->var = malloc(sizeof(void*) * 2);
	this->var[0] = this->data;
	this->var[1] = this->data + sizeof(Camera3D);

	Camera3D* cam = this->var[0];
	cam->projection = CAMERA_PERSPECTIVE;
	cam->position = (Vector3){0, 0, 0};
	cam->target = (Vector3){0, 0, -1};
	cam->up = (Vector3){0, 1, 0};
	cam->fovy = 90;

	int* block = this->var[1];
	*block = B_DIRT;

	this->pos.x = world.w * 0.5;
	this->pos.z = world.l * 0.5;
	this->pos.y = 13;

	this->vel.x = this->vel.y = this->vel.z = 0;

	this->size.x = this->size.z = 0.2;
	this->size.y = 1.8;

	cam->position = Vector3Add(cam->position, this->pos);
	cam->position.y += 1.5;
	cam->target = Vector3Add(cam->target, cam->position);
}

void E_PLAYER_TICK(Entity* this) {
	static bool flying;
	static bool grounded;

	const float speed = 0.1;
	const float gravity = 0.01;
	const float terminal_vel = -1;

	Camera3D* cam = this->var[0];
	int* block = this->var[1];

	Vector2 dir = get_movedir(*cam);
	this->vel.x = dir.x * speed;
	this->vel.z = dir.y * speed;

	if (!flying) {
		if (this->vel.y > terminal_vel)
			this->vel.y -= gravity;

		if (get_block(this->pos.x - this->size.x, this->pos.y + this->vel.y, this->pos.z - this->size.z) > B_AIR
		 || get_block(this->pos.x - this->size.x, this->pos.y + this->vel.y, this->pos.z + this->size.z) > B_AIR
		 || get_block(this->pos.x + this->size.x, this->pos.y + this->vel.y, this->pos.z - this->size.z) > B_AIR
		 || get_block(this->pos.x + this->size.x, this->pos.y + this->vel.y, this->pos.z + this->size.z) > B_AIR)
			grounded = true;

		entity_collision(this);

		if (input.jump && grounded) {
			this->vel.y = 0.17;
			grounded = false;
		}

		if (input.crouch && grounded) {
			if (get_block(this->pos.x + this->vel.x - this->size.x, this->pos.y - 1, this->pos.z - this->size.z) == B_AIR
			 && get_block(this->pos.x + this->vel.x - this->size.x, this->pos.y - 1, this->pos.z + this->size.z) == B_AIR
			 && get_block(this->pos.x + this->vel.x + this->size.x, this->pos.y - 1, this->pos.z - this->size.z) == B_AIR
			 && get_block(this->pos.x + this->vel.x + this->size.x, this->pos.y - 1, this->pos.z + this->size.z) == B_AIR)
				this->vel.x = 0;

			if (get_block(this->pos.x - this->size.x, this->pos.y - 1, this->pos.z + this->vel.z - this->size.z) == B_AIR
			 && get_block(this->pos.x - this->size.x, this->pos.y - 1, this->pos.z + this->vel.z + this->size.z) == B_AIR
			 && get_block(this->pos.x + this->size.x, this->pos.y - 1, this->pos.z + this->vel.z - this->size.z) == B_AIR
			 && get_block(this->pos.x + this->size.x, this->pos.y - 1, this->pos.z + this->vel.z + this->size.z) == B_AIR)
				this->vel.z = 0;
		}
	}
	else {
		if (input.jump)
			this->vel.y = speed;
		else if (input.crouch)
			this->vel.y = -speed;
		else
			this->vel.y = 0;
	}

	if (input.hit) {
		Vector3 v = get_look_block(*cam);
		if (v.x != 0 || v.y != 0 || v.z != 0)
			set_block(v.x, v.y, v.z, B_AIR);
	}
	if (input.use) {
		place_block(*cam, *block);
	}

	this->pos = Vector3Add(this->pos, this->vel);
	cam->position = Vector3Add(cam->position, this->vel);
	cam->target = Vector3Add(cam->target, this->vel);

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
