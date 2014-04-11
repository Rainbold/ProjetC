#include <SDL/SDL_image.h>
#include <assert.h>

#include <sprite.h>
#include <misc.h>
#include <constant.h>

// Sprites general 
#define MAP_CASE        "sprite/wood_box.png"
#define MAP_KEY			"sprite/key.png"
#define MAP_DOOR		"sprite/door.png"
#define MAP_CLOSED_DOOR	"sprite/closed_door.png"

// Scenery elements
#define MAP_STONE		"sprite/stone.png"
#define MAP_TREE        "sprite/tree.png"

// Sprites of Banner
#define BANNER_LINE		"sprite/banner_line.png"
#define BANNER_LIFE		"sprite/banner_life.png"
#define BANNER_BOMB		"sprite/bomb3.png"
#define BANNER_RANGE	"sprite/banner_range.png"
#define BANNER_0		"sprite/banner_0.jpg"
#define BANNER_1		"sprite/banner_1.jpg"
#define BANNER_2		"sprite/banner_2.jpg"
#define BANNER_3		"sprite/banner_3.jpg"
#define BANNER_4		"sprite/banner_4.jpg"
#define BANNER_5		"sprite/banner_5.jpg"
#define BANNER_6		"sprite/banner_6.jpg"
#define BANNER_7		"sprite/banner_7.jpg"
#define BANNER_8		"sprite/banner_8.jpg"
#define BANNER_9		"sprite/banner_9.jpg"

// Sprites of Bombs
#define BOMB			"sprite/bomb40.png"

// Sprites of menu
#define MENU_BG_GREY 	"sprite/m_bg_grey.png"
#define MENU_BG_MAIN	"sprite/m_bg_main.png"
#define MENU_B_NEWGAME	"sprite/m_b_newgame.png"
#define MENU_B_QUIT		"sprite/m_b_quit.png"
#define MENU_B_KEEP		"sprite/m_b_continuer.png"
#define MENU_B_MAIN		"sprite/m_b_main.png"
#define MENU_H_PAUSE	"sprite/m_h_pause.png"

// Sprites of Bonus
#define IMG_BONUS_BOMB_RANGE_INC  "sprite/bonus_bomb_range_inc.png"
#define IMG_BONUS_BOMB_RANGE_DEC  "sprite/bonus_bomb_range_dec.png"
#define IMG_BONUS_BOMB_NB_INC     "sprite/bonus_bomb_nb_inc.png"
#define IMG_BONUS_BOMB_NB_DEC     "sprite/bonus_bomb_nb_dec.png"

// Sprites of Players
#define PLAYER_LEFT     "sprite/player_left.png"
#define PLAYER_UP       "sprite/player_up.png"
#define PLAYER_RIGHT    "sprite/player_right.png"
#define PLAYER_DOWN     "sprite/player_down.png"

#define PLAYER			"sprite/bomberman40.png"

// Sprites of Monsters
#define MONSTER_LEFT     "sprite/monster_left.png"
#define MONSTER_UP       "sprite/monster_up.png"
#define MONSTER_RIGHT    "sprite/monster_right.png"
#define MONSTER_DOWN     "sprite/monster_down.png"

/*	Main menu : main BG with sprites | id text[]
 * 		1--Single player				1
 * 		  |----New Game						6
 * 		  |_-_-Load Game					7
 * 		2--Multi players				2
 * 		  |----2 players					8
 * 		  |----3 players					9
 * 		  |_-_-4 players					10
 * 		3--Quit							3
 *
 * 	Pause menu : BG grey with alpha
 * 		1--Continue						4
 * 		2--Main menu					5
 * 		  |----Save ?						11
 * 		  |----yes							12
 * 		  |_-_-No							13
 * 		3--Quit							3
 * 		  |----Save ?						11
 *		  |----yes							12
 * 		  |_-_-No							13
 *
 * 		  							Total : 13
 */

// text
SDL_Surface* numbers_ttf[10];
SDL_Surface* text[3];
#ifdef USE_WIIMOTE
/*Text : 						id
 * Looking for wiimotes...		1
 * wiimote id connected			2
 * wiimote id disconnected		3
 */
SDL_Surface* text_wiimote[3];
#endif


// banner
SDL_Surface* numbers[10];
SDL_Surface* banner_life;
SDL_Surface* banner_bomb;
SDL_Surface* banner_range;
SDL_Surface* banner_line;

