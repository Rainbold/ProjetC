#include <SDL/SDL_image.h>
#include <SDL/SDL_ttf.h>
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

// Font
#define FONT 			"sprite/font.ttf"

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
#define MENU_STARS		"sprite/stars.png"

// Sprites of Bonus
#define IMG_BONUS_BOMB_RANGE_INC  "sprite/bonus_bomb_range_inc.png"
#define IMG_BONUS_BOMB_RANGE_DEC  "sprite/bonus_bomb_range_dec.png"
#define IMG_BONUS_BOMB_NB_INC     "sprite/bonus_bomb_nb_inc.png"
#define IMG_BONUS_BOMB_NB_DEC     "sprite/bonus_bomb_nb_dec.png"

// Sprites of Players
#define PLAYER			"sprite/bomberman40.png"

// Sprites of Monsters
#define MONSTER_LEFT     "sprite/monster_left.png"
#define MONSTER_UP       "sprite/monster_up.png"
#define MONSTER_RIGHT    "sprite/monster_right.png"
#define MONSTER_DOWN     "sprite/monster_down.png"

/*	Main menu : main BG with sprites | id text[]
 * 		1--Single player				0
 * 		  |----New Game						5
 * 		  |_-_-Load Game					6
 * 		2--Multi players				1
 * 		  |----2 players					7
 * 		  |----3 players					8
 * 		  |_-_-4 players					9
 *
 * 	Pause menu : BG grey with alpha
 * 		1--Continue						2
 * 		2--Main menu					3
 * 		  |----Save ?						10
 * 		  |----yes							11
 * 		  |_-_-No							12
 * 		3--Quit							4
 * 		  |----Save ?						10
 *		  |----yes							11
 * 		  |_-_-No							12
 *
 * 		  							Total : 13 + 1 curseur + 1 Pause
 */

SDL_Surface* numbers[10];
SDL_Surface* menu[NB_SURFACE_MENU];

#define NB_ANIM_STARS 4
#define SIZE_OF_STARS 10
SDL_Surface* menu_stars;
SDL_Rect menu_rect_stars[2 * NB_ANIM_STARS - 1];

#ifdef USE_WIIMOTE
/*Text : 						id
 * Looking for wiimotes...		1
 * wiimote id connected			2
 * wiimote id disconnected		3
 */
SDL_Surface* text_wiimote[3];
#endif


// banner
//SDL_Surface* numbers[10];
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

#define SIZE_OF_SPRITE 40

// bonus
#define NB_BONUS 4
SDL_Surface* bonus[NB_BONUS];

// monster
SDL_Surface* monster_img[4];

// player

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

void menu_load() {
	TTF_Font* police = TTF_OpenFont(FONT, 50);
	SDL_Color couleurBlanche = {255, 255, 255};
	SDL_Color couleurNoir = {0, 0, 0};

	menu[M_H_PAUSE] = TTF_RenderText_Blended(police, "PAUSE", couleurBlanche);
	menu[M_H_SAVE] = TTF_RenderText_Blended(police, "Save ?", couleurBlanche);

	TTF_CloseFont(police);
	police = TTF_OpenFont(FONT, 40);

	numbers[0] = TTF_RenderText_Blended(police, "0", couleurNoir);
	numbers[1] = TTF_RenderText_Blended(police, "1", couleurNoir);
	numbers[2] = TTF_RenderText_Blended(police, "2", couleurNoir);
	numbers[3] = TTF_RenderText_Blended(police, "3", couleurNoir);
	numbers[4] = TTF_RenderText_Blended(police, "4", couleurNoir);
	numbers[5] = TTF_RenderText_Blended(police, "5", couleurNoir);
	numbers[6] = TTF_RenderText_Blended(police, "6", couleurNoir);
	numbers[7] = TTF_RenderText_Blended(police, "7", couleurNoir);
	numbers[8] = TTF_RenderText_Blended(police, "8", couleurNoir);
	numbers[9] = TTF_RenderText_Blended(police, "9", couleurNoir);

	TTF_CloseFont(police);
	police = TTF_OpenFont(FONT, 22);

	menu[M_B_SINGLE] = TTF_RenderText_Blended(police, "Single Player", couleurBlanche);
	menu[M_B_NEWGAME] = TTF_RenderText_Blended(police, "New Game", couleurBlanche);
	menu[M_B_LOADGAME] = TTF_RenderText_Blended(police, "Load Game", couleurBlanche);

	menu[M_B_MULTI] = TTF_RenderText_Blended(police, "Multi  Player", couleurBlanche);
	menu[M_B_2PLAYER] = TTF_RenderText_Blended(police, "2 Players", couleurBlanche);
	menu[M_B_3PLAYER] = TTF_RenderText_Blended(police, "3 Players", couleurBlanche);
	menu[M_B_4PLAYER] = TTF_RenderText_Blended(police, "4 Players", couleurBlanche);

	menu[M_B_KEEP] = TTF_RenderText_Blended(police, "Continue", couleurBlanche);
	menu[M_B_MAINMENU] = TTF_RenderText_Blended(police, "Main Menu", couleurBlanche);
	menu[M_B_QUIT] = TTF_RenderText_Blended(police, "Quit", couleurBlanche);

	menu[M_B_YES] = TTF_RenderText_Blended(police, "Yes", couleurBlanche);
	menu[M_B_NO] = TTF_RenderText_Blended(police, "No", couleurBlanche);

	menu[M_SELECT] = TTF_RenderText_Blended(police, ">", couleurBlanche);



	TTF_CloseFont(police);

	menu[M_BG_GREY] = load_image(MENU_BG_GREY);
	menu[M_BG_MAINMENU] = load_image(MENU_BG_MAIN);
	menu[M_STARS] = load_image(MENU_STARS); // 18/18 -> ok

	for(int i = 0; i < NB_ANIM_STARS; i++) {
			menu_rect_stars[i].x = SIZE_OF_STARS * i;
			menu_rect_stars[i].y = 0;
			menu_rect_stars[i].w = SIZE_OF_STARS;
			menu_rect_stars[i].h = SIZE_OF_STARS;
		}
		for(int i = NB_ANIM_STARS; i < 2*NB_ANIM_STARS - 1; i++) {
			menu_rect_stars[i].x = menu_rect_stars[2*NB_ANIM_STARS-1-i].x;
			menu_rect_stars[i].y = 0;
			menu_rect_stars[i].w = SIZE_OF_STARS;
			menu_rect_stars[i].h = SIZE_OF_STARS;
		}
}

void menu_unload() {
	for (int i = 0; i < NB_SURFACE_MENU; i++) {
		SDL_FreeSurface(menu[i]);
	}
	for (int i = 0; i < 10; i++) {
		SDL_FreeSurface(numbers[i]);
	}
	SDL_FreeSurface(menu_stars);
}

void banner_load() {
	// numbers imgs
/*	numbers[0] = load_image(BANNER_0);
	numbers[1] = load_image(BANNER_1);
	numbers[2] = load_image(BANNER_2);
	numbers[3] = load_image(BANNER_3);
	numbers[4] = load_image(BANNER_4);
	numbers[5] = load_image(BANNER_5);
	numbers[6] = load_image(BANNER_6);
	numbers[7] = load_image(BANNER_7);
	numbers[8] = load_image(BANNER_8);
	numbers[9] = load_image(BANNER_9);
*/


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

SDL_Surface* sprite_get_players() {
	return players;
}

SDL_Rect sprite_get_rect_player_anim(int i,enum way direction) {
	SDL_Rect* rect = player[direction];
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

SDL_Rect* sprite_get_rect_stars() {
	return(menu_rect_stars);
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
