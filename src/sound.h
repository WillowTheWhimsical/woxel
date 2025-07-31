#ifndef SOUND_H
#define SOUND_H

#include <raylib.h>

extern Sound* sound;

enum SOUNDS {
	S_BLIP,
	S_SPLAT,
	S_FIDDLESTICKS,
	S_BUTTON,
	S_TICK,
	S_STEP0,
	S_STEP1,
	S_STEP2,
	S_STEP3,
	S_DENY,
	S_SWITCH,
	S_MOVESELECT,
	S_SELECT,
	
	SOUNDS
};

void load_sounds();
void unload_sounds();

#endif
