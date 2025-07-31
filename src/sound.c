#include "sound.h"

#include <malloc.h>

Sound* sound;

void load_sounds() {
	sound = malloc(sizeof(Sound) * SOUNDS);
	for (int i = 0; i < SOUNDS; i++) {
		Wave wave = LoadWave(TextFormat("res/sfx/%d.wav", i));
		sound[i] = LoadSoundFromWave(wave);
		UnloadWave(wave);
	}
}

void unload_sounds() {
	for (int i = 0; i < SOUNDS; i++)
		UnloadSound(sound[i]);
	free(sound);
}
