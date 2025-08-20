#include "entity.h"
#include "world.h"

#include <raylib.h>
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
	if (entities == 0)
		entity = malloc(sizeof(Entity));
	else
		entity = realloc(entity, sizeof(Entity) * (entities + 1));

	Entity* new_entity = &entity[entities];

	int id;
	for (bool done = true; !done;) {
		id = rand();
		for_entities(e)
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
	Vector3 vel = (Vector3){this->vel_x, this->vel_y, this->vel_z};
	for (int i = 0; i <= (int)this->size_y; i++) {
		if (
			(vel.x < 0 && (
			    get_block(this->pos_x + vel.x - this->size_x, this->pos_y + this->size_y - i, this->pos_z - this->size_z) >= 0
			 || get_block(this->pos_x + vel.x - this->size_x, this->pos_y + this->size_y - i, this->pos_z + this->size_z) >= 0
			)) || (vel.x > 0 && (
			    get_block(this->pos_x + vel.x + this->size_x, this->pos_y + this->size_y - i, this->pos_z - this->size_z) >= 0
			 || get_block(this->pos_x + vel.x + this->size_x, this->pos_y + this->size_y - i, this->pos_z + this->size_z) >= 0
		  ))
		) { this->vel_x = 0; break; }
	}
	if (
		(vel.x < 0 && (
		    get_block(this->pos_x + vel.x - this->size_x, this->pos_y, this->pos_z - this->size_z) >= 0
		 || get_block(this->pos_x + vel.x - this->size_x, this->pos_y, this->pos_z + this->size_z) >= 0
		)) || (vel.x > 0 && (
		    get_block(this->pos_x + vel.x + this->size_x, this->pos_y, this->pos_z - this->size_z) >= 0
		 || get_block(this->pos_x + vel.x + this->size_x, this->pos_y, this->pos_z + this->size_z) >= 0
	  ))
	) { this->vel_x = 0; }

	for (int i = 0; i <= (int)this->size_y; i++) {
		if (
			(vel.z < 0 && (
			    get_block(this->pos_x - this->size_x, this->pos_y + this->size_y - i, this->pos_z - this->size_z + vel.z) >= 0
			 || get_block(this->pos_x + this->size_x, this->pos_y + this->size_y - i, this->pos_z - this->size_z + vel.z) >= 0
			)) || (vel.z > 0 && (
			    get_block(this->pos_x - this->size_x, this->pos_y + this->size_y - i, this->pos_z + this->size_z + vel.z) >= 0
			 || get_block(this->pos_x + this->size_x, this->pos_y + this->size_y - i, this->pos_z + this->size_z + vel.z) >= 0
			))
		) { this->vel_z = 0; }
	}
	if (
		(vel.z < 0 && (
		    get_block(this->pos_x - this->size_x, this->pos_y, this->pos_z - this->size_z + vel.z) >= 0
		 || get_block(this->pos_x + this->size_x, this->pos_y, this->pos_z - this->size_z + vel.z) >= 0
		)) || (vel.z > 0 && (
		    get_block(this->pos_x - this->size_x, this->pos_y, this->pos_z + this->size_z + vel.z) >= 0
		 || get_block(this->pos_x + this->size_x, this->pos_y, this->pos_z + this->size_z + vel.z) >= 0
		))
	) { this->vel_z = 0; }

	if (
		(vel.y < 0 && (
		    get_block(this->pos_x - this->size_x, this->pos_y + vel.y, this->pos_z - this->size_z) >= 0
		 || get_block(this->pos_x - this->size_x, this->pos_y + vel.y, this->pos_z + this->size_z) >= 0
		 || get_block(this->pos_x + this->size_x, this->pos_y + vel.y, this->pos_z - this->size_z) >= 0
		 || get_block(this->pos_x + this->size_x, this->pos_y + vel.y, this->pos_z + this->size_z) >= 0
		)) || (vel.y > 0 && (
		    get_block(this->pos_x - this->size_x, this->pos_y + vel.y + this->size_y, this->pos_z - this->size_z) >= 0
		 || get_block(this->pos_x - this->size_x, this->pos_y + vel.y + this->size_y, this->pos_z + this->size_z) >= 0
		 || get_block(this->pos_x + this->size_x, this->pos_y + vel.y + this->size_y, this->pos_z - this->size_z) >= 0
		 || get_block(this->pos_x + this->size_x, this->pos_y + vel.y + this->size_y, this->pos_z + this->size_z) >= 0
		))
	) { this->vel_y = 0; }
}
