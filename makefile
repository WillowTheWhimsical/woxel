CC = gcc
CFLAGS = -Wall -O3
LDFLAGS = -lm -lraylib

obj = .build/main.o .build/game.o .build/input.o .build/tick.o .build/draw.o .build/world.o .build/menu.o .build/texture.o .build/sound.o .build/entity.o .build/e_player.o
target = woxel

all: setup $(target)

$(target): $(obj)
	$(CC) $^ -o $@ $(LDFLAGS)

$(obj): .build/%.o: src/%.c
	$(CC) $(CFLAGS) -c $^ -o $@

setup:
	mkdir -p .build

clean:
	rm -rf .build $(target)