// map
SDL_Surface* box;
SDL_Surface* goal;
SDL_Surface* key;
SDL_Surface* door;
SDL_Surface* closed_door;
SDL_Surface* stone;
SDL_Surface* tree;

// Menu
SDL_Surface* menu[NB_SELECT_MENU];

#define SIZE_OF_SPRITE 40

// bonus
#define NB_BONUS 4
SDL_Surface* bonus[NB_BONUS];

// monster
SDL_Surface* monster_img[4];

// player
SDL_Surface* player_img[4]; // old
// sprites 40x60
#define NB_ANIM_PLAYER 8
#define TAILLE_TAB_ANIM_PLAYER 4
#define SIZE_OF_PLAYER_SPRITE 60

SDL_Surface* players;

SDL_Rect* player[TAILLE_TAB_ANIM_PLAYER];
SDL_Rect player_rect_up[NB_ANIM_PLAYER];
SDL_Rect player_rect_down[NB_ANIM_PLAYER];
SDL_Rect player_rect_left[NB_ANIM_PLAYER];
SDL_Rect player_rect_right[NB_ANIM_PLAYER];

// bombs
#define TAILLE_TAB_ANIM_BOMB 8
#define NB_ANIM_BOMBS 4

SDL_Surface* bombs;

SDL_Rect* bomb[TAILLE_TAB_ANIM_BOMB];
SDL_Rect bomb_rect_wait[NB_ANIM_BOMBS];
SDL_Rect bomb_rect_center[2*NB_ANIM_BOMBS - 1];
SDL_Rect bomb_rect_v[2*NB_ANIM_BOMBS - 1];
SDL_Rect bomb_rect_up[2*NB_ANIM_BOMBS - 1];
SDL_Rect bomb_rect_down[2*NB_ANIM_BOMBS - 1];
SDL_Rect bomb_rect_left[2*NB_ANIM_BOMBS - 1];
SDL_Rect bomb_rect_right[2*NB_ANIM_BOMBS - 1];
SDL_Rect bomb_rect_h[2*NB_ANIM_BOMBS - 1];

void banner_load() {
	// numbers imgs
	numbers[0] = load_image(BANNER_0);
	numbers[1] = load_image(BANNER_1);
	numbers[2] = load_image(BANNER_2);
	numbers[3] = load_image(BANNER_3);
	numbers[4] = load_image(BANNER_4);
	numbers[5] = load_image(BANNER_5);
	numbers[6] = load_image(BANNER_6);
	numbers[7] = load_image(BANNER_7);
	numbers[8] = load_image(BANNER_8);
	numbers[9] = load_image(BANNER_9);

	// other banner sprites
	banner_life = load_image(BANNER_LIFE);
	banner_bomb = load_image(BANNER_BOMB);
	banner_range = load_image(BANNER_RANGE);
	banner_line = load_image(BANNER_LINE);
}

void banner_unload() {
	// numbers imgs
	for (int i = 0; i < 10; i++) {
		SDL_FreeSurface(numbers[i]);
	}

	// other banner sprites
	SDL_FreeSurface(banner_life);
	SDL_FreeSurface(banner_bomb);
	SDL_FreeSurface(banner_range);
	SDL_FreeSurface(banner_life);
}

void map_load() {
	// Sprite loading
	tree = load_image(MAP_TREE);
	box = load_image(MAP_CASE);
	key = load_image(MAP_KEY);
	stone = load_image(MAP_STONE);
	door = load_image(MAP_DOOR);
	closed_door = load_image(MAP_CLOSED_DOOR);
}

void map_unload() {
	SDL_FreeSurface(tree);
	SDL_FreeSurface(box);
	SDL_FreeSurface(goal);
	SDL_FreeSurface(key);
	SDL_FreeSurface(stone);
	SDL_FreeSurface(door);
	SDL_FreeSurface(closed_door);
}

void bonus_load() {
	bonus[BONUS_RANGE_INC-1] = load_image(IMG_BONUS_BOMB_RANGE_INC);
	bonus[BONUS_RANGE_DEC-1] = load_image(IMG_BONUS_BOMB_RANGE_DEC);
	bonus[BONUS_BOMB_INC-1] = load_image(IMG_BONUS_BOMB_NB_INC);
	bonus[BONUS_BOMB_DEC-1] = load_image(IMG_BONUS_BOMB_NB_DEC);
}

