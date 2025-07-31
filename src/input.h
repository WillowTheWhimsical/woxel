#ifndef INPUT_H
#define INPUT_H

#include "stdbool.h"

struct Input {
	int key_forward, key_backward, key_left, key_right, key_jump, key_crouch, key_hit, key_use, key_inventory, key_nextslot, key_prevslot, key_fly, key_mconfirm, key_mup, key_mdown;
	int forward, backward, left, right, jump, crouch, hit, use, inventory, nextslot, prevslot, fly, mconfirm, mup, mdown;
	bool hit_bool, use_bool, inventory_bool, nextslot_bool, prevslot_bool, fly_bool, mup_bool, mdown_bool;
	float mdx, mdy, sensitivity;
};
extern struct Input input;

void set_binds();
void get_input();
void input_tick();

#endif
