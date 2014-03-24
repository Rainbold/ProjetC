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
	float timer; // timer used during the animations
	int range; // bomb's range
	int range_dir[4]; // Real range
	int state; //  0 = waiting, 1 = exploding
};

void bomb_init(struct game* game, struct map* map, int x, int y, b_type type, int range) {
	struct bomb* bomb = malloc( sizeof(*bomb) );
	bomb->type = type;
	bomb->curAnim = ANIM_1;
	bomb->timer = game_get_real_ticks(game);
	bomb->range = range;

	for(int i = 0; i <= 3; i ++){
		bomb->range_dir[i] = bomb->range;

	}
	bomb-> state = 0; // 0 = waiting

	s_type typeL = LIST_BOMB;
	map_insert_bomb(map, x, y, typeL, bomb);
}

void bomb_plant(struct game* game, struct map* map, struct player* player) {
	int x = player_get_x(player);
	int y = player_get_y(player);

	if(player_get_nb_bomb(player) && map_get_cell_type(map, x, y) != CELL_BOMB) { // if player has at least one bomb and he is not on a bomb

		bomb_init(game, map, x, y, BOMB_NORMAL, player_get_nb_range(player));
		map_set_cell_type(map, x, y, CELL_BOMB);
		player_dec_nb_bomb(player);
	}
}

void bomb_display(struct game* game, struct map* map, struct player* player) {
	assert(map);
	assert(player);
	struct list* bList = map_get_bombs(map);
	struct list* l;
	struct bomb* bomb = NULL;
	struct bomb* ldata;
	int x;
	int y;
	int cellType = 0;

	if(bList != NULL){ // if there is at least one bomb
		bomb_update(game, map, player);

		bList = map_get_bombs(map);
		l = map_get_bombs(map);
		// Bomb Display And Event
		while(bList != NULL) { // For each bombs
			bomb = bList->data;
			if(!(bomb->state)){ // bomb who's waiting
					window_display_image(
						sprite_get_bomb(bomb->curAnim),
						bList->x * SIZE_BLOC, bList->y * SIZE_BLOC);
			}
			else { // bomb who's exploding
				// Center
				window_display_image(
					sprite_get_bomb(bomb->curAnim),
					bList->x * SIZE_BLOC, bList->y * SIZE_BLOC);

				for(int d = 0; d < 4; d++){ // 0: NORTH, 1: SOUTH, 2: WEST, 3: EAST
					for(int r = 1; r <= bomb->range_dir[d]; r++){
/* d |dx |dy */ 		x = bList->x + r * ((d - 3) / 2 * ((d * 2) - 1)); // + r * dx
/* 0 | 1 | 0 */			y = bList->y + r * ((d / 2) * ((d * 2) - 5));	  // + r * dy
/* 1 |-1 | 0 */			l = map_get_bombs(map);
/* 2 | 0 |-1 */			window_display_image(
/* 3 | 0 | 1 */				sprite_get_bomb(bomb->curAnim),
							x * SIZE_BLOC, y * SIZE_BLOC);

						// Event

						// If the player is standing on the bomb, he loses a life
						if(bList->x == player_get_x(player) && bList->y == player_get_y(player))
							player_dec_nb_life(player, game);

						cellType = map_get_cell_type(map, x, y);
						switch(cellType){
						case CELL_PLAYER : // Player
							if(x == player_get_x(player) && y == player_get_y(player))
								player_dec_nb_life(player, game);
							break;
						case CELL_MONSTER : // Monster
							map_set_monsters( map, monster_dec_nb_life(map_get_monsters(map), x, y, game) );
							break;
						case CELL_CASE : // Case
							map_case_destroyed(game, map, x, y);
							break;
						case CELL_BOMB : // Bomb
							while(l != NULL) {
								if(l->x == x && l->y == y) {
									ldata = l->data;
									ldata->timer =  game_get_real_ticks(game) - 4000.f;
								}
								l = l->next;
							}
							break;
						default: // Everything else
							break;
						}
					}
				}
			}
			bList = bList->next;
		}
	}
}

void bomb_update(struct game* game, struct map* map, struct player* player) {
	assert(map);
	assert(player);
	struct list* bList = map_get_bombs(map);
	struct bomb* bomb = NULL;
	int reload = 0;

	while(bList != NULL) {
		bomb = bList->data;
		if(!(bomb->state)) { // If the bomb has not exploded yet we display the animation of the bomb...
			// Bomb's animation
			if(game_get_real_ticks(game) - bomb->timer < 1000.f) {
				bomb->curAnim = ANIM_1;
			} else if(game_get_real_ticks(game) - bomb->timer < 2000.f) {
				bomb->curAnim = ANIM_2;
			} else if(game_get_real_ticks(game) - bomb->timer < 3000.f) {
				bomb->curAnim = ANIM_3;
			} else if(game_get_real_ticks(game) - bomb->timer < 4000.f) {
				bomb->curAnim = ANIM_4;
			} else {
				bomb->curAnim = ANIM_5;
				bomb->state = 1;
				bomb_explo_init(map, player, bList);
				player_inc_nb_bomb(player);
			}
		}
		else{ // The bomb is exploding
			if(game_get_real_ticks(game) - bomb->timer > 5000.f) {
				bomb_free(map, bList);
				reload = 1;
			}
		}
		if(reload){
			bList = map_get_bombs(map);
			reload = 0;
		}
		else {
			bList = bList->next;
		}
	}
}

void bomb_explo_init(struct map* map, struct player* player, struct list* bList){ // When the bomb explode, used once per bomb
	assert(bList);
	struct bomb* bomb = bList->data;
	int cellType = 0;
	int rt = 0;
	int x;
	int y;

	if(map_get_cell_type(map, bList->x, bList->y) != CELL_PLAYER)
		map_set_cell_type(map, bList->x, bList->y, CELL_EMPTY);

	for(int d = 0; d < 4; d++){ // 0: NORTH, 1: SOUTH, 2: WEST, 3: EAST
		for(int r = 1; r <= bomb->range; r++){
			x = bList->x + r * ((d - 3) / 2 * ((d * 2) - 1)); // + r * dx
			y = bList->y + r * ((d / 2) * ((d * 2) - 5));	  // + r * dy
			//printf("\nd: %d, x: %d, y: %d, dx: %d, dy: %d", d, x, y, ((d - 3) / 2 * ((d * 2) - 1)), ((d / 2) * ((d * 2) - 5)));

			if(0 <= x  && x < map_get_width(map) && 0 <= y && y < map_get_height(map)) { // if in map
				rt = r;
				cellType = map_get_cell_type(map, x, y);

				switch(cellType){ // Looking for something stopping the fire
				case CELL_GOAL:
				case CELL_SCENERY:
				case CELL_KEY:
				case CELL_DOOR:
				case CELL_CLOSED_DOOR:
					rt--;
					/* no break */
				case CELL_CASE:
				case CELL_BONUS:
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
	map_set_bombs(map, list_remove(map_get_bombs(map), bList->x, bList->y));
}