void bonus_unload() {
	for (int i = 0; i < NB_BONUS; i++)
		SDL_FreeSurface(bonus[i]);
}

void player_load() {
	player_img[WEST] = load_image(PLAYER_LEFT);
	player_img[EAST] = load_image(PLAYER_RIGHT);
	player_img[NORTH] = load_image(PLAYER_UP);
	player_img[SOUTH] = load_image(PLAYER_DOWN);

	players = load_image(PLAYER);

	player[SOUTH] =  player_rect_down;
	player[NORTH] = player_rect_up;
	player[EAST] = player_rect_right;
	player[WEST] = player_rect_left;

	for(int i = 0; i < NB_ANIM_PLAYER; i++) {
		player_rect_down[i].x = SIZE_OF_SPRITE * i;
		player_rect_down[i].y = 0;
		player_rect_down[i].w = SIZE_OF_SPRITE ;
		player_rect_down[i].h = SIZE_OF_PLAYER_SPRITE;
	}
	for(int i = 0; i < NB_ANIM_PLAYER; i++) {
		player_rect_up[i].x = SIZE_OF_SPRITE * i;
		player_rect_up[i].y = 3 * SIZE_OF_PLAYER_SPRITE;
		player_rect_up[i].w = SIZE_OF_SPRITE ;
		player_rect_up[i].h = SIZE_OF_PLAYER_SPRITE;
	}
	for(int i = 0; i < NB_ANIM_PLAYER; i++) {
		player_rect_right[i].x = SIZE_OF_SPRITE * i;
		player_rect_right[i].y = SIZE_OF_PLAYER_SPRITE;
		player_rect_right[i].w = SIZE_OF_SPRITE ;
		player_rect_right[i].h = SIZE_OF_PLAYER_SPRITE;
	}
	for(int i = 0; i < NB_ANIM_PLAYER; i++) {
		player_rect_left[i].x = SIZE_OF_SPRITE * i;
		player_rect_left[i].y = 2 * SIZE_OF_PLAYER_SPRITE;
		player_rect_left[i].w = SIZE_OF_SPRITE ;
		player_rect_left[i].h = SIZE_OF_PLAYER_SPRITE;
	}

}

void player_unload() {
	for (int i = 0; i < 4; i++)
		SDL_FreeSurface(player_img[i]);
	SDL_FreeSurface(players);
}

void monster_load() {
	monster_img[WEST] = load_image(MONSTER_LEFT);
	monster_img[EAST] = load_image(MONSTER_RIGHT);
	monster_img[NORTH] = load_image(MONSTER_UP);
	monster_img[SOUTH] = load_image(MONSTER_DOWN);
}

void monster_unload() {
	for (int i = 0; i < 4; i++)
		SDL_FreeSurface(monster_img[i]);
}

