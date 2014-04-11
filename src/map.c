#include <SDL/SDL_image.h>
#include <assert.h> 
#include <stdio.h>
#include <time.h>

#include <map.h>
#include <constant.h>
#include <misc.h>
#include <sprite.h>
#include <window.h>
#include <list.h>
#include <monster.h>

struct map {
	int width;
	int height;
	char* grid;
	struct list* monstersList;
	struct list* bombsList;
};

#define CELL(i,j) (i +  map->width * j)

struct map* map_new(int width, int height)
{
	assert(width > 0 && height > 0);

	struct map* map = malloc(sizeof *map);
	if (map == NULL )
		error("map_new : malloc map failed");

	map->width = width;
	map->height = height;

	map->monstersList = list_new();
	map->bombsList = list_new();

	map->grid = malloc(height * width);
	if (map->grid == NULL) {
		error("map_new : malloc grid failed");
	}

	// Grid cleaning
	int i, j;
	for (i = 0; i < width; i++)
		for (j = 0; j < height; j++)
			map->grid[CELL(i,j)] = CELL_EMPTY;

	return map;
}

struct list* map_load_monsters(struct map* map, struct game* game) {
	int i, j;
	map->monstersList = list_delete(map->monstersList);
	for (j = 0; j < map->height; j++)
		for (i = 0; i < map->width; i++)
			if(map->grid[CELL(i,j)] == CELL_MONSTER) 
				monster_init(map, i, j, MONSTER_NORMAL, 1, 2, 5, game);

	return map->monstersList;
}

int map_is_inside(struct map* map, int x, int y)
{
	assert(map);

	if(x < 0 || y < 0 || x >= map->width || y >= map->height)
		return 0;

	return 1;
}

void map_free(struct map* map)
{
	if (map == NULL )
		return;
	free(map->grid);
	free(map);
}

int map_get_width(struct map* map)
{
	assert(map != NULL);
	return map->width;
}

int map_get_height(struct map* map)
{
	assert(map);
	return map->height;
}

enum cell_type map_get_cell_type(struct map* map, int x, int y)
{
	assert(map && map_is_inside(map, x, y));
	return map->grid[CELL(x,y)] & 15;
}

char map_get_cell_compose_type(struct map* map, int x, int y)
{
	assert(map && map_is_inside(map, x, y));
	return map->grid[CELL(x,y)];
}

void map_set_cell_type(struct map* map, int x, int y, enum cell_type type)
{
	assert(map && map_is_inside(map, x, y));
	map->grid[CELL(x,y)] = type;
}

struct list* map_get_monsters(struct map* map)
{
	assert(map);
	return map->monstersList;
}

struct list* map_get_bombs(struct map* map)
{
	assert(map);
	return map->bombsList;
}

void map_set_monsters(struct map* map, struct list* mList)
{
	assert(map);
	map->monstersList = mList;
}

void map_set_bombs(struct map* map, struct list* bList)
{
	assert(map);
	map->bombsList = bList;
}

void map_case_destroyed(struct game* game, struct map* map, int x, int y)
{
	assert(map && map_is_inside(map, x, y));
	
	int r = rand_ab(0, 99);

	if(0 <= r && r < 30)
		map_set_cell_type(map, x, y, CELL_EMPTY);
	else if( 30 <= r && r < 35 )
		map_set_cell_type(map, x, y, CELL_BONUS_LIFE);
	else if( 35 <= r && r < 40 ) {
		map_set_cell_type(map, x, y, CELL_MONSTER);
		map_load_monsters(map, game);
	}
	else if( 40 <= r && r < 55 )
		map_set_cell_type(map, x, y, CELL_BONUS_RANGE_INC);
	else if( 55 <= r && r < 70 )
		map_set_cell_type(map, x, y, CELL_BONUS_RANGE_DEC);
	else if( 70 <= r && r < 85 )
		map_set_cell_type(map, x, y, CELL_BONUS_BOMB_INC);
	else if( 85 <= r && r < 100 )
		map_set_cell_type(map, x, y, CELL_BONUS_BOMB_DEC);
}

