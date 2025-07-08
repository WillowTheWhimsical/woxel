CC = gcc
CFLAGS = -Wall
LDFLAGS = -lraylib -lm

obj = .build/main.o .build/game.o .build/input.o .build/tick.o .build/draw.o .build/world.o .build/entity.o .build/e_player.o
target = voxel

all: setup $(target)

$(target): $(obj)
	$(CC) $^ -o $@ $(LDFLAGS)

$(obj): .build/%.o: src/%.c
	$(CC) -c $(CFLAGS) $^ -o $@

setup:
	mkdir -p .build
	
clean:
	rm -rf .build $(target)