void bombs_load()
{
	bombs = load_image(BOMB);
	bomb[0] = bomb_rect_center;
	bomb[1] = bomb_rect_v;
	bomb[2] = bomb_rect_h;
	bomb[3] = bomb_rect_down;
	bomb[4] = bomb_rect_up;
	bomb[5] = bomb_rect_left;
	bomb[6] = bomb_rect_right;
	bomb[7] = bomb_rect_wait;

	for(int i = 0; i < NB_ANIM_BOMBS; i++) {
		bomb_rect_wait[i].x = 0;
		bomb_rect_wait[i].y = SIZE_OF_SPRITE * i;
		bomb_rect_wait[i].w = SIZE_OF_SPRITE;
		bomb_rect_wait[i].h = SIZE_OF_SPRITE;
	}
	for(int i = 0; i < NB_ANIM_BOMBS; i++) {
		bomb_rect_center[i].x = 7 * SIZE_OF_SPRITE;
		bomb_rect_center[i].y = SIZE_OF_SPRITE * i;
		bomb_rect_center[i].w = SIZE_OF_SPRITE;
		bomb_rect_center[i].h = SIZE_OF_SPRITE;
	}
	for(int i = NB_ANIM_BOMBS; i < 2*NB_ANIM_BOMBS - 1; i++) {
		bomb_rect_center[i].x = 7 * SIZE_OF_SPRITE;
		bomb_rect_center[i].y = bomb_rect_center[2*NB_ANIM_BOMBS-1-i].y;
		bomb_rect_center[i].w = SIZE_OF_SPRITE;
		bomb_rect_center[i].h = SIZE_OF_SPRITE;
	}

	for(int i = 0; i < NB_ANIM_BOMBS; i++) {
		bomb_rect_v[i].x = 5 * SIZE_OF_SPRITE;
		bomb_rect_v[i].y = SIZE_OF_SPRITE * i;
		bomb_rect_v[i].w = SIZE_OF_SPRITE;
		bomb_rect_v[i].h = SIZE_OF_SPRITE;
	}
	for(int i = NB_ANIM_BOMBS; i < 2*NB_ANIM_BOMBS - 1; i++) {
		bomb_rect_v[i].x = 5 * SIZE_OF_SPRITE;
		bomb_rect_v[i].y = bomb_rect_v[2*NB_ANIM_BOMBS-1-i].y;
		bomb_rect_v[i].w = SIZE_OF_SPRITE;
		bomb_rect_v[i].h = SIZE_OF_SPRITE;
	}

	for(int i = 0; i < NB_ANIM_BOMBS; i++) {
		bomb_rect_up[i].x = 1 * SIZE_OF_SPRITE;
		bomb_rect_up[i].y = SIZE_OF_SPRITE * i;
		bomb_rect_up[i].w = SIZE_OF_SPRITE;
		bomb_rect_up[i].h = SIZE_OF_SPRITE;
	}
	for(int i = NB_ANIM_BOMBS; i < 2*NB_ANIM_BOMBS - 1; i++) {
		bomb_rect_up[i].x = 1 * SIZE_OF_SPRITE;
		bomb_rect_up[i].y = bomb_rect_up[2*NB_ANIM_BOMBS-1-i].y;
		bomb_rect_up[i].w = SIZE_OF_SPRITE;
		bomb_rect_up[i].h = SIZE_OF_SPRITE;
	}

	for(int i = 0; i < NB_ANIM_BOMBS; i++) {
		bomb_rect_down[i].x = 2 * SIZE_OF_SPRITE;
		bomb_rect_down[i].y = SIZE_OF_SPRITE * i;
		bomb_rect_down[i].w = SIZE_OF_SPRITE;
		bomb_rect_down[i].h = SIZE_OF_SPRITE;
	}
	for(int i = NB_ANIM_BOMBS; i < 2*NB_ANIM_BOMBS - 1; i++) {
		bomb_rect_down[i].x = 2 * SIZE_OF_SPRITE;
		bomb_rect_down[i].y = bomb_rect_down[2*NB_ANIM_BOMBS-1-i].y;
		bomb_rect_down[i].w = SIZE_OF_SPRITE;
		bomb_rect_down[i].h = SIZE_OF_SPRITE;
	}

	for(int i = 0; i < NB_ANIM_BOMBS; i++) {
		bomb_rect_left[i].x = 3 * SIZE_OF_SPRITE;
		bomb_rect_left[i].y = SIZE_OF_SPRITE * i;
		bomb_rect_left[i].w = SIZE_OF_SPRITE;
		bomb_rect_left[i].h = SIZE_OF_SPRITE;
	}
	for(int i = NB_ANIM_BOMBS; i < 2*NB_ANIM_BOMBS - 1; i++) {
		bomb_rect_left[i].x = 3 * SIZE_OF_SPRITE;
		bomb_rect_left[i].y = bomb_rect_left[2*NB_ANIM_BOMBS-1-i].y;
		bomb_rect_left[i].w = SIZE_OF_SPRITE;
		bomb_rect_left[i].h = SIZE_OF_SPRITE;
	}

	for(int i = 0; i < NB_ANIM_BOMBS; i++) {
		bomb_rect_right[i].x = 4 * SIZE_OF_SPRITE;
		bomb_rect_right[i].y = SIZE_OF_SPRITE * i;
		bomb_rect_right[i].w = SIZE_OF_SPRITE;
		bomb_rect_right[i].h = SIZE_OF_SPRITE;
	}
	for(int i = NB_ANIM_BOMBS; i < 2*NB_ANIM_BOMBS - 1; i++) {
		bomb_rect_right[i].x = 4 * SIZE_OF_SPRITE;
		bomb_rect_right[i].y = bomb_rect_right[2*NB_ANIM_BOMBS-1-i].y;
		bomb_rect_right[i].w = SIZE_OF_SPRITE;
		bomb_rect_right[i].h = SIZE_OF_SPRITE;
	}

	for(int i = 0; i < NB_ANIM_BOMBS; i++) {
		bomb_rect_h[i].x = 6 * SIZE_OF_SPRITE;
		bomb_rect_h[i].y = SIZE_OF_SPRITE * i;
		bomb_rect_h[i].w = SIZE_OF_SPRITE;
		bomb_rect_h[i].h = SIZE_OF_SPRITE;
	}
	for(int i = NB_ANIM_BOMBS; i < 2*NB_ANIM_BOMBS - 1; i++) {
		bomb_rect_h[i].x = 6 * SIZE_OF_SPRITE;
		bomb_rect_h[i].y = bomb_rect_h[2*NB_ANIM_BOMBS-1-i].y;
		bomb_rect_h[i].w = SIZE_OF_SPRITE;
		bomb_rect_h[i].h = SIZE_OF_SPRITE;
	}
}