void display_bonus(struct map* map, int x, int y, char type)
{
	// bonus is encoded with the 4 most significant bits
	switch (type >> 4) {
	case BONUS_RANGE_INC:
		window_display_image(sprite_get_bonus(BONUS_RANGE_INC), x, y);
		break;

	case BONUS_RANGE_DEC:
		window_display_image(sprite_get_bonus(BONUS_RANGE_DEC), x, y);
		break;

	case BONUS_BOMB_INC:
		window_display_image(sprite_get_bonus(BONUS_BOMB_INC), x, y);
		break;

	case BONUS_BOMB_DEC:
		window_display_image(sprite_get_bonus(BONUS_BOMB_DEC), x, y);
		break;
	case BONUS_LIFE:
		window_display_image(sprite_get_banner_life(), x, y);
		break;
	}
}

void display_scenery(struct map* map, int x, int  y, char type)
{
	switch (type >> 4) { // sub-types are encoded with the 4 most significant bits
	case SCENERY_STONE:
		window_display_image(sprite_get_stone(), x, y);
		break;

	case SCENERY_TREE:
		window_display_image(sprite_get_tree(), x, y);
		break;
	}
}

void display_door(struct map* map, int x, int  y, unsigned char type)
{
	switch (type >> 7) { // sub-type is encoded with the most significant bit
	case CLOSED_DOOR:
		window_display_image(sprite_get_closed_door(), x, y);
		break;

	case OPENED_DOOR:
		window_display_image(sprite_get_door(), x, y);
		break;
	}
}

void map_display(struct map* map)
{
	assert(map != NULL);
	assert(map->height > 0 && map->width > 0);

	int x, y;
	for (int i = 0; i < map->width; i++) {
		for (int j = 0; j < map->height; j++) {
			x = i * SIZE_BLOC;
			y = j * SIZE_BLOC;

			char type = map->grid[CELL(i,j)];

			switch (type & 15) { // type is encoded with 4 bits, 15 (1111) is a mask to keep the four less significant bits
			case CELL_SCENERY:
				display_scenery(map, x, y, type);
				break;
			case CELL_CASE:
				window_display_image(sprite_get_box(), x, y);
				break;
			case CELL_BONUS:
				display_bonus(map, x, y, type);
				break;
			case CELL_KEY:
				window_display_image(sprite_get_key(), x, y);
				break;
			case CELL_DOOR:
				display_door(map, x, y, type);
				break;
			}
		}
	}
}

struct map* map_get_default(int n) {

	int map_width;
	int map_height;
struct map* map;

	switch(n) {
	case 1:
		map_width = 12;
		map_height = 12;
		map = map_new(map_width, map_height);
		break;
	case 2:
		map_width = 12;
		map_height = 12;
		map = map_new(map_width, map_height);
		break;
	case 3:
		map_width = 12;
		map_height = 24;
		map = map_new(map_width, map_height);
		break;
	}

