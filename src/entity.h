#ifndef ENTITY_H
#define ENTITY_H

#include "entities.h"

#include <raylib.h>

#define for_entities(e) for (int i = 0; i < entities; i++) { Entity* e = &entity[i];
#define entity_init(e) ENTITY_INIT[e->type](e);
#define entity_tick(e) ENTITY_TICK[e->type](e);

typedef struct Entity {
	int id, type, tex, frame, *frames, anims;
	Vector3 pos, vel, size;
	Vector2 dir, frame_size;
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
