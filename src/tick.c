#include "tick.h"
#include "input.h"
#include "entity.h"

#include <raylib.h>

const float TICKRATE = 1 / 60.f;

float t, lt, dt, ptimer = 0;

void tick() {
	lt = t;
	t = GetTime();
	ptimer += t - lt;

	while (ptimer >= TICKRATE) {
		input_tick();

		for_entities(e)
			entity_tick(e);
		}

		ptimer -= TICKRATE;
	}
}
