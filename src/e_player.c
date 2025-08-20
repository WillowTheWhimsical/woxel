#include "entity.h"
#include "client.h"
#include "input.h"
#include "world.h"
#include "blocks.h"
#include "menu.h"
#include "sound.h"
#include "music.h"

#include <enet/enet.h>
#include <malloc.h>
#include <raylib.h>
#include <raymath.h>

Vector2 get_movedir(Camera3D);
Vector3 get_look_block(Camera3D);
void place_block(Camera3D cam, int block);

void E_PLAYER_INIT(Entity* this) {
	this->tex = -1;
	this->data = malloc(sizeof(Camera3D) + sizeof(int) + sizeof(int) + sizeof(Menu) + sizeof(Menu));
	this->var = malloc(sizeof(void*) * 5);
	this->var[0] = this->data;
	this->var[1] = this->data + sizeof(Camera3D);
	this->var[2] = this->data + sizeof(Camera3D) + sizeof(int);
	this->var[3] = this->data + sizeof(Camera3D) + sizeof(int) + sizeof(int);
	this->var[4] = this->data + sizeof(Camera3D) + sizeof(int) + sizeof(int) + sizeof(Menu);

	Camera3D* cam = this->var[0];
	cam->projection = CAMERA_PERSPECTIVE;
	cam->position = (Vector3){0, 0, 0};
	cam->target = (Vector3){0, 0, -1};
	cam->up = (Vector3){0, 1, 0};
	cam->fovy = 90;

	int* block = this->var[1];
	*block = B_DIRT;

	int* in_menu = this->var[2];
	*in_menu = 0;

	Menu* inventory_menu = this->var[3];
	*inventory_menu = spawn_menu(BLOCK_TYPES, "Inventory");
	for (int i = 0; i < BLOCK_TYPES; i++)
		set_menu_option(inventory_menu, i, block_name[i]);
	update_menu(inventory_menu);

	Menu* pause_menu = this->var[4];
	*pause_menu = spawn_menu(6, "Pause");
	set_menu_option(pause_menu, 0, "Resume");
	set_menu_option(pause_menu, 1, "Save World");
	set_menu_option(pause_menu, 2, "Load World");
	set_menu_option(pause_menu, 3, "Toggle Music");
	set_menu_option(pause_menu, 4, "Fullscreen");
	set_menu_option(pause_menu, 5, "Exit");
	update_menu(pause_menu);

	this->pos_x = world.w * 0.5;
	this->pos_z = world.l * 0.5;
	this->pos_y = 13;

	this->vel_x = this->vel_y = this->vel_z = 0;

	this->size_x = this->size_z = 0.2;
	this->size_y = 1.8;

	cam->position = Vector3Add(cam->position, vec3(this->pos));
	cam->position.y += 1.5;
	cam->target = Vector3Add(cam->target, cam->position);
}

