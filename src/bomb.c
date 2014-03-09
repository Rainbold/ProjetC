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

struct bomb {
	int x, y;
	enum currentAnimation curAnimBomb;
	float timer; // timer used during the animations
	int range; // bomb's range
	int directions[4]; // range per direction
	int stopAnimDir[4]; // used to tell wether or not the animation is stopped in a specific direction
};

// Initilization of the bomb
struct bomb* bomb_plant(struct map* map, struct player* player)
{
	struct bomb* bomb = malloc( sizeof(*bomb) );
	bomb->x = player_get_x(player);
	bomb->y = player_get_y(player);
	player_dec_nb_bomb(player);

	bomb->timer = SDL_GetTicks(); // The time is stored when the bomb is created

	bomb->curAnimBomb = ANIM_4;
	bomb->range = player_get_nb_range(player);
	bomb->directions[NORTH] = 1;
	bomb->directions[SOUTH] = 1;
	bomb->directions[WEST] = 1;
	bomb->directions[EAST] = 1;
	
	bomb->stopAnimDir[NORTH] = 0;
	bomb->stopAnimDir[SOUTH] = 0;
	bomb->stopAnimDir[WEST] = 0;
	bomb->stopAnimDir[EAST] = 0;

	map_set_cell_type(map, bomb->x, bomb->y, CELL_BOMB);

	return bomb;
}

void bomb_display(struct map* map, struct player* player, struct bomb* bomb, struct bomb* bombs[], int bombCounter) 
{
	assert(bomb);
	
	int anim = 1; 
	
	// Bomb's animation
	if(SDL_GetTicks() - bomb->timer < 1000.f) {
		bomb->curAnimBomb = ANIM_1;
	} else if(SDL_GetTicks() - bomb->timer < 2000.f) {
		bomb->curAnimBomb = ANIM_2;
	} else if(SDL_GetTicks() - bomb->timer < 3000.f) {
		bomb->curAnimBomb = ANIM_3;
	} else if(SDL_GetTicks() - bomb->timer < 4000.f) {
		bomb->curAnimBomb = ANIM_4;
	} else {
		bomb->curAnimBomb = ANIM_5;
		anim = 0;
	}


	if(anim) // If the bomb has not exploded yet we display the animation of the bomb...
		window_display_image(sprite_get_bomb(bomb->curAnimBomb),
				bomb->x * SIZE_BLOC, bomb->y * SIZE_BLOC);
	else // ... otherwise we display the explosion
		bomb_explosion(map, player, bomb, bombs, bombCounter);
}

int bomb_get_x(struct bomb* bomb)
{
	assert(bomb);
	return bomb->x;
}

int bomb_get_y(struct bomb* bomb)
{
	assert(bomb);
	return bomb->y;
}

float bomb_get_timer(struct bomb* bomb)
{
	assert(bomb);
	return bomb->timer;
}

void bomb_set_timer(struct bomb* bomb, float timer)
{
	assert(bomb);
	bomb->timer = timer;
}

