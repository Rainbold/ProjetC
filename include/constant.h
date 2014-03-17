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

typedef enum currentAnimation {
	ANIM_1 = 0,
	ANIM_2, // 1
	ANIM_3, // 2
	ANIM_4, //3
	ANIM_5 //4
} currentAnimation_t;

typedef enum state {
	MENU = 0,
	GAME,
	NEWGAME,
	ENDGAME,
	QUIT
} state_t;

typedef enum game_state {
	PLAYING = 0,
	PAUSED, // 1
} game_state_t;

#define NB_SELECT_MENU 7
typedef enum select_menu {
	M_B_NEWGAME = 0,
	M_B_MAINMENU,
	M_B_QUIT,
	M_B_KEEP,
	M_BG_GREY,
	M_BG_MAINMENU,
	M_H_PAUSE
} select_menu_t;

typedef enum type {
	MAIN = 0
} type_t;

#endif /* CONSTANT */
