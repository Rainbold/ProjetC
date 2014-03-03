#ifndef CONSTANT_H_
#define CONSTANT_H_

#define WINDOW_NAME "[PG110] Bomberman 2014"

// Size (# of pixels) of a cell of the map
#define SIZE_BLOC       40

// Size (# of pixels) of banner
#define LINE_HEIGHT			4
#define BANNER_HEIGHT		40

// Time management
#define DEFAULT_GAME_FPS 30

// map size
#define MAP_WIDTH 12
#define MAP_HEIGHT 12

typedef enum way {
	NORTH = 0,
	SOUTH, // 1
	WEST, // 2
	EAST //3
} way_t;

#endif /* CONSTANT */
