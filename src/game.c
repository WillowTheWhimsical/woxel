#include "game.h"
#include "client.h"
#include "input.h"
#include "tick.h"
#include "draw.h"
#include "texture.h"
#include "sound.h"
#include "music.h"
#include "world.h"
#include "entity.h"

#include <raylib.h>
#include <unistd.h>
#include <stdlib.h>
#include <string.h>

bool fullscreen = false;
float sensitivity = 0.25;
char path[256] = {'\0'};
Vector3 size = {32, 32, 32};
char* hostname;
int port = 42069;

void init() {
	client_join(hostname, port);

	SetTraceLogLevel(LOG_WARNING);
	
	SetConfigFlags(FLAG_MSAA_4X_HINT);

	if (fullscreen) {
		InitWindow(0, 0, "Willow's Whimsical World");
		ToggleFullscreen();
	}
	else
		InitWindow(800, 600, "Willow's Whimsical World");

	SetTargetFPS(GetMonitorRefreshRate(GetCurrentMonitor()));

	SetExitKey(KEY_NULL);

	set_binds();
	input.sensitivity = sensitivity;

	load_textures();

	InitAudioDevice();
	load_sounds();

	if (path[0] == '\0')
		generate_world(size.x, size.y, size.z);
	else
		load_world(path);

	spawn_entity(E_PLAYER);
	Entity* goobert = spawn_entity(E_GOOBERT);
	goobert->pos_x = world.w * 0.5;
	goobert->pos_y = 24;
	goobert->pos_z = world.h * 0.5;
}

int loop() {
	get_input();
	tick();
	render();
	music_update();
	client_update();

	return WindowShouldClose();
}

void uninit() {
	client_leave();
	CloseAudioDevice();
	destroy_world();
	unload_textures();
	unload_sounds();
	unload_music();
}

void process_args(int argc, char* argv[]) {
	hostname = malloc(sizeof(char) * (strlen("localhost") + 1));
	strcpy(hostname, "localhost");
	int opt;
	while ((opt = getopt(argc, argv, "fs:o:h:p:")) != -1) {
		switch (opt) {
			case 'f':
				fullscreen = true;
				break;
			case 's':
				sensitivity = atoi(optarg) / 100.f;
				break;
			case 'o':
				strncpy(path, optarg, 255);
				break;
			case 'h':
				hostname = realloc(hostname, sizeof(char) * (strlen(optarg) + 1));
				strcpy(hostname, optarg);
				break;
			case 'p':
				port = atoi(optarg);
				break;
		}
	}
}
