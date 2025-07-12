#ifndef ENTITY_H
#define ENTITY_H

#include "entities.h"

#include <raylib.h>

#define for_entities(e) for (int i = 0; i < entities; i++) { Entity* e = &entity[i];
#define entity_init(entity) ENTITY_INIT[entity->type](entity);
#define entity_tick(entity) ENTITY_TICK[entity->type](entity);

typedef struct Entity {
	int id;
	int type;
	Vector3 pos, vel, size;
	void* data;
	void** var;
} Entity;

void uninit_entities();

Entity* spawn_entity(int type);
void smite_entity(int id);

void entity_collision(Entity* self);

extern int entities;
extern Entity* entity;
extern void (*ENTITY_INIT[ENTITY_TYPES])(Entity* self);
extern void (*ENTITY_TICK[ENTITY_TYPES])(Entity* self);

#endif
