#include <SDL/SDL.h>
#include <SDL/SDL_image.h>
#include <assert.h>

#include <player.h>
#include <map.h>
#include <sprite.h>
#include <window.h>
#include <misc.h>
#include <constant.h>
#include <bomb.h>
#include <monster.h>

struct bomb {
	int x, y;
	enum currentAnimation curAnimBomb;
	float timer; // timer used during the animations
	int range; // bomb's range
	int range_dir[4];
	int state;
};

void bomb_init(struct bomb* bomb, struct player* player) {
	bomb->x = player_get_x(player);
	bomb->y = player_get_y(player);
	bomb->timer = SDL_GetTicks(); // The time is stored when the bomb is created
	bomb->curAnimBomb = ANIM_1;
	bomb->range = player_get_nb_range(player);
	for(int i = 0; i <= 3; i ++){
		bomb->range_dir[i] = bomb->range;
	}
	bomb->state = 0; // 0 : waiting, 1 : exploding
}
int bomb_get_x(struct bomb* bomb) {
	assert(bomb);
	return bomb->x;
}

int bomb_get_y(struct bomb* bomb) {
	assert(bomb);
	return bomb->y;
}

float bomb_get_timer(struct bomb* bomb) {
	assert(bomb);
	return bomb->timer;
}

void bomb_set_timer(struct bomb* bomb, float timer) {
	assert(bomb);
	bomb->timer = timer;
}

// Initialization of the bomb
void bomb_plant(struct map* map, struct player* player){
	assert(map);
	assert(player);
	if(player_get_nb_bomb(player)) { // if player has at least one bomb
		struct bomb* bomb = NULL;
		for(int i = 0; i < MAX_BOMB; i++){ // Checking if the player is not standing on a bomb already
			bomb = map_get_bomb(map, i);
			if(bomb != NULL) {
			if(bomb->x == player_get_x(player) && bomb->y == player_get_y(player))
				return;
			}
		}
		for(int i = 0; i < MAX_BOMB; i++){
			if(map_get_bomb(map, i) == NULL){
				bomb = malloc( sizeof(*bomb) );
				bomb_init(bomb, player);
				player_dec_nb_bomb(player);
				map_set_cell_type(map, bomb->x, bomb->y, CELL_BOMB);
				map_set_bomb(map, bomb, i);
				map_inc_bomb_counter(map);
				i = MAX_BOMB + 1;
			}
		}
	}
}

void bomb_display(struct map* map, struct player* player, struct bomb* bombs[]) {

	assert(map);
	assert(player);
	assert(bombs);

	if(map_get_bomb_counter(map) != 0){ // if there is at least one bomb
		bomb_update(map, player, bombs);

		// bomb_display
		for(int i = 0; i < MAX_BOMB; i++){ // For each bomb in bombs[]
			if(bombs[i] != NULL){ // if there is a bomb...
				if(!(bombs[i]->state)){ // bomb who's waiting
					window_display_image(
						sprite_get_bomb(bombs[i]->curAnimBomb),
						bombs[i]->x * SIZE_BLOC, bombs[i]->y * SIZE_BLOC);
				}
				else { // bomb who's exploding
					// Center
					window_display_image(
						sprite_get_bomb(bombs[i]->curAnimBomb),
						bombs[i]->x * SIZE_BLOC, bombs[i]->y * SIZE_BLOC);

					for(int dx = -1; dx <= 1; dx += 2 ) { // For every direction in x
						for(int r = 1; r <= bombs[i]->range_dir[dx+2]; r++){
							window_display_image(
								sprite_get_bomb(bombs[i]->curAnimBomb),
								( bombs[i]->x + r * dx ) * SIZE_BLOC, bombs[i]->y * SIZE_BLOC);
						}
					}
					for(int dy = -1; dy <= 1; dy += 2) { // For every direction in y
						for(int r = 1; r <= bombs[i]->range_dir[dy+1]; r++){
							window_display_image(
								sprite_get_bomb(bombs[i]->curAnimBomb),
								bombs[i]->x * SIZE_BLOC, ( bombs[i]->y + r * dy ) * SIZE_BLOC);
						}
					}
				}
			}
		}
	}
}


void bomb_update(struct map* map, struct player* player, struct bomb* bombs[]) {

	assert(map);
	assert(player);
	assert(bombs);

	struct bomb* bomb = NULL;

	for(int i = 0; i < MAX_BOMB; i++){
		if(bombs[i] != NULL){

			bomb = bombs[i];
			if(!(bomb->state)) { // If the bomb has not exploded yet we display the animation of the bomb...

				// Bomb's animation
				if(SDL_GetTicks() - bomb_get_timer(bomb) < 1000.f) {
					bomb->curAnimBomb = ANIM_1;
				} else if(SDL_GetTicks() - bomb->timer < 2000.f) {
					bomb->curAnimBomb = ANIM_2;
				} else if(SDL_GetTicks() - bomb->timer < 3000.f) {
					bomb->curAnimBomb = ANIM_3;
				} else if(SDL_GetTicks() - bomb->timer < 4000.f) {
					bomb->curAnimBomb = ANIM_4;
				} else {
					bomb->curAnimBomb = ANIM_5;
					bomb->state = 1;
					bomb_explo_init(map, player, bomb);
					player_inc_nb_bomb(player);
				}
			}
			else{ // The bomb is exploding
				if(SDL_GetTicks() - bomb->timer > 5000.f) {
					bomb_free(bomb);
					bombs[i] = NULL;
					map_dec_bomb_counter(map);
				}
				else {
					bomb_explo_event(map, player, bomb, bombs);
				}
			}
		}
	}
}