	char themap1[12 * 12] = {
			CELL_PLAYER, CELL_EMPTY, CELL_EMPTY, CELL_EMPTY, CELL_EMPTY, CELL_EMPTY, CELL_EMPTY, CELL_EMPTY, CELL_EMPTY, CELL_EMPTY, CELL_EMPTY, CELL_EMPTY,
			CELL_STONE, CELL_STONE, CELL_STONE, CELL_EMPTY, CELL_STONE, CELL_EMPTY, CELL_STONE, CELL_STONE, CELL_STONE, CELL_STONE, CELL_EMPTY, CELL_EMPTY,
			CELL_BONUS_LIFE, CELL_EMPTY, CELL_EMPTY, CELL_EMPTY, CELL_STONE, CELL_CASE, CELL_STONE, CELL_EMPTY, CELL_EMPTY, CELL_STONE, CELL_EMPTY, CELL_EMPTY,
			CELL_EMPTY, CELL_EMPTY, CELL_EMPTY, CELL_EMPTY, CELL_STONE, CELL_CASE, CELL_STONE, CELL_EMPTY, CELL_EMPTY, CELL_STONE, CELL_EMPTY, CELL_EMPTY,
			CELL_EMPTY, CELL_EMPTY, CELL_EMPTY, CELL_EMPTY, CELL_STONE, CELL_CASE, CELL_STONE, CELL_EMPTY, CELL_EMPTY, CELL_STONE, CELL_EMPTY, CELL_EMPTY,
			CELL_EMPTY, CELL_EMPTY, CELL_EMPTY, CELL_EMPTY, CELL_STONE, CELL_STONE, CELL_STONE, CELL_EMPTY, CELL_EMPTY, CELL_STONE, CELL_EMPTY, CELL_EMPTY,
			CELL_EMPTY, CELL_EMPTY, CELL_EMPTY, CELL_EMPTY, CELL_EMPTY, CELL_EMPTY, CELL_EMPTY , CELL_EMPTY, CELL_EMPTY, CELL_STONE,  CELL_EMPTY, CELL_EMPTY,
			CELL_EMPTY, CELL_TREE, CELL_CASE, CELL_TREE, CELL_EMPTY, CELL_EMPTY, CELL_EMPTY, CELL_EMPTY,  CELL_EMPTY, CELL_STONE,  CELL_EMPTY, CELL_EMPTY,
			CELL_EMPTY, CELL_TREE, CELL_TREE, CELL_TREE, CELL_EMPTY, CELL_EMPTY, CELL_EMPTY, CELL_EMPTY, CELL_EMPTY,  CELL_STONE,  CELL_EMPTY, CELL_EMPTY,
			CELL_EMPTY, CELL_EMPTY, CELL_EMPTY, CELL_EMPTY, CELL_EMPTY, CELL_EMPTY, CELL_EMPTY, CELL_EMPTY, CELL_OPENED_DOOR | (2 << 4), CELL_STONE,  CELL_EMPTY, CELL_EMPTY,
			CELL_CASE, CELL_STONE, CELL_STONE, CELL_STONE, CELL_STONE, CELL_STONE, CELL_STONE, CELL_STONE, CELL_STONE, CELL_STONE,  CELL_CASE_LIFE, CELL_CASE_RANGE_INC,
		CELL_EMPTY,  CELL_EMPTY, CELL_EMPTY, CELL_EMPTY, CELL_EMPTY, CELL_EMPTY, CELL_EMPTY, CELL_EMPTY, CELL_EMPTY, CELL_EMPTY, CELL_BONUS_RANGE_INC, CELL_CLOSED_DOOR
		};
	char themap2[12 * 12] = {
			CELL_STONE, CELL_STONE, CELL_STONE, CELL_STONE, CELL_STONE, CELL_STONE, CELL_STONE, CELL_STONE, CELL_STONE, CELL_STONE, CELL_STONE, CELL_STONE,
			CELL_PLAYER, CELL_EMPTY, CELL_EMPTY, CELL_EMPTY, CELL_EMPTY, CELL_OPENED_DOOR, CELL_CASE, CELL_CASE, CELL_CASE, CELL_CASE, CELL_CASE, CELL_STONE,
			CELL_EMPTY, CELL_STONE, CELL_EMPTY, CELL_STONE, CELL_EMPTY, CELL_STONE, CELL_CASE, CELL_STONE, CELL_CASE, CELL_STONE, CELL_CASE, CELL_STONE,
			CELL_EMPTY,  CELL_EMPTY, CELL_EMPTY, CELL_EMPTY, CELL_EMPTY, CELL_CASE, CELL_CASE, CELL_CASE, CELL_CASE, CELL_CASE, CELL_CASE, CELL_CASE,
			CELL_CASE, CELL_STONE, CELL_CASE, CELL_STONE, CELL_CASE, CELL_STONE, CELL_CASE, CELL_STONE, CELL_CASE, CELL_STONE, CELL_CASE, CELL_STONE,
			CELL_OPENED_DOOR | (3 << 4),  CELL_CASE, CELL_CASE, CELL_CASE, CELL_CASE, CELL_CASE, CELL_CASE, CELL_CASE, CELL_CASE, CELL_CASE, CELL_CASE, CELL_CASE,
			CELL_CASE, CELL_STONE, CELL_CASE, CELL_STONE, CELL_CASE, CELL_STONE, CELL_CASE, CELL_STONE, CELL_CASE, CELL_STONE, CELL_CASE, CELL_STONE,
			CELL_CASE,  CELL_CASE, CELL_CASE, CELL_CASE, CELL_CASE, CELL_CASE, CELL_CASE, CELL_CASE, CELL_CASE, CELL_CASE, CELL_CASE, CELL_CASE,
			CELL_CASE, CELL_STONE, CELL_CASE, CELL_STONE, CELL_CASE, CELL_STONE, CELL_CASE, CELL_STONE, CELL_CASE, CELL_STONE, CELL_CASE, CELL_STONE,
			CELL_CASE,  CELL_CASE, CELL_CASE, CELL_CASE, CELL_CASE, CELL_CASE, CELL_CASE, CELL_CASE, CELL_CASE, CELL_CASE, CELL_CASE, CELL_CASE,
			CELL_CASE, CELL_STONE, CELL_CASE, CELL_STONE, CELL_CASE, CELL_STONE, CELL_CASE, CELL_STONE, CELL_CASE, CELL_STONE, CELL_CASE, CELL_STONE,
			CELL_CASE,  CELL_CASE, CELL_CASE, CELL_CASE, CELL_CASE, CELL_CASE, CELL_CASE, CELL_CASE, CELL_CASE, CELL_CASE, CELL_CASE, CELL_CASE,
		};