void E_PLAYER_TICK(Entity* this) {
	static bool flying;
	static bool grounded;
	static int step_sound;
	static float step_sound_timer;
	static bool fullscreen;
	static bool mute_music;

	const float speed = 0.1;
	const float jump_speed = 0.17;
	const float gravity = 0.01;
	const float terminal_vel = -1;
	const float step_sound_delay = 2.3;

	Camera3D* cam = this->var[0];
	int* block = this->var[1];
	int* in_menu = this->var[2];
	Menu* inventory_menu = this->var[3];
	Menu* pause_menu = this->var[4];

	Vector2 dir = get_movedir(*cam);
	this->vel_x = dir.x * speed;
	this->vel_z = dir.y * speed;

	if (!flying) {
		if (input.crouch) {
			this->vel_x *= 0.5;
			this->vel_z *= 0.5;
		}

		if (this->vel_y > terminal_vel)
			this->vel_y -= gravity;

		if (get_block(this->pos_x - this->size_x, this->pos_y + this->vel_y, this->pos_z - this->size_z) > B_AIR
		 || get_block(this->pos_x - this->size_x, this->pos_y + this->vel_y, this->pos_z + this->size_z) > B_AIR
		 || get_block(this->pos_x + this->size_x, this->pos_y + this->vel_y, this->pos_z - this->size_z) > B_AIR
		 || get_block(this->pos_x + this->size_x, this->pos_y + this->vel_y, this->pos_z + this->size_z) > B_AIR)
			grounded = true;

		entity_collision(this);

		if (input.jump && grounded) {
			this->vel_y = jump_speed;
			grounded = false;
			PlaySound(sound[S_STEP0 + GetRandomValue(0, 3)]);
		}

		if (input.crouch && grounded) {
			if (get_block(this->pos_x + this->vel_x - this->size_x, this->pos_y - 1, this->pos_z - this->size_z) == B_AIR
			 && get_block(this->pos_x + this->vel_x - this->size_x, this->pos_y - 1, this->pos_z + this->size_z) == B_AIR
			 && get_block(this->pos_x + this->vel_x + this->size_x, this->pos_y - 1, this->pos_z - this->size_z) == B_AIR
			 && get_block(this->pos_x + this->vel_x + this->size_x, this->pos_y - 1, this->pos_z + this->size_z) == B_AIR)
				this->vel_x = 0;

			if (get_block(this->pos_x - this->size_x, this->pos_y - 1, this->pos_z + this->vel_z - this->size_z) == B_AIR
			 && get_block(this->pos_x - this->size_x, this->pos_y - 1, this->pos_z + this->vel_z + this->size_z) == B_AIR
			 && get_block(this->pos_x + this->size_x, this->pos_y - 1, this->pos_z + this->vel_z - this->size_z) == B_AIR
			 && get_block(this->pos_x + this->size_x, this->pos_y - 1, this->pos_z + this->vel_z + this->size_z) == B_AIR)
				this->vel_z = 0;
		}

		float actual_speed = fabsf(this->vel_x) + fabsf(this->vel_z);
		if (grounded && actual_speed > 0) {
			if (!IsSoundPlaying(sound[S_STEP0 + step_sound])) {
				if (step_sound_timer >= step_sound_delay) {
					step_sound = GetRandomValue(0, 3);
					PlaySound(sound[S_STEP0 + step_sound]);
					step_sound_timer = 0;
				}
			}
			step_sound_timer += actual_speed;
		}
		else
			step_sound_timer = step_sound_delay;
	}
	else {
		if (input.jump) {
			input.jump = false;
			this->vel_y = speed;
		}
		else if (input.crouch) {
			input.crouch = false;
			this->vel_y = -speed;
		}
		else
			this->vel_y = 0;
	}

	if (!*in_menu) {
		if (input.hit) {
			input.hit = false;
			Vector3 v = get_look_block(*cam);
			if (v.x != 0 || v.y != 0 || v.z != 0) {
				set_block(v.x, v.y, v.z, B_AIR);
				PlaySound(sound[S_TICK]);
				client_send(TextFormat("set %d %d %d %d", (int)v.x, (int)v.y, (int)v.z, B_AIR));
			}
		}
		if (input.use) {
			input.use = false;
			place_block(*cam, *block);
		}
	}
	else {
		switch (*in_menu) {
			case 1:;
				menu_tick(inventory_menu);
				for (int i = 0; i < inventory_menu->options; i++) {
					if (inventory_menu->option[i]) {
						inventory_menu->option[i] = false;
						*block = i;
						break;
					}
				}
				break;
			case 2:
				menu_tick(pause_menu);
				if (pause_menu->option[0]) {
					pause_menu->option[0] = false;
					*in_menu = 0;
				}
				else if (pause_menu->option[1]) {
					pause_menu->option[1] = false;
					save_world("world.wwf");
					PlaySound(sound[S_DENY]);
				}
				else if (pause_menu->option[2]) {
					pause_menu->option[2] = false;
					destroy_world();
					load_world("world.wwf");
					PlaySound(sound[S_DENY]);
				}
				else if (pause_menu->option[3]) {
					pause_menu->option[3] = false;
					mute_music = !mute_music;
					SetMusicVolume(music, !mute_music);
				}
				else if (pause_menu->option[4]) {
					pause_menu->option[4] = false;
					fullscreen = !fullscreen;
					if (fullscreen) {
						SetWindowSize(GetMonitorWidth(GetCurrentMonitor()), GetMonitorHeight(GetCurrentMonitor()));
						ToggleFullscreen();
					}
					else {
						ToggleFullscreen();
						SetWindowSize(800, 600);
					}
					update_menu(pause_menu);
					update_menu(inventory_menu);
				}
				else if (pause_menu->option[5]) {
					pause_menu->option[5] = false;
					PlaySound(sound[S_BUTTON]);
					if (GetRandomValue(1, 69) == 1)
						PlaySound(sound[S_FIDDLESTICKS]);
				}
				break;
		}
	}

	set_vec3(this->pos, Vector3Add(vec3(this->pos), vec3(this->vel)));
	cam->position = Vector3Add(cam->position, vec3(this->vel));
	cam->target = Vector3Add(cam->target, vec3(this->vel));

	if (input.inventory || input.pause) {
		if (*in_menu) *in_menu = 0;
		else if (input.inventory) *in_menu = 1;
		else *in_menu = 2;
		input.inventory = input.pause = false;
		PlaySound(sound[S_MOVESELECT]);
	}

	if (input.fly) {
		input.fly = false;
		flying = !flying;
		PlaySound(sound[S_DENY]);
	}

	client_send(TextFormat("pos %f %f %f %d", this->pos_x, this->pos_y, this->pos_z, client_getid()));

	Vector2 lookdir = Vector2Normalize((Vector2){cam->target.x - cam->position.x, cam->target.z - cam->position.z});
	client_send(TextFormat("dir %f %f %d", lookdir.x, lookdir.y, client_getid()));
}

