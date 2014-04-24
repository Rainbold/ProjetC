#ifndef CONSTANT_H_
#define CONSTANT_H_

#define WINDOW_NAME "[PG110] Bomberman 2014"

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

/*	NEWGAME_MULTI2_1,
	NEWGAME_MULTI2_2,
	NEWGAME_MULTI2_3,
	NEWGAME_MULTI2_4,
	NEWGAME_MULTI2_5,
	NEWGAME_MULTI2_6,
	NEWGAME_MULTI2_7,
	NEWGAME_MULTI2_8,
	NEWGAME_MULTI2_9,
	NEWGAME_MULTI2_10,

	NEWGAME_MULTI3_1,
	NEWGAME_MULTI3_2,
	NEWGAME_MULTI3_3,
	NEWGAME_MULTI3_4,
	NEWGAME_MULTI3_5,
	NEWGAME_MULTI3_6,
	NEWGAME_MULTI3_7,
	NEWGAME_MULTI3_8,
	NEWGAME_MULTI3_9,
	NEWGAME_MULTI3_10,

	NEWGAME_MULTI4_1,
	NEWGAME_MULTI4_2,
	NEWGAME_MULTI4_3,
	NEWGAME_MULTI4_4,
	NEWGAME_MULTI4_5,
	NEWGAME_MULTI4_6,
	NEWGAME_MULTI4_7,
	NEWGAME_MULTI4_8,
	NEWGAME_MULTI4_9,
	NEWGAME_MULTI4_10,
*/
	ENDGAME,
	QUIT,
	KEEP,
	MAINMENU
} state_t;

typedef enum game_state {
	PLAYING = 0,
	PAUSED, // 1
	CHOOSE_MAP, //2
} game_state_t;

#define NB_SURFACE_MENU 19
typedef enum select_menu {
	M_B_SINGLE = 0,
	M_B_NEWGAME,
	M_B_LOADGAME,

	M_B_MULTI,
	M_B_2PLAYER,
	M_B_3PLAYER,
	M_B_4PLAYER,

	M_H_PAUSE,
	M_B_KEEP,
	M_B_MAINMENU,
	M_B_QUIT,

	M_H_SAVE,
	M_B_YES,
	M_B_NO,

	M_BG_GREY,
	M_BG_MAINMENU,

	M_SELECT,
	M_SELECT_BLACK,
	M_STARS // 19/19
} select_menu_t;

typedef enum type_menu {
	MAIN = 0,
	PAUSE_SINGLE,
	PAUSE_MULTI,
} type_t;

typedef enum key_event {
	UP = 0,
	DOWN
} key_event_t;
#endif /* CONSTANT */
