#include <bomb.h>
#include <list.h>
#include <assert.h>
#include <monster.h>
#include <player.h>
#include <map.h>
#include <game.h>


struct bomb {
	b_type type;
	b_curAnim curAnim;
	struct player* player;
	int range; // bomb's range
	int range_dir[4]; // Real range
	int state; //  0 = waiting, 1 = exploding
	int anim; // used as a timer
};

void bomb_init(struct game* game, struct map* map, int x, int y, b_type type, int range, struct player* player) {
	struct bomb* bomb = malloc( sizeof(*bomb) );
	bomb->type = type;
	bomb->range = range;

	for(int i = 0; i <= 3; i ++){
		bomb->range_dir[i] = bomb->range;
	}

	bomb->state = 0; // 0 = waiting
	bomb->anim = 0;
	bomb->player = player;

	s_type typeL = LIST_BOMB;
	map_insert_bomb(map, x, y, typeL, bomb);
}

void bomb_plant(struct game* game, struct map* map, struct player* player) {
	int x = player_get_x(player);
	int y = player_get_y(player);

	if(player_get_nb_life(player) && player_get_nb_bomb(player) && map_get_cell_type(map, x, y) != CELL_BOMB && map_get_cell_type(map, x, y) != CELL_DOOR) { // if player has at least one bomb and he is not on a bomb
		bomb_init(game, map, x, y, BOMB_NORMAL, player_get_nb_range(player), player);
		map_set_cell_type(map, x, y, CELL_BOMB);
		player_dec_nb_bomb(player);
	}
}

void bomb_event(struct game* game, struct map* map, struct bomb* bomb, int x, int y) {
	struct player** players = game_get_players(game);
	struct player* player = NULL;
	struct bomb* sbomb = NULL;

	for(int i = 0; i < game_get_nb_player(game); i++) {
		player = players[i];
		if((x == player_get_x(player) && y == player_get_y(player)))
			player_hit(player, 3*DEFAULT_GAME_FPS);
	}

	int cellType = map_get_cell_type(map, x, y);
	switch(cellType & 15){
	case CELL_BONUS : // Case
		map_set_cell_type(map, x, y, CELL_EMPTY);
		break;
	case CELL_MONSTER : // Monster
		map_set_monsters( map, monster_hit(map_get_monsters(map), x, y, game) );
		break;
	case CELL_CASE : // Case
		if(bomb->anim >= DEFAULT_GAME_FPS)
			map_case_destroyed(game, map, x, y);
		break;
	case CELL_BOMB : // Bomb
		if((sbomb = search_bomb(map, x, y, 0)) != NULL)
				sbomb->anim = 4 * DEFAULT_GAME_FPS;
		break;
	default: // Everything else
		break;
	}
}

void bomb_display(struct game* game, struct map* map) {
	assert(game);
	assert(map);

	struct list* bList = map_get_bombs(map);
	struct bomb* bomb = NULL;
	int x;
	int y;

	if(bList != NULL){ // if there is at least one bomb

		// Bomb Display And Event
		while(bList != NULL) { // For each bombs
			bomb = list_get_data(bList);
			if(!(bomb->state)){ // bomb who's waiting
				window_display_sprite(
						sprite_get_bombs(),
						sprite_get_rect_bomb_anim(7, ((bomb->anim)/4)%4),
						list_get_x(bList) * SIZE_BLOC, list_get_y(bList) * SIZE_BLOC);
			}
			else { // bomb who's exploding
				//printf("anim: %d, cal: %d\n", bomb->anim, bomb->anim/4);

				// Center
				window_display_sprite(
						sprite_get_bombs(),
						sprite_get_rect_bomb_anim(0, bomb->anim / 4),
						list_get_x(bList) * SIZE_BLOC, list_get_y(bList) * SIZE_BLOC);
				// Event
				bomb_event(game, map, bomb, list_get_x(bList), list_get_y(bList));

				for(int d = 0; d < 4; d++){ // 0: SOUTH, 1: NORTH, 2: WEST, 3: EAST
					for(int r = 1; r <= bomb->range_dir[d]; r++){
/* d |dx |dy */ 		x = list_get_x(bList) + r * ((d / 2) * ((d * 2) - 5)); 		// + r * dx
/* 0 | 0 | 1 */			y = list_get_y(bList) + r * ((d - 3) / 2 * ((d * 2) - 1));	// + r * dy
/* 1 | 0 |-1 */			if(list_find(map_get_bombs(map), x, y) == NULL) {
/* 2 |-1 | 0 */				if(r != bomb->range) {
/* 3 | 1 | 0 */					window_display_sprite(sprite_get_bombs(), sprite_get_rect_bomb_anim(d/2 +1, bomb->anim/4), x * SIZE_BLOC, y * SIZE_BLOC);
							}
							else {
								window_display_sprite(sprite_get_bombs(), sprite_get_rect_bomb_anim(d+3, bomb->anim/4), x * SIZE_BLOC, y * SIZE_BLOC);
							}
						}
						// Event
						bomb_event(game, map, bomb, x, y);
					} // end for (range)
				} // end for (direction)
			}
			bList = list_get_next(bList);
		}
	}
}

