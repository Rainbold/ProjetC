#include <SDL/SDL_image.h>
#include <assert.h>
#include <math.h>

#include <player.h>
#include <sprite.h>
#include <window.h>
#include <misc.h>
#include <constant.h>

struct player {
	int x, y;
	int x_sprite, y_sprite;
	way_t current_way;
	int way[4];
	int nb_bomb;
	int nb_life;
	int nb_range;
	int invicibility;
	int timer;
	int moving;
	int velocity;
	int anim;
};

struct player* player_init(int bomb_number, int life_number, int range_number) {
	struct player* player = malloc(sizeof(*player));
	if (!player)
		error("Memory error");

	player->current_way = SOUTH;
	player->way[NORTH] = 0;
	player->way[SOUTH] = 0;
	player->way[EAST] = 0;
	player->way[WEST] = 0;
	player->nb_bomb = bomb_number;
	player->nb_life = life_number;
	player->nb_range = range_number;
	player->timer = -1;
	player->invicibility = 0;
	player->moving = 0;
	player->velocity = 4;
	player->x_sprite = 0;
	player->y_sprite = 0;
	player->anim = 0;

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

void player_set_current_way(struct player* player, way_t way) {
	assert(player);
	player->current_way = way;
}

void player_set_way(struct player* player, way_t way) {
	assert(player);
	player->way[way] = 1;
}

void player_unset_way(struct player* player, way_t way) {
	assert(player);
	player->way[way] = 0;
}

void player_inc_moving(struct player* player) {
	assert(player);
	player->moving++;
}

void player_dec_moving(struct player* player) {
	assert(player);
	if(player->moving > 0)
		player->moving--;
}

int player_get_moving(struct player* player) {
	assert(player);
	return(player->moving);
}

void player_inc_velocity(struct player* player) {
	assert(player);
	player->velocity++;
}

void player_dec_velocity(struct player* player) {
	assert(player);
	if(player->velocity > 1)
		player->velocity--;
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

void player_dec_nb_life(struct player* player, struct game* game) { // nb_life-- TODO gameover if nb_life <= 0
	assert(player);
	if(player_get_nb_life(player) > 0 && (player->invicibility != 1 || player->timer == -1) ) {
		player->nb_life -= 1;
		player->timer = game_get_frame(game);
		player->invicibility = 1;
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
	if(player_get_nb_range(player) > 1)
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

static int player_move_aux(struct player* player, struct map* map, int x, int y, struct game* game) {
	int cellType = 0;
//	struct bomb* bomb;
//	struct list* bList;

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
		case CELL_BOMB:
		//	bList = map_get_bombs(map);
		//	bomb = bList->data;
		//	if(bomb->frame)
			return 0;
			break;
		case CELL_GOAL: // todo : goal
			break;
		case CELL_MONSTER:
			player_dec_nb_life(player, game);
			break;

		case CELL_PLAYER: // todo : you win
			break;

		default:
			break;
	}

	// Player has moved
	return 1;
}

int player_move(struct player* player, struct map* map, struct game* game) {
	int x = player->x;
	int y = player->y;
	int move = 0;

printf("%d%d%d%d\n", player->way[0],  player->way[1],  player->way[2],  player->way[3]);

	if(player->way[NORTH] && (player->moving == 1 || (player->x_sprite <= -15 && player_move_aux(player, map, x - 1, y - 1, game)) || (player->x_sprite >= 15 && player_move_aux(player, map, x, y - 1, game))  || player_move_aux(player, map, x, y - 1, game)  || player->y_sprite > 0)) {
		player->current_way = NORTH;
		printf("check\n)");
		if(player->x_sprite <= -15 && player_move_aux(player, map, x - 1, y - 1, game)) {
			player->x_sprite -= player->velocity;
			player->current_way = WEST;
		}
		else if(player->x_sprite >= 15 && player_move_aux(player, map, x + 1, y - 1, game)) {
			player->x_sprite += player->velocity;
			player->current_way = EAST;
		}
		else if (player_move_aux(player, map, x, y - 1, game) || player->y_sprite > 0) {
			player->y_sprite -= player->velocity;
			if(player->x_sprite > 0) {
				player->x_sprite -= player->velocity;
				if(player->x_sprite < 0)
					player->x_sprite = 0;
			}
			else if(player->x_sprite < 0) {
				player->x_sprite += player->velocity;
				if(player->x_sprite > 0)
					player->x_sprite = 0;
			}
		}
	}
	else if(player->way[SOUTH] && (player->moving == 1 || (player->x_sprite <= -15 && player_move_aux(player, map, x - 1, y + 1, game))|| (player->x_sprite >= 15 && player_move_aux(player, map, x + 1, y + 1, game)) || (player_move_aux(player, map, x, y + 1, game) || player->y_sprite < 0) )) {
		player->current_way = SOUTH;
		if(player->x_sprite <= -15 && player_move_aux(player, map, x - 1, y + 1, game)) {
			player->x_sprite -= player->velocity;
			player->current_way = WEST;
		}
		else if(player->x_sprite >= 15 && player_move_aux(player, map, x + 1, y + 1, game)) {
			player->x_sprite += player->velocity;
			player->current_way = EAST;
		}
		else if (player_move_aux(player, map, x, y + 1, game) || player->y_sprite < 0) {
			player->y_sprite += player->velocity;
			if(player->x_sprite > 0) {
				player->x_sprite -= player->velocity;
				if(player->x_sprite < 0)
					player->x_sprite = 0;
			}
			else if(player->x_sprite < 0) {
				player->x_sprite += player->velocity;
				if(player->x_sprite > 0)
					player->x_sprite = 0;
			}
		}
	}
	else if(player->way[WEST] && (player->moving == 1 || (player->y_sprite <= -15 && player_move_aux(player, map, x - 1, y - 1, game)) || (player->y_sprite >= 15 && player_move_aux(player, map, x - 1, y + 1, game)) || player_move_aux(player, map, x - 1, y, game)  || player->x_sprite > 0)) {
		player->current_way = WEST;
		if(player->y_sprite <= -15 && player_move_aux(player, map, x - 1, y - 1, game)) {
			player->y_sprite -= player->velocity;
			player->current_way = NORTH;
		}
		else if(player->y_sprite >= 15 && player_move_aux(player, map, x - 1, y + 1, game)) {
			player->y_sprite += player->velocity;
			player->current_way = SOUTH;
		}
		else if (player_move_aux(player, map, x - 1, y, game) || player->x_sprite > 0) {
			player->x_sprite -= player->velocity;
			if(player->y_sprite > 0) {
				player->y_sprite -= player->velocity;
				if(player->y_sprite < 0)
					player->y_sprite = 0;
			}
			else if(player->y_sprite < 0) {
				player->y_sprite += player->velocity;
				if(player->y_sprite > 0)
					player->y_sprite = 0;
			}
		}
	}
	else if(player->way[EAST] && (player->moving == 1 || (player->y_sprite <= -15 && player_move_aux(player, map, x + 1, y - 1, game)) || (player->y_sprite >= 15 && player_move_aux(player, map, x + 1, y + 1, game)) || player_move_aux(player, map, x + 1, y, game) || player->x_sprite < 0)) {
		player->current_way = EAST;
		if(player->y_sprite <= -15 && player_move_aux(player, map, x + 1, y - 1, game)) {
			player->y_sprite -= player->velocity;
			player->current_way = NORTH;
		}
		else if(player->y_sprite >= 15 && player_move_aux(player, map, x + 1, y + 1, game)) {
			player->y_sprite += player->velocity;
			player->current_way = SOUTH;
		}
		else if (player_move_aux(player, map, x + 1, y, game) || player->x_sprite < 0) {
			player->x_sprite += player->velocity;
			if(player->y_sprite > 0) {
				player->y_sprite -= player->velocity;
				if(player->y_sprite < 0)
					player->y_sprite = 0;
			}
			else if(player->y_sprite < 0) {
				player->y_sprite += player->velocity;
				if(player->y_sprite > 0)
					player->y_sprite = 0;
			}
		}
	}

	if(player->y_sprite < -20) {
		player->y_sprite += 40;
		player->y--;
		move = 1;
		player->current_way = NORTH;
	}
	if(player->y_sprite > 20) {
		player->y_sprite -= 40;
		player->y++;
		move = 1;
		player->current_way = SOUTH;
	}
	if(player->x_sprite < -20) {
		player->x_sprite += 40;
		player->x--;
		move = 1;
		player->current_way = WEST;
	}
	printf("x_spire: %d\n", player->x_sprite);
	if(player->x_sprite > 20) {
		player->x_sprite -= 40;
		player->x++;
		move = 1;
		player->current_way = EAST;
		printf("ok\n");
	}

	char type = map_get_cell_compose_type(map, player->x, player->y);

	if(type == (CELL_BONUS|(BONUS_LIFE << 4)))
		player_inc_nb_life(player);
	else if (type == (CELL_BONUS|(BONUS_BOMB_NB_INC << 4)))
		player_inc_nb_bomb(player);
	else if (type == (CELL_BONUS|(BONUS_BOMB_NB_DEC << 4)))
		player_dec_nb_bomb(player);
	else if (type == (CELL_BONUS|(BONUS_BOMB_RANGE_INC << 4)))
		player_inc_nb_range(player);
	else if (type == (CELL_BONUS|(BONUS_BOMB_RANGE_DEC << 4)))
		player_dec_nb_range(player);

	if (move) {
		if(map_get_cell_type(map, x, y) != CELL_BOMB && map_get_cell_type(map, x, y) != CELL_MONSTER)
			map_set_cell_type(map, x, y, CELL_EMPTY);
		map_set_cell_type(map, player->x, player->y, CELL_PLAYER);
	}
	printf("mov: %d, x: %d, y: %d, x_sprite: %d, y_sprite: %d\n", player->moving, player->x, player->y, player->x_sprite, player->y_sprite);
	return move;
}

void player_display(struct player* player, struct game* game) {
	assert(player);
	int anim;

	if( player->invicibility == 1 ) {
		if( (int)floor( (game_get_frame(game) - player->timer) )%4 == 0 )
			SDL_SetAlpha(sprite_get_players(), SDL_SRCALPHA, 128);
		else
			SDL_SetAlpha(sprite_get_players(), SDL_SRCALPHA, 192);
	}

	if( game_get_frame(game) - player->timer > DEFAULT_GAME_FPS * 3 ) {
		player->invicibility = 0;
		SDL_SetAlpha(sprite_get_players(), SDL_SRCALPHA, 255);
	}

//	window_display_image(sprite_get_player(player->current_way),
//			player->x * SIZE_BLOC + player->x_sprite, player->y * SIZE_BLOC + player->y_sprite);

	if(player->moving) {
		anim = (((game_get_frame(game) - player->anim)*(player->velocity)/12)+1)%8;
	}
	else {
		anim = 0;
	}

	window_display_sprite(	sprite_get_players(),
							sprite_get_rect_player_anim(anim, player->current_way),
							player->x * SIZE_BLOC + player->x_sprite,
							player->y * SIZE_BLOC + player->y_sprite - 20
							);
}
