#include "music.h"

#include <string.h>

Music music;

void music_update() {
	static float timer;
	static int delay;

	if (delay == 0) delay = GetRandomValue(1, 6) * 30;

	if (timer >= delay) {
		char song[16];
		switch (GetRandomValue(0, 4)) {
			case 0:
				strcpy(song, "gymnopedie1");
				break;
			case 1:
				strcpy(song, "gymnopedie2");
				break;
			case 2:
				strcpy(song, "gymnopedie3");
				break;
			case 3:
				strcpy(song, "nocturne-9.2");
				break;
			case 4:
				strcpy(song, "nocturne-55.1");
				break;
		}
		UnloadMusicStream(music);
		music = LoadMusicStream(TextFormat("res/mus/%s.ogg", song));
		PlayMusicStream(music);
		timer = 0;
		delay = 0;
	}

	if (GetMusicTimePlayed(music) >= GetMusicTimeLength(music) - 0.1) StopMusicStream(music);

	if (!IsMusicStreamPlaying(music))
		timer += GetFrameTime();
	else
		UpdateMusicStream(music);
}

void unload_music() {
	UnloadMusicStream(music);
}
