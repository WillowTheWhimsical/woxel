#include "entity.h"
#include "texture.h"

#include <raymath.h>
#include <malloc.h>

void E_GOOBERT_INIT(Entity* this) {
	this->tex = T_GOOBERT;
	this->vel = (Vector3){0, 0, 0};
	this->size = (Vector3){0.45, 0.45, 0.45};
	this->frame_size = (Vector2){16, 16};
	this->frame = 0;
	this->dir = (Vector2){0, 0};
}

void E_GOOBERT_TICK(Entity* this) {
	const float gravity = 0.01;
	const float terminal_vel = -1;
	const float speed = 0.05;

	if (this->vel.y > terminal_vel)
		this->vel.y -= gravity;

	this->vel.x = this->dir.x * speed;
	this->vel.z = this->dir.y * speed;

	entity_collision(this);

	if (this->vel.x == 0 || this->vel.z == 0) {
		this->dir.x = GetRandomValue(-10, 10) * 0.1;
		this->dir.y = GetRandomValue(-10, 10) * 0.1;
		this->dir = Vector2Normalize(this->dir);
	}

	this->pos = Vector3Add(this->pos, this->vel);
}

void __attribute__((constructor)) _construct_goobert() {
	ENTITY_INIT[E_GOOBERT] = E_GOOBERT_INIT;
	ENTITY_TICK[E_GOOBERT] = E_GOOBERT_TICK;
}
