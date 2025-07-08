#include "entity.h"

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

Entity* SpawnEntity(int type) {
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

void KillEntity(int id) {

}
