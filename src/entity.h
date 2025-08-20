#ifndef ENTITY_H
#define ENTITY_H

#include "entities.h"

#define for_entities(e) for (int i = 0; i < entities; i++) { Entity* e = &entity[i];
#define entity_init(e) ENTITY_INIT[e->type](e);
#define entity_tick(e) ENTITY_TICK[e->type](e);

#define vec2(v) ((Vector2){v##_x, v##_y})
#define vec3(v) ((Vector3){v##_x, v##_y, v##_z})

#define set_vec2(v0, v1) v0##_x = v1.x; v0##_y = v1.y;
#define set_vec3(v0, v1) v0##_x = v1.x; v0##_y = v1.y; v0##_z = v1.z;

typedef struct Entity {
	int id, type, tex, frame, *frames, anims;
	float pos_x, pos_y, pos_z,
	      vel_x, vel_y, vel_z,
	      size_x, size_y, size_z,
	      dir_x, dir_y,
	      frame_size_x, frame_size_y;
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