void bombs_unload()
{
	SDL_FreeSurface(bombs);
}

void menu_load() {
	menu[M_B_NEWGAME] = load_image(MENU_B_NEWGAME);
	menu[M_B_MAINMENU] = load_image(MENU_B_MAIN);
	menu[M_B_QUIT] = load_image(MENU_B_QUIT);
	menu[M_B_KEEP] = load_image(MENU_B_KEEP);
	menu[M_BG_GREY] = load_image(MENU_BG_GREY);
	menu[M_BG_MAINMENU] = load_image(MENU_BG_MAIN);
	menu[M_H_PAUSE] = load_image(MENU_H_PAUSE);
}

void menu_unload() {
	for (int i = 0; i < NB_SELECT_MENU; i++)
	SDL_FreeSurface(menu[i]);
}
void sprite_load() {
	map_load();
	bonus_load();
	banner_load();
	player_load();
	monster_load();
	bombs_load();
	menu_load();
}

void sprite_free() {
	map_unload();
	bonus_unload();
	banner_unload();
	player_unload();
	monster_unload();
	bombs_unload();
	menu_unload();
}
/*
SDL_Surface* sprite_get_bomb(short bombAnim) {
	assert(bombAnim >= 0 && bombAnim < NB_BOMBS);
	return bombs[bombAnim];
}
*/
SDL_Surface* sprite_get_bombs() {
	return bombs;
}

// Sprite function

SDL_Rect sprite_get_rect_bomb_anim(int i, int j) {
	assert(0 <= i && 0 <= j && i < TAILLE_TAB_ANIM_BOMB && j < 2*NB_ANIM_BOMBS);
	SDL_Rect* rect = bomb[i];
	return rect[j];
}

SDL_Surface* sprite_get_number(short number) {
	assert(number >= 0 && number <= 9);
	return numbers[number];
}
/*
SDL_Surface* sprite_get_player(enum way direction) {
	assert(player_img[direction]);
	return player_img[direction];
} */
SDL_Surface* sprite_get_players() {
	return players;
}

SDL_Rect sprite_get_rect_player_anim(int i,enum way direction) {
	//assert(players[direction]);
	SDL_Rect* rect = player[direction];
	//assert(rect[i]);
	return rect[i];
}

SDL_Surface* sprite_get_monster(enum way direction) {
	assert(monster_img[direction]);
	return monster_img[direction];
}

SDL_Surface* sprite_get_banner_life() {
	assert(banner_life);
	return banner_life;
}

SDL_Surface* sprite_get_banner_bomb() {
	assert(banner_bomb);
	return banner_bomb;
}

SDL_Surface* sprite_get_banner_line() {
	assert(banner_line);
	return banner_line;
}

SDL_Surface* sprite_get_banner_range() {
	assert(banner_range);
	return banner_range;
}

SDL_Surface* sprite_get_bonus(bonus_type_t bonus_type) {
	assert(bonus[bonus_type-1]);
	return bonus[bonus_type-1];
}

SDL_Surface* sprite_get_menu(select_menu_t select_menu) {
	assert(menu[select_menu]);
	return menu[select_menu];
}

SDL_Surface* sprite_get_tree() {
	assert(tree);
	return tree;
}

SDL_Surface* sprite_get_box() {
	assert(box);
	return box;
}

SDL_Surface* sprite_get_key() {
	assert(key);
	return key;
}

SDL_Surface* sprite_get_stone() {
	assert(stone);
	return stone;
}

SDL_Surface* sprite_get_door() {
	assert(door);
	return door;
}

SDL_Surface* sprite_get_closed_door() {
	assert(closed_door);
	return closed_door;
}
