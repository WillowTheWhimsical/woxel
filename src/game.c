#include "game.h"
#include "input.h"
#include "tick.h"
#include "draw.h"
#include "texture.h"
#include "sound.h"
#include "music.h"
#include "world.h"
#include "entity.h"

#include <unistd.h>
#include <stdlib.h>
#include <string.h>

bool fullscreen = false;
float sensitivity = 0.25;
char path[256] = {'\0'};
Vector3 size = {32, 32, 32};

void init() {
	SetTraceLogLevel(LOG_WARNING);
	
	SetConfigFlags(FLAG_MSAA_4X_HINT);

	if (fullscreen) {
		InitWindow(0, 0, "Willow's Whimsical World");
		ToggleFullscreen();
	}
	else
		InitWindow(800, 600, "Willow's Whimsical World");

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
}

int loop() {
	get_input();
	tick();
	render();
	music_update();

	return WindowShouldClose();
}

void uninit() {
	CloseAudioDevice();
	destroy_world();
	unload_textures();
	unload_sounds();
	unload_music();
}

void process_args(int argc, char* argv[]) {
	int opt;
	while ((opt = getopt(argc, argv, "fs:o:w:h:l:")) != -1) {
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
			case 'w':
				size.x = atoi(optarg);
				break;
			case 'h':
				size.y = atoi(optarg);
				break;
			case 'l':
				size.z = atoi(optarg);
				break;
		}
	}
}
