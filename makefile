CC=gcc
CFLAGS=-Wall
LDFLAGS=-lraylib

SRC=./src/*.c
TARGET=./voxel

all: debug

debug:
	$(CC) $(CFLAGS) -Werror $(SRC) -o $(TARGET) $(LDFLAGS)

release:
	$(CC) $(CFLAGS) -O3 $(SRC) -o $(TARGET) $(LDFLAGS)

clean:
	rm -f $(TARGET)
