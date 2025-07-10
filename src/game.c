#include "game.h"
#include "input.h"
#include "tick.h"
#include "draw.h"
#include "texture.h"
#include "world.h"
#include "entity.h"

#include <raylib.h>
#include <unistd.h>
#include <stdlib.h>
#include <string.h>

bool fullscreen = false;
float sensitivity = 0.25;
char path[256] = {'\0'};

void init() {
	SetTraceLogLevel(LOG_WARNING);
	
	SetConfigFlags(FLAG_MSAA_4X_HINT);

	if (fullscreen) {
		InitWindow(0, 0, "Voxel");
		ToggleFullscreen();
	}
	else
		InitWindow(800, 600, "Voxel");

	DisableCursor();
	SetExitKey(KEY_NULL);

	// SetTargetFPS(GetMonitorRefreshRate(GetCurrentMonitor()));

	set_binds();
	input.sensitivity = sensitivity;

	load_textures();

	if (path[0] == '\0')
		generate_world(20, 20, 20);
	else
		load_world(path);

	SpawnEntity(E_PLAYER);
}

int loop() {
	get_input();
	tick();
	render();

	return WindowShouldClose();
}

void uninit() {
	destroy_world();
	unload_textures();
}

void process_args(int argc, char* argv[]) {
	int opt;
	while ((opt = getopt(argc, argv, "fs:o:")) != -1) {
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
		}
	}
}
