#ifndef ENTITY_H
#define ENTITY_H

#include "entities.h"

#define for_entity(e) for (int i = 0; i < entities; i++) { Entity* e = &entity[i];
#define entity_init(entity) ENTITY_INIT[entity->type](entity);
#define entity_tick(entity) ENTITY_TICK[entity->type](entity);

typedef struct Entity {
	int id;
	int type;
	void* data;
	void** var;
} Entity;

void uninit_entities();

Entity* SpawnEntity(int type);
void KillEntity(int id);

extern int entities;
extern Entity* entity;
extern void (*ENTITY_INIT[ENTITY_TYPES])(Entity* self);
extern void (*ENTITY_TICK[ENTITY_TYPES])(Entity* self);

#endif