void bomb_update(struct map* map) {
	assert(map);

	struct list* bList = map_get_bombs(map);
	struct bomb* bomb = NULL;
	int reload = 0;

	while(bList != NULL) {
		bomb = list_get_data(bList);
		if(!(bomb->state)) { // If the bomb has not exploded yet we display the animation of the bomb...
			// Bomb's animation
			if(bomb->anim < 4 * DEFAULT_GAME_FPS)
				bomb->anim++;
			else {
				bomb->state = 1;
				bomb_explo_init(map, bList);
				player_inc_nb_bomb(bomb->player);
				bomb->anim = 0;
			}
		}
		else{ // The bomb is exploding
			if(bomb->anim >= 1 * DEFAULT_GAME_FPS) {
				bomb_free(map, bList);
				reload = 1;
			}
			else {
				//if(bomb->anim >= 1 * DEFAULT_GAME_FPS)
					bomb->anim++;
			}
		}

		if(reload){
			bList = map_get_bombs(map);
			reload = 0;
		}
		else {
			bList = list_get_next(bList);
		}
	} // end while
}

void bomb_explo_init(struct map* map, struct list* bList){ // When the bomb explode, used once per bomb
	assert(bList);
	struct bomb* bomb = list_get_data(bList);
	int cellType = 0;
	int rt = 0;
	int x;
	int y;

	if(map_get_cell_type(map, list_get_x(bList), list_get_y(bList)) == CELL_BOMB)
		map_set_cell_type(map, list_get_x(bList), list_get_y(bList), CELL_EMPTY);

	for(int d = 0; d < 4; d++){ // 0: NORTH, 1: SOUTH, 2: WEST, 3: EAST
		for(int r = 1; r <= bomb->range; r++){
			x = list_get_x(bList) + r * ((d / 2) * ((d * 2) - 5));		// + r * dx
			y = list_get_y(bList) + r * ((d - 3) / 2 * ((d * 2) - 1));	// + r * dy
			//printf("\nd: %d, x: %d, y: %d, dx: %d, dy: %d", d, x, y, ((d - 3) / 2 * ((d * 2) - 1)), ((d / 2) * ((d * 2) - 5)));

			if(0 <= x  && x < map_get_width(map) && 0 <= y && y < map_get_height(map)) { // if in map
				rt = r;
				cellType = map_get_cell_type(map, x, y);

				switch(cellType){ // Looking for something stopping the fire
				case CELL_GOAL:
				case CELL_SCENERY:
				case CELL_KEY:
					rt--;
					/* no break */
				case CELL_CASE:
				case CELL_BONUS:
				case CELL_BOMB:
					r = bomb->range + 1;
					break;
				default:
					break;
				} // end switch
			} // end if in map
		} // end range
		// Set range_dir
		bomb->range_dir[d]= rt;
		rt = 0;
	}
}

void bomb_free(struct map* map,struct list* bList){
	assert(map);
	assert(bList);
	map_set_bombs(map, list_remove2(map_get_bombs(map), bList));
}

struct bomb* search_bomb(struct map* map, int x, int y, int state) {
	struct list* bList = map_get_bombs(map);
	while(bList != NULL) {
		if(list_get_x(bList) == x && list_get_y(bList) == y) {
			struct bomb* bomb = list_get_data(bList);
			if(bomb->state == state)
				return list_get_data(bList);
		}
		bList = list_get_next(bList);
	}
	return NULL;
}
