#include "entity.h"
#include "texture.h"

#include <raymath.h>
#include <malloc.h>

void E_GOOBERT_INIT(Entity* this) {
	this->tex = T_GOOBERT;
	this->vel_x = this->vel_y = this->vel_z = 0;
	this->size_x = this->size_y = this->size_z = 0.45;
	this->frame_size_x = this->frame_size_y = 16;
	this->frame = 0;
	this->dir_x = this->dir_y = 0;
}

void E_GOOBERT_TICK(Entity* this) {
	const float gravity = 0.01;
	const float terminal_vel = -1;
	const float speed = 0.05;

	if (this->vel_y > terminal_vel)
		this->vel_y -= gravity;

	this->vel_x = this->dir_x * speed;
	this->vel_z = this->dir_y * speed;

	entity_collision(this);

	if (this->vel_x == 0 || this->vel_z == 0) {
		this->dir_x = GetRandomValue(-10, 10) * 0.1;
		this->dir_y = GetRandomValue(-10, 10) * 0.1;
		set_vec2(this->dir, Vector2Normalize(vec2(this->dir)));
	}

	set_vec3(this->pos, Vector3Add(vec3(this->pos), vec3(this->vel)));
}

void __attribute__((constructor)) _construct_goobert() {
	ENTITY_INIT[E_GOOBERT] = E_GOOBERT_INIT;
	ENTITY_TICK[E_GOOBERT] = E_GOOBERT_TICK;
}
