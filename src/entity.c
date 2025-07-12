#include "entity.h"
#include "world.h"
#include "blocks.h"

#include <malloc.h>
#include <stdlib.h>
#include <stdbool.h>

int entities = 0;
Entity* entity;
void (*ENTITY_INIT[ENTITY_TYPES])(Entity* self);
void (*ENTITY_TICK[ENTITY_TYPES])(Entity* self);

void uninit_entities() {
	free(entity);
}

Entity* spawn_entity(int type) {
	if (entities > 0)
		entity = malloc(sizeof(Entity));
	else
		entity = realloc(entity, sizeof(Entity) * (entities + 1));
	
	Entity* new_entity = &entity[entities];

	int id;
	for (bool done = true; !done;) {
		id = rand();
		for_entity(e)
			if (id == e->id) {
				done = false;
				break;
			}
		}
	}

	new_entity->type = type;

	entity_init(new_entity);

	entities++;
	return new_entity;
}

void smite_entity(int id) {

}

void entity_collision(Entity* this) {
	Vector3 vel = this->vel;
	for (int i = 0; i <= (int)this->size.y; i++) {
		if (
			(vel.x < 0 && (
			    get_block(this->pos.x + vel.x - this->size.x, this->pos.y + this->size.y - i, this->pos.z - this->size.z) > B_AIR
			 || get_block(this->pos.x + vel.x - this->size.x, this->pos.y + this->size.y - i, this->pos.z + this->size.z) > B_AIR
			)) || (vel.x > 0 && (
			    get_block(this->pos.x + vel.x + this->size.x, this->pos.y + this->size.y - i, this->pos.z - this->size.z) > B_AIR
			 || get_block(this->pos.x + vel.x + this->size.x, this->pos.y + this->size.y - i, this->pos.z + this->size.z) > B_AIR
		  ))
		) { this->vel.x = 0; break; }
	}
	if (
		(vel.x < 0 && (
		    get_block(this->pos.x + vel.x - this->size.x, this->pos.y, this->pos.z - this->size.z) > B_AIR
		 || get_block(this->pos.x + vel.x - this->size.x, this->pos.y, this->pos.z + this->size.z) > B_AIR
		)) || (vel.x > 0 && (
		    get_block(this->pos.x + vel.x + this->size.x, this->pos.y, this->pos.z - this->size.z) > B_AIR
		 || get_block(this->pos.x + vel.x + this->size.x, this->pos.y, this->pos.z + this->size.z) > B_AIR
	  ))
	) { this->vel.x = 0; }

	for (int i = 0; i <= (int)this->size.y; i++) {
		if (
			(vel.z < 0 && (
			    get_block(this->pos.x - this->size.x, this->pos.y + this->size.y - i, this->pos.z - this->size.z + vel.z) > B_AIR
			 || get_block(this->pos.x + this->size.x, this->pos.y + this->size.y - i, this->pos.z - this->size.z + vel.z) > B_AIR
			)) || (vel.z > 0 && (
			    get_block(this->pos.x - this->size.x, this->pos.y + this->size.y - i, this->pos.z + this->size.z + vel.z) > B_AIR
			 || get_block(this->pos.x + this->size.x, this->pos.y + this->size.y - i, this->pos.z + this->size.z + vel.z) > B_AIR
			))
		) { this->vel.z = 0; }
	}
	if (
		(vel.z < 0 && (
		    get_block(this->pos.x - this->size.x, this->pos.y, this->pos.z - this->size.z + vel.z) > B_AIR
		 || get_block(this->pos.x + this->size.x, this->pos.y, this->pos.z - this->size.z + vel.z) > B_AIR
		)) || (vel.z > 0 && (
		    get_block(this->pos.x - this->size.x, this->pos.y, this->pos.z + this->size.z + vel.z) > B_AIR
		 || get_block(this->pos.x + this->size.x, this->pos.y, this->pos.z + this->size.z + vel.z) > B_AIR
		))
	) { this->vel.z = 0; }

	if (
		(vel.y < 0 && (
		    get_block(this->pos.x - this->size.x, this->pos.y + vel.y, this->pos.z - this->size.z) > B_AIR
		 || get_block(this->pos.x - this->size.x, this->pos.y + vel.y, this->pos.z + this->size.z) > B_AIR
		 || get_block(this->pos.x + this->size.x, this->pos.y + vel.y, this->pos.z - this->size.z) > B_AIR
		 || get_block(this->pos.x + this->size.x, this->pos.y + vel.y, this->pos.z + this->size.z) > B_AIR
		)) || (vel.y > 0 && (
		    get_block(this->pos.x - this->size.x, this->pos.y + vel.y + this->size.y, this->pos.z - this->size.z) > B_AIR
		 || get_block(this->pos.x - this->size.x, this->pos.y + vel.y + this->size.y, this->pos.z + this->size.z) > B_AIR
		 || get_block(this->pos.x + this->size.x, this->pos.y + vel.y + this->size.y, this->pos.z - this->size.z) > B_AIR
		 || get_block(this->pos.x + this->size.x, this->pos.y + vel.y + this->size.y, this->pos.z + this->size.z) > B_AIR
		))
	) { this->vel.y = 0; }
}
