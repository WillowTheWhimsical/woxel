#include "tick.h"
#include "input.h"
#include "entity.h"

#include <raylib.h>

float timer = 0;

void tick() {
	timer += GetFrameTime();
	if (timer > 1.f / 60.f) {
		input_tick();

		for_entities(e)
			entity_tick(e);
		}

		timer = 0;
	}
}
