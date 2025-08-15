#include "entity.h"
#include "texture.h"

#include <malloc.h>

void E_PUPPET_INIT(Entity* this) {
	this->tex = T_GOOBERT;
	this->vel = (Vector3){0, 0, 0};
	this->size = (Vector3){0.45, 0.45, 0.45};
	this->frame_size = (Vector2){16, 16};
	this->frame = 0;
	this->dir = (Vector2){0, 0};

	this->data = malloc(sizeof(unsigned int));
	this->var = malloc(sizeof(void*) * 1);
	this->var[0] = this->data;
}

void E_PUPPET_TICK(Entity* this) {
	int* id = this->var[0];

	static int timer = 0;

	if (timer >= 60) {
		printf("%d (%f, %f, %f)\n", *id, this->pos.x, this->pos.y, this->pos.z);
		timer = 0;
	}

	timer++;
}

void __attribute__((constructor)) _construct_puppet() {
	ENTITY_INIT[E_PUPPET] = E_PUPPET_INIT;
	ENTITY_TICK[E_PUPPET] = E_PUPPET_TICK;
}