void bomb_explo_event(struct map* map, struct player* player, struct bomb* bomb, struct bomb* bombs[]) { // While the bomb is exploding
	// If the player is standing on the bomb, he loses a life
	if(bomb->x == player_get_x(player) && bomb->y == player_get_y(player))
		player_dec_nb_life(player);

	int cellType = 0;
	for(int dx = -1; dx <= 1; dx += 1 ) { // For every direction in x
		for(int r = 1; r <= bomb->range_dir[dx+2]; r++){
			cellType = map_get_cell_type(map, (bomb->x + dx * r), bomb->y);
			switch(cellType){
			case CELL_PLAYER : // Player
				if((bomb->x + dx * r) == player_get_x(player) && bomb->y == player_get_y(player))
					player_dec_nb_life(player);
				break;
			case CELL_MONSTER : // Monster
				//map_set_monsters( map, monster_kill(map_get_monsters(map), (bomb->x + dx * r), bomb->y) );
				//map_set_cell_type(map, (bomb->x + dx * r), bomb->y, CELL_EMPTY);
				break;
			case CELL_CASE : // Case
				map_case_destroyed(map, (bomb->x + dx * r),bomb->y);
				break;
			default: // Every thing else
				// Bomb
				for(int i = 0; i < MAX_BOMB; i++){
					if(bombs[i] != NULL){
						if((bombs[i]->x == (bomb->x + dx * r)) && (bombs[i]->y == bomb->y) && (SDL_GetTicks() - bomb_get_timer(bombs[i])) < 4000.f && (bombs[i] != bomb))
							bomb_set_timer(bombs[i], SDL_GetTicks() - 4000.f);
					}
				}
				break;
			}
		}
	}
	for(int dy = -1; dy <= 1; dy += 2) { // For every direction in y
		for(int r = 1; r <= bomb->range_dir[dy+1]; r++){
			cellType = map_get_cell_type(map, bomb->x, (bomb->y + dy * r));

			switch(cellType){
			case CELL_PLAYER : // Player
				if(bomb->x == player_get_x(player) && (bomb->y + dy * r) == player_get_y(player))
					player_dec_nb_life(player);
				break;
			case CELL_MONSTER : // Monster
				//map_set_monsters( map, monster_kill(map_get_monsters(map), bomb->x, (bomb->y + dy * r)) );
				//map_set_cell_type(map, bomb->x, (bomb->y + dy * r), CELL_EMPTY);
				break;
			case CELL_CASE : // Case
				map_case_destroyed(map, bomb->x, (bomb->y + dy * r));
				break;
			default: // Every thing else
				// Bomb
				for(int i = 0; i < MAX_BOMB; i++){
					if(bombs[i] != NULL){
						if((bombs[i]->x == bomb->x) && (bombs[i]->y == (bomb->y + dy * r)) && (SDL_GetTicks() - bomb_get_timer(bombs[i])) < 4000.f && (bombs[i] != bomb))
							bomb_set_timer(bombs[i], SDL_GetTicks() - 4000.f);
					}
				}
				break;
			}
		}
	}
}

void bomb_explo_init(struct map* map, struct player* player, struct bomb* bomb){ // When the bomb explode
	int cellType = 0;
	int rt = 0;
	if(map_get_cell_type(map, bomb->x, bomb->y) != CELL_PLAYER)
		map_set_cell_type(map, bomb->x, bomb->y, CELL_EMPTY);

	for(int dx = -1; dx <= 1; dx += 2 ){ // For every direction in x
		rt = 0;
		for(int r = 1; r <= bomb->range; r++){ // range
			if(0 <= (bomb->x + dx * r) && (bomb->x + dx * r) < map_get_width(map)) {
				rt = r;
				cellType = map_get_cell_type(map, (bomb->x + dx * r), bomb->y);

				switch(cellType){ // Looking for something stopping the fire
				case CELL_GOAL :
				case CELL_SCENERY :
				case CELL_KEY :
				case CELL_DOOR :
				case CELL_CLOSED_DOOR :
					rt--;
					/* no break */
				case CELL_CASE:
				case CELL_BONUS :
					r = bomb->range + 1;
					break;
				default:
					break;
				} // end switch
			} // end if in map
		} // end range
		// Set range_dir
		bomb->range_dir[dx+2]= rt;
	}
	cellType = 0;
	for(int dy = -1; dy <= 1; dy += 2 ){ // For every direction in y
		rt = 0;
		for(int r = 1; r <= bomb->range; r++){ // range
			if(0 <= (bomb->y + dy * r) && (bomb->y + dy * r) < (map_get_height(map)) ) {
				rt = r;
				cellType = map_get_cell_type(map, bomb->x, (bomb->y + dy * r));

				switch(cellType){ // Looking for something stopping the fire
				case CELL_GOAL :
				case CELL_SCENERY :
				case CELL_KEY :
				case CELL_DOOR :
				case CELL_CLOSED_DOOR :
					rt--;
					/* no break */
				case CELL_CASE:
				case CELL_BONUS :
					r = bomb->range + 1;
					break;
				default:
					break;
				} // end switch
			} // end if in map
		} // end range
		// Set range_dir
		bomb->range_dir[dy+1]= rt;
	}
}

void bomb_free(struct bomb* bomb){
	free(bomb);
}