		char themap3[12 * 24] = {
				CELL_PLAYER, CELL_EMPTY, CELL_EMPTY, CELL_EMPTY, CELL_EMPTY, CELL_EMPTY, CELL_EMPTY, CELL_EMPTY, CELL_EMPTY, CELL_EMPTY, CELL_EMPTY, CELL_EMPTY,
				CELL_STONE, CELL_STONE, CELL_STONE, CELL_EMPTY, CELL_STONE, CELL_EMPTY, CELL_STONE, CELL_STONE, CELL_STONE, CELL_STONE, CELL_EMPTY, CELL_EMPTY,
				CELL_EMPTY, CELL_EMPTY, CELL_EMPTY, CELL_EMPTY, CELL_STONE, CELL_CASE, CELL_STONE, CELL_EMPTY, CELL_EMPTY, CELL_STONE, CELL_EMPTY, CELL_EMPTY,
				CELL_EMPTY, CELL_EMPTY, CELL_EMPTY, CELL_EMPTY, CELL_STONE, CELL_CASE, CELL_STONE, CELL_EMPTY, CELL_EMPTY, CELL_STONE, CELL_EMPTY, CELL_EMPTY,
				CELL_EMPTY, CELL_EMPTY, CELL_EMPTY, CELL_EMPTY, CELL_STONE, CELL_CASE, CELL_STONE, CELL_EMPTY, CELL_EMPTY, CELL_STONE, CELL_EMPTY, CELL_EMPTY,
				CELL_EMPTY, CELL_EMPTY, CELL_EMPTY, CELL_EMPTY, CELL_STONE, CELL_STONE, CELL_STONE, CELL_EMPTY, CELL_EMPTY, CELL_STONE, CELL_EMPTY, CELL_EMPTY,
				CELL_EMPTY, CELL_EMPTY, CELL_EMPTY, CELL_EMPTY, CELL_EMPTY, CELL_EMPTY, CELL_EMPTY , CELL_EMPTY, CELL_EMPTY, CELL_STONE,  CELL_EMPTY, CELL_EMPTY,
				CELL_EMPTY, CELL_TREE, CELL_CASE, CELL_TREE, CELL_EMPTY, CELL_EMPTY, CELL_EMPTY, CELL_EMPTY,  CELL_EMPTY, CELL_STONE,  CELL_EMPTY, CELL_EMPTY,
				CELL_EMPTY, CELL_TREE, CELL_TREE, CELL_TREE, CELL_EMPTY, CELL_EMPTY, CELL_EMPTY, CELL_EMPTY, CELL_EMPTY,  CELL_STONE,  CELL_EMPTY, CELL_EMPTY,
				CELL_EMPTY, CELL_EMPTY, CELL_EMPTY, CELL_EMPTY, CELL_EMPTY, CELL_EMPTY, CELL_EMPTY, CELL_EMPTY, CELL_EMPTY, CELL_STONE,  CELL_EMPTY, CELL_EMPTY,
				CELL_CASE, CELL_STONE, CELL_STONE, CELL_STONE, CELL_STONE, CELL_STONE, CELL_STONE, CELL_STONE, CELL_STONE, CELL_STONE,  CELL_CASE_LIFE, CELL_EMPTY,
				CELL_EMPTY,  CELL_EMPTY, CELL_EMPTY, CELL_EMPTY, CELL_EMPTY, CELL_EMPTY, CELL_EMPTY, CELL_EMPTY, CELL_EMPTY, CELL_EMPTY, CELL_EMPTY, CELL_EMPTY,
				CELL_EMPTY, CELL_EMPTY, CELL_EMPTY, CELL_EMPTY, CELL_EMPTY, CELL_EMPTY, CELL_EMPTY, CELL_EMPTY, CELL_EMPTY, CELL_EMPTY, CELL_EMPTY, CELL_EMPTY,
				CELL_STONE, CELL_STONE, CELL_STONE, CELL_EMPTY, CELL_STONE, CELL_EMPTY, CELL_STONE, CELL_STONE, CELL_STONE, CELL_STONE, CELL_EMPTY, CELL_EMPTY,
				CELL_EMPTY, CELL_EMPTY, CELL_EMPTY, CELL_EMPTY, CELL_STONE, CELL_CASE, CELL_STONE, CELL_EMPTY, CELL_EMPTY, CELL_STONE, CELL_EMPTY, CELL_EMPTY,
				CELL_EMPTY, CELL_EMPTY, CELL_EMPTY, CELL_EMPTY, CELL_STONE, CELL_CASE, CELL_STONE, CELL_EMPTY, CELL_EMPTY, CELL_STONE, CELL_EMPTY, CELL_EMPTY,
				CELL_EMPTY, CELL_EMPTY, CELL_EMPTY, CELL_EMPTY, CELL_STONE, CELL_CASE, CELL_STONE, CELL_EMPTY, CELL_EMPTY, CELL_STONE, CELL_EMPTY, CELL_EMPTY,
				CELL_EMPTY, CELL_EMPTY, CELL_EMPTY, CELL_EMPTY, CELL_STONE, CELL_STONE, CELL_STONE, CELL_EMPTY, CELL_EMPTY, CELL_STONE, CELL_EMPTY, CELL_EMPTY,
				CELL_EMPTY, CELL_EMPTY, CELL_EMPTY, CELL_EMPTY, CELL_EMPTY, CELL_EMPTY, CELL_EMPTY , CELL_EMPTY, CELL_EMPTY, CELL_STONE,  CELL_EMPTY, CELL_EMPTY,
				CELL_EMPTY, CELL_TREE, CELL_CASE, CELL_TREE, CELL_EMPTY, CELL_EMPTY, CELL_EMPTY, CELL_EMPTY,  CELL_EMPTY, CELL_STONE,  CELL_EMPTY, CELL_EMPTY,
				CELL_EMPTY, CELL_TREE, CELL_TREE, CELL_TREE, CELL_EMPTY, CELL_EMPTY, CELL_EMPTY, CELL_EMPTY, CELL_EMPTY,  CELL_STONE,  CELL_EMPTY, CELL_EMPTY,
				CELL_EMPTY, CELL_EMPTY, CELL_EMPTY, CELL_EMPTY, CELL_EMPTY, CELL_EMPTY, CELL_EMPTY, CELL_EMPTY, CELL_EMPTY, CELL_STONE,  CELL_EMPTY, CELL_EMPTY,
				CELL_CASE, CELL_STONE, CELL_STONE, CELL_STONE, CELL_STONE, CELL_STONE, CELL_STONE, CELL_STONE, CELL_STONE, CELL_STONE,  CELL_CASE_LIFE, CELL_EMPTY,
				CELL_EMPTY,  CELL_EMPTY, CELL_EMPTY, CELL_EMPTY, CELL_EMPTY, CELL_EMPTY, CELL_EMPTY, CELL_EMPTY, CELL_EMPTY, CELL_EMPTY, CELL_EMPTY, CELL_EMPTY
			};

		switch(n) {
		case 1:
			for (int i = 0; i < map_width * map_height; i++)
				map->grid[i] = themap1[i];
			break;
		case 2:
			for (int i = 0; i < map_width * map_height; i++)
				map->grid[i] = themap2[i];
			break;
		case 3:
			for (int i = 0; i < map_width * map_height; i++)
				map->grid[i] = themap3[i];
			break;
		}
	return map;
}

void map_insert_monster(struct map* map, int x, int y, s_type type, void* data)
{
	map->monstersList = list_insert_back(map->monstersList, x, y, type, data);
}

void map_insert_bomb(struct map* map, int x, int y, s_type type, void* data)
{
	map->bombsList = list_insert_back(map->bombsList, x, y, type, data);
}