void __attribute__((constructor)) _construct_player() {
	ENTITY_INIT[E_PLAYER] = E_PLAYER_INIT;
	ENTITY_TICK[E_PLAYER] = E_PLAYER_TICK;
}

Vector2 get_movedir(Camera cam) {
	// bug: somehow walking between cardinal directions is ~1.4 times faster
	Vector3 lookdir = Vector3Normalize(Vector3Subtract(cam.target, cam.position));
	Vector2 movedir = Vector2Normalize((Vector2){lookdir.x, lookdir.z});
	Vector2 dir = {0, 0};
	if (input.forward) {
		dir.x += movedir.x;
		dir.y += movedir.y;
	}
	if (input.backward) {
		dir.x -= movedir.x;
		dir.y -= movedir.y;
	}
	if (input.left) {
		dir.x += movedir.y;
		dir.y -= movedir.x;
	}
	if (input.right) {
		dir.x -= movedir.y;
		dir.y += movedir.x;
	}
	return Vector2Normalize(dir);
}

Vector3 get_look_block(Camera3D cam) {
	Vector3 dir = Vector3Normalize(Vector3Subtract(cam.target, cam.position));
	for (float i = 0; i < 5; i += 0.1) {
		int x = cam.position.x + dir.x * i;
		int y = cam.position.y + dir.y * i;
		int z = cam.position.z + dir.z * i;
		if (get_block(x, y, z) > B_AIR)
			return (Vector3){x, y, z};
	}
	return (Vector3){0, 0, 0};
}

void place_block(Camera3D cam, int block) {
	Vector3 dir = Vector3Normalize(Vector3Subtract(cam.target, cam.position));
	for (float i = 0; i < 5; i += 0.1) {
		int x = cam.position.x + dir.x * i;
		int y = cam.position.y + dir.y * i;
		int z = cam.position.z + dir.z * i;
		if (get_block(x, y, z) > B_AIR) {
			x = cam.position.x + dir.x * (i - 0.1);
			y = cam.position.y + dir.y * (i - 0.1);
			z = cam.position.z + dir.z * (i - 0.1);
			set_block(x, y, z, block);
			PlaySound(sound[S_TICK]);
			client_send(TextFormat("set %d %d %d %d", x, y, z, block));
			return;
		}
	}
}
