#include "game.h"

int main(int argc, char* argv[]) {
	process_args(argc, argv);

	init();
	while (!loop());
	uninit();

	return 0;
}
