#include <raylib.h>

int main(int argc, char* argv[]) {
	SetTraceLogLevel(LOG_WARNING);
	
	SetConfigFlags(FLAG_MSAA_4X_HINT);
	InitWindow(800, 600, "Voxel");
	SetExitKey(KEY_NULL);

	while (!WindowShouldClose()) {
		BeginDrawing();
			ClearBackground(BLACK);
			DrawFPS(0, 0);
		EndDrawing();
	}

	return 0;
}
