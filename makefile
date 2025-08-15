CC = gcc
CFLAGS = -Wall -O3
LDFLAGS = -lm -lwif -lraylib -lenet

obj = .build/main.o .build/game.o .build/client.o .build/input.o .build/tick.o .build/draw.o .build/world.o .build/blocks.o .build/menu.o .build/texture.o .build/sound.o .build/music.o .build/entity.o .build/e_player.o .build/e_puppet.o .build/e_goobert.o
target = woxel

all: setup $(target) server

$(target): $(obj)
	$(CC) $^ -o $@ $(LDFLAGS)

$(obj): .build/%.o: src/%.c
	$(CC) $(CFLAGS) -c $^ -o $@

server:
	$(CC) $(CFLAGS) src/server.c -lenet -o $(target)-server

setup:
	mkdir -p .build

clean:
	rm -rf .build $(target) $(target)-server
