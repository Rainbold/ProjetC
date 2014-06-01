#ifndef CONSTANT_H_
#define CONSTANT_H_

#define WINDOW_NAME "Bomberman Returns"

// Comment if you don't want to use wiimote or if you cannot compile
//#define USE_WIIMOTE

// Size (# of pixels) of a cell of the map 
#define SIZE_BLOC       40

// Size (# of pixels) of banner
#define LINE_HEIGHT			4
#define BANNER_HEIGHT		40

// Time management
#define DEFAULT_GAME_FPS 30

// map size
#define MENU_WIDTH 600
#define MENU_HEIGHT 600
#define MAP_WIDTH 12
#define MAP_HEIGHT 12

typedef enum way {
	NORTH = 0,
	SOUTH, // 1
	WEST, // 2
	EAST //3
} way_t;

typedef enum bomb_currentAnimation {
	ANIM_1 = 0,
	ANIM_2, // 1
	ANIM_3, // 2
	ANIM_4, //3
	ANIM_5 //4
} b_curAnim;

typedef enum state {
	MENU = 0,
	GAME,
	NEWGAME_SINGLE,
	LOADGAME,

	NEWGAME_MULTI2,
	NEWGAME_MULTI3,
	NEWGAME_MULTI4,

	GO, // GameOver
	W, // Win

	ENDGAME,
	QUIT,
	KEEP,
	MAINMENU,
	CHANGEMAP,
	SAVEGAME_MAINMENU,
	SAVEGAME_QUIT,

	DEFAULT,
} state_t;

typedef enum game_state {
	PLAYING = 0,
	PAUSED, // 1
	CHOOSE_MAP, //2
	SCORE, // 3
} game_state_t;

#define NB_SURFACE_MENU 26
typedef enum select_menu {
	M_B_SINGLE = 0,
	M_B_NEWGAME,	// 1
	M_B_LOADGAME,	// 2

	M_B_MULTI,		// 3
	M_B_2PLAYER,	// 4
	M_B_3PLAYER,	// 5
	M_B_4PLAYER,	// 6

	M_H_PAUSE,		// 7
	M_B_KEEP,		// 8
	M_B_MAINMENU,	// 9
	M_B_QUIT,		// 10

	M_H_SAVE,		// 11
	M_B_YES,		// 12
	M_B_NO,			// 13

	M_B_CHANGEMAP,	// 14

	M_BG_GREY,		// 15
	M_BG_MAINMENU,	// 16
	M_BG_WIN,		// 17
	M_BG_GAMEOVER,	// 18

	M_S_SELECT,		// 19
	M_S_SELECT_BLACK,// 20
	M_S_STARS,		// 21
	M_S_ALIEN2,		// 22
	M_S_ALIEN3,		// 23
	M_S_ALIEN4,		// 24
	M_S_CAGE,		// 25
} select_menu_t;

typedef enum type_menu {
	MAIN = 0,
	GAMEOVER,
	WIN,
	PAUSE_SINGLE,
	PAUSE_MULTI,
} type_t;

typedef enum key_event {
	UP = 0,
	DOWN
} key_event_t;
#endif /* CONSTANT */
