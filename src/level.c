#include <assert.h>
#include <stdlib.h>
#include <stddef.h> // NULL (macos)
#include <level.h>
#include <game.h> 
#include <map.h>
#include <list.h>

struct level {
	struct map** maps; // array of the level's maps
	short nb_maps; // nb maps of the level
	short cur_map; // the current map
};

struct level* level_get_level(int num, struct game* game) {
	struct level* level = malloc(sizeof(*level));
	switch (num) {
	case 0: // lvl 0
		level->nb_maps = 2;
		level->cur_map = 0;

		level->maps = malloc(sizeof(*level->maps) * level->nb_maps);

		level->maps[0] = map_get_default(1);
		level->maps[1] = map_get_default(2);

		map_load_monsters(level->maps[0], game);
		return level;
		break;
	case 1: // lvl 1
		level->nb_maps = 1;
		level->cur_map = 0;

		level->maps = malloc(sizeof(*level->maps) * level->nb_maps);

		level->maps[0] = map_get_default(3);

		map_load_monsters(level->maps[0], game);
		return level;
		break;
	}
	return NULL ;
}

void level_change_map(struct game* game, struct player* player, struct map* map, unsigned char num) {
	struct level* level = game_get_curr_level(game);

	struct list* bList = map_get_bombs(map);

	assert(game);
	assert(player);
	assert(map);

	while(bList != NULL) {
		bList = list_cut_head(bList);
		player_inc_nb_bomb(player);
	}

	player_reset_way_mov(player);

	if(num <= level->nb_maps) {
		level->cur_map = num;
		map_load_monsters(map, game);
		printf("Next map\n");
	}
	else {
		level_free(level);
		level = game_next_lvl(game);
		printf("Next level\n");
	}
	player_from_map(player, level->maps[level->cur_map]);
	window_resize(map_get_width(level->maps[level->cur_map]), map_get_height(level->maps[level->cur_map]));
}

struct map* level_get_curr_map(struct level* level) {
	return level->maps[level->cur_map];
}

struct map* level_get_map(struct level* level, int num) {
	assert(num <= level->nb_maps);
	return level->maps[num];
}

void level_free(struct level* level) {
	for (int i = 0; i < level->nb_maps; i++)
		map_free(level->maps[i]);

	free(level->maps);
}

void level_display(struct level* level) {
	map_display(level->maps[level->cur_map]);
}
