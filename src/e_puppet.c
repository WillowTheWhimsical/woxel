#include "entity.h"
#include "texture.h"

#include <malloc.h>

void E_PUPPET_INIT(Entity* this) {
	this->tex = T_GOOBERT;
	this->vel_x = this->vel_y = this->vel_z = 0;
	this->size_x = this->size_y = this->size_z = 0.45;
	this->frame_size_x = this->frame_size_y = 16;
	this->frame = 0;
	this->dir_x = this->dir_y = 0;

	this->data = malloc(sizeof(unsigned int));
	this->var = malloc(sizeof(void*) * 1);
	this->var[0] = this->data;
}

void E_PUPPET_TICK(Entity* this) {

}

void __attribute__((constructor)) _construct_puppet() {
	ENTITY_INIT[E_PUPPET] = E_PUPPET_INIT;
	ENTITY_TICK[E_PUPPET] = E_PUPPET_TICK;
}
