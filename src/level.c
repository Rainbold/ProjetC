#include <file.h>
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
	int map_count = 0;
	int nb_players = game_get_nb_player(game);

	if(nb_players >= 2) {
		level->nb_maps = sprite_get_nb_map_multi();
		level->cur_map = 0;
		level->maps = malloc(sizeof(*level->maps) * level->nb_maps);
		for(int i = 0; i < level->nb_maps; i++)
			level->maps[i] = file_load_map_multi(i, nb_players);
	}
	else {
		for(int i = 0; i < 8; i++) {
			if(file_map_exist(num, i))
				map_count++;
			else
				i = 8;
		}

		if(map_count == 0)
			return NULL;

		level->nb_maps = map_count;
		level->cur_map = 0;

		level->maps = malloc(sizeof(*level->maps) * level->nb_maps);

		for(int i = 0; i < level->nb_maps; i++) {
			level->maps[i] = file_load_map(num, i);
		}

		map_load_monsters(level->maps[0], game);
	} // end else

	window_resize(map_get_width(level_get_curr_map(level)) * SIZE_BLOC, map_get_height(level_get_curr_map(level)) * SIZE_BLOC + BANNER_HEIGHT + LINE_HEIGHT);

	return level;
}

void level_change_map(struct game* game, struct player* player, struct map* map, unsigned char num) {
	struct level* level = game_get_curr_level(game);

	struct list* bList = map_get_bombs(map);

	assert(game);
	assert(player);
	assert(map);

	while(bList != NULL) {
		map_set_cell_type(map, list_get_x(bList), list_get_y(bList), CELL_EMPTY);
		bList = list_cut_head(bList);
		player_inc_nb_bomb(player);
	}

	map_set_bombs(map, NULL);

	player_reset_way_mov(player);

	if(num < level->nb_maps) {
		level->cur_map = num;
		map_load_monsters(map, game);
		printf("Next map\n");
	}
	else {
		level_free(level);
		level = game_next_lvl(game);
		if(level == NULL) {
			printf("END of GAME\n");
			exit(EXIT_SUCCESS);
		}
		else {
		printf("Next level\n");
		}
	}
	players_from_map(game, level->maps[level->cur_map]);
	window_resize(map_get_width(level_get_curr_map(level)) * SIZE_BLOC, map_get_height(level_get_curr_map(level)) * SIZE_BLOC + BANNER_HEIGHT + LINE_HEIGHT);
}

struct map* level_get_curr_map(struct level* level) {
	return level->maps[level->cur_map];
}

void level_set_cur_map(struct level* level, int i) {
	level->cur_map = i;
}

int level_get_curr_nb_map(struct level* level) {
	assert(level);
	return(level->cur_map);
}

struct map* level_get_map(struct level* level, int num) {
	assert(num <= level->nb_maps);
	return level->maps[num];
}

void level_free(struct level* level) {
	for (int i = 0; i < level->nb_maps; i++)
		map_free(level->maps[i]);
	free(level->maps);
	free(level);
}

void level_display(struct level* level) {
	map_display(level->maps[level->cur_map], 0, 0);
}
