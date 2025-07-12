#ifndef WORLD_H
#define WORLD_H

struct World {
	int* tile;
	int w, h, l;
};
extern struct World world;

void generate_world(int w, int h, int l);
void save_world(const char* path);
void load_world(const char* path);
void destroy_world();

int get_block(int x, int y, int z);
void set_block(int x, int y, int z, int block);

#endif
