#include <SDL/SDL_image.h>
#include <assert.h>

#include <player.h>
#include <sprite.h>
#include <window.h>
#include <misc.h>
#include <constant.h>

struct player {
	int x, y;
	enum way current_way;
	int nb_bomb;
	int nb_life;
	int nb_range;
	float timer;
};

struct player* player_init(int bomb_number, int life_number, int range_number) {
	struct player* player = malloc(sizeof(*player));
	if (!player)
		error("Memory error");

	player->current_way = SOUTH;
	player->nb_bomb = bomb_number;
	player->nb_life = life_number;
	player->nb_range = range_number;
	player->timer = SDL_GetTicks();

	return player;
}
 
void player_free(struct player* player) {
	assert(player);
	free(player);
}

int player_get_x(struct player* player) {
	assert(player != NULL);
	return player->x;
}

int player_get_y(struct player* player) {
	assert(player != NULL);
	return player->y;
}

void player_set_current_way(struct player* player, enum way way) {
	assert(player);
	player->current_way = way;
}

int player_get_nb_bomb(struct player* player) { // get nb_bomb
	assert(player);
	return player->nb_bomb;
}

void player_inc_nb_bomb(struct player* player) { // nb_bomb++
	assert(player);
	if(player_get_nb_bomb(player) < 9)
		player->nb_bomb += 1;
}

void player_dec_nb_bomb(struct player* player) { // nb_bomb--
	assert(player);
	if(player_get_nb_bomb(player) > 0)
		player->nb_bomb -= 1;
}

int player_get_nb_life(struct player* player) { // get nb_life
	assert(player);
	return player->nb_life;
}

void player_inc_nb_life(struct player* player) { // nb_life++
	assert(player);
	if(player_get_nb_life(player) < 9)
		player->nb_life += 1;
}

void player_dec_nb_life(struct player* player) { // nb_life-- TODO gameover if nb_life <= 0
	assert(player);
	if(player_get_nb_life(player) > 0 && SDL_GetTicks() - player->timer >= 4000.f) {
		player->nb_life -= 1;
		player->timer = SDL_GetTicks();
	}
}

int player_get_nb_range(struct player* player) { // get nb_range
	assert(player);
	return player->nb_range;
}

void player_inc_nb_range(struct player* player) { // nb_range++
	assert(player);
	if(player_get_nb_range(player) < 9)
		player->nb_range += 1;
}

void player_dec_nb_range(struct player* player) { // nb_range--
	assert(player);
	if(player_get_nb_range(player) > 0)
		player->nb_range -= 1;
}

void player_from_map(struct player* player, struct map* map) {
	assert(player);
	assert(map);

	int i, j;
	for (i = 0; i < map_get_width(map); i++) {
		for (j = 0; j < map_get_height(map); j++) {
			if (map_get_cell_type(map, i, j) == CELL_PLAYER) {
				player->x = i;
				player->y = j;
			}
		}
	}
}

static int player_move_aux(struct player* player, struct map* map, int x, int y) {
	int cellType = 0;

	if (!map_is_inside(map, x, y))
		return 0;
	
	switch (map_get_cell_type(map, x, y)) {
	case CELL_SCENERY:
		return 0; // stop the function and the player movements
		break;

	case CELL_CASE: 
		switch(player->current_way)
		{
			case NORTH:
			if(y < 1)
				return 0;
			else {
				cellType = map_get_cell_type(map, x, y - 1);
				if( cellType != CELL_EMPTY || y < 1)
					return 0;
				map_set_cell_type(map, x, y, CELL_EMPTY);
				map_set_cell_type(map, x, y - 1, CELL_CASE);
			}
			break;

			case SOUTH:
			if(y >= map_get_height(map) - 1)
				return 0;
			else {
				cellType = map_get_cell_type(map, x, y + 1);
				if( cellType != CELL_EMPTY)
					return 0;
				map_set_cell_type(map, x, y, CELL_EMPTY);
				map_set_cell_type(map, x, y + 1, CELL_CASE);
			}
			break;

			case EAST:
			if(x >= map_get_width(map) - 1)
				return 0;
			else {
				cellType = map_get_cell_type(map, x + 1, y);
				if( cellType != CELL_EMPTY)
					return 0;
				map_set_cell_type(map, x, y, CELL_EMPTY);
				map_set_cell_type(map, x + 1, y, CELL_CASE);
			}
			break;

			case WEST:
			if(x < 1)
				return 0;
			else {
				cellType = map_get_cell_type(map, x - 1, y);
				if( cellType != CELL_EMPTY)
					return 0;
				map_set_cell_type(map, x, y, CELL_EMPTY);
				map_set_cell_type(map, x - 1, y, CELL_CASE);
			}
			break;
		}
		break;

	case CELL_BONUS: // todo : bonus

		break;

	case CELL_GOAL: // todo : goal
		break;

	case CELL_MONSTER: // todo : monster
		break;

	case CELL_PLAYER: // todo : you win
		break;

	default:
		break;
	}

	// Player has moved
	return 1;
}

int player_move(struct player* player, struct map* map) {
	int x = player->x;
	int y = player->y;
	int move = 0;

	switch (player->current_way) {
	case NORTH:
		if (player_move_aux(player, map, x, y - 1)) {
			player->y--;
			move = 1;
		}
		break;

	case SOUTH:
		if (player_move_aux(player, map, x, y + 1)) {
			player->y++;
			move = 1;
		}
		break;

	case WEST:
		if (player_move_aux(player, map, x - 1, y)) {
			player->x--;
			move = 1;
		}
		break;

	case EAST:
		if (player_move_aux(player, map, x + 1, y)) {
			player->x++;
			move = 1;
		}
		break;
	}

	if (move) {
		map_set_cell_type(map, x, y, CELL_EMPTY);
		map_set_cell_type(map, player->x, player->y, CELL_PLAYER);
	}
	return move;
}

void player_display(struct player* player) {
	assert(player);
	window_display_image(sprite_get_player(player->current_way),
			player->x * SIZE_BLOC, player->y * SIZE_BLOC);
}
