#ifndef INPUT_H
#define INPUT_H

#include "stdbool.h"

struct Input {
	int key_forward, key_backward, key_left, key_right, key_jump, key_crouch, key_hit, key_use, key_inventory;
	int forward, backward, left, right, jump, crouch, hit, use, inventory;
	bool hit_bool, use_bool, inventory_bool;
	float mdx, mdy, sensitivity;
};
extern struct Input input;

void set_binds();
void get_input();
void input_tick();

#endif