void bomb_explosion(struct map* map, struct player* player, struct bomb* bomb, struct bomb* bombs[], int bombCounter)
{

	if(SDL_GetTicks() - bomb->timer < 5000.f) {
		bomb->curAnimBomb = ANIM_5;
		window_display_image(sprite_get_bomb(bomb->curAnimBomb),
				bomb->x * SIZE_BLOC, bomb->y * SIZE_BLOC);

		int cellType = 0;


		
		// If the player is standing on the bomb, he loses a life
		if(bomb->x == player_get_x(player) && bomb->y == player_get_y(player))
			player_dec_nb_life(player);

		// Explosion's displaying in every direction
		for(int i=1; i <= bomb->range; i++)
		{
			if(bomb->directions[NORTH]>=i && bomb->y-i >= 0) {
				cellType = map_get_cell_type(map, bomb->x,bomb->y-i);
				if(cellType != CELL_SCENERY) { // If the current cell isn't of the type CELL_SCENERY ...
					window_display_image(sprite_get_bomb(bomb->curAnimBomb), bomb->x * SIZE_BLOC, (bomb->y-i) * SIZE_BLOC); // ... we display the explosion's sprite

					if(!bomb->stopAnimDir[NORTH]) {
						for(int j=0; j<bombCounter; j++) {
							if(bomb_get_x(bombs[j]) == bomb->x && bomb_get_y(bombs[j]) == bomb->y-i && SDL_GetTicks() - bomb_get_timer(bombs[j]) < 4000.f) {
								bomb_set_timer(bombs[j], SDL_GetTicks() - 4000.f);
							}
						}
						if(bomb->x == player_get_x(player) && bomb->y-i == player_get_y(player))
							player_dec_nb_life(player);
						if(cellType == CELL_CASE) {
							map_set_cell_type(map, bomb->x,bomb->y-i, CELL_EMPTY); // CELL_CASE is replaced by CELL_EMPTY
							bomb->stopAnimDir[NORTH] = 1; // Variable used to control that the explosion can't go any further
						} else {
							bomb->directions[NORTH]++; // If the cell is not a case, then the explosion can keep going
						}
					}
				}
				else{ // ... otherwise we stop the explosion
					bomb->directions[NORTH] = i; 
					bomb->stopAnimDir[NORTH] = 1;
				}
			}

			if(bomb->directions[SOUTH]>=i && bomb->y+i < map_get_height(map)) {
				cellType = map_get_cell_type(map,bomb->x,bomb->y+i);
				if(cellType != CELL_SCENERY) {
					window_display_image(sprite_get_bomb(bomb->curAnimBomb), bomb->x * SIZE_BLOC, (bomb->y+i) * SIZE_BLOC);

					if(!bomb->stopAnimDir[SOUTH]) {
						for(int j=0; j<bombCounter; j++) {
							if(bomb_get_x(bombs[j]) == bomb->x && bomb_get_y(bombs[j]) == bomb->y+i && SDL_GetTicks() - bomb_get_timer(bombs[j]) < 4000.f) {
								bomb_set_timer(bombs[j], SDL_GetTicks() - 4000.f);
							}
						}
						if(bomb->x == player_get_x(player) && bomb->y+i == player_get_y(player))
							player_dec_nb_life(player);
						if(cellType == CELL_CASE) {
							map_set_cell_type(map, bomb->x,bomb->y+i, CELL_EMPTY);
							bomb->stopAnimDir[SOUTH] = 1;
						} else {
							bomb->directions[SOUTH]++;
						}
					}
				}
				else{
					bomb->directions[SOUTH] = i;
					bomb->stopAnimDir[SOUTH] = 1;
				}
			}

			if(bomb->directions[EAST]>=i && bomb->x+i < map_get_width(map)) {
				cellType = map_get_cell_type(map,bomb->x+i,bomb->y);
				if(cellType != CELL_SCENERY) {
					window_display_image(sprite_get_bomb(bomb->curAnimBomb), (bomb->x+i) * SIZE_BLOC, bomb->y * SIZE_BLOC);
					if(!bomb->stopAnimDir[EAST]) {
						for(int j=0; j<bombCounter; j++) {
							if(bomb_get_x(bombs[j]) == bomb->x+i && bomb_get_y(bombs[j]) == bomb->y && SDL_GetTicks() - bomb_get_timer(bombs[j]) < 4000.f) {
								bomb_set_timer(bombs[j], SDL_GetTicks() - 4000.f);
							}
						}
						if(bomb->x+i == player_get_x(player) && bomb->y == player_get_y(player))
							player_dec_nb_life(player);
						if(cellType == CELL_CASE) {
							map_set_cell_type(map, bomb->x+i,bomb->y, CELL_EMPTY);
							bomb->stopAnimDir[EAST] = 1;
						} else {
							bomb->directions[EAST]++;
						}
					}
				}
				else {
					bomb->directions[EAST] = i;
					bomb->stopAnimDir[EAST] = 1;
				}
			}

			if(bomb->directions[WEST]>=i && bomb->x-i >= 0) {
				cellType = map_get_cell_type(map,bomb->x-i,bomb->y);
				if(cellType != CELL_SCENERY) {
					window_display_image(sprite_get_bomb(bomb->curAnimBomb), (bomb->x-i) * SIZE_BLOC, bomb->y * SIZE_BLOC);
					if(!bomb->stopAnimDir[WEST]) {
						for(int j=0; j<bombCounter; j++) {
							if(bomb_get_x(bombs[j]) == bomb->x-i && bomb_get_y(bombs[j]) == bomb->y && SDL_GetTicks() - bomb_get_timer(bombs[j]) < 4000.f) {
								bomb_set_timer(bombs[j], SDL_GetTicks() - 4000.f);
							}
						}
						if(bomb->x-i == player_get_x(player) && bomb->y == player_get_y(player))
							player_dec_nb_life(player);
						if(cellType == CELL_CASE) {
							map_set_cell_type(map, bomb->x-i,bomb->y, CELL_EMPTY);
							bomb->stopAnimDir[WEST] = 1;
						} else {
							bomb->directions[WEST]++;
						}
					}
				}
				else{
					bomb->directions[WEST] = i;
					bomb->stopAnimDir[WEST] = 1;
				}
			}
		}
	
	}
}
