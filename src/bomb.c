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
	float timer;
	int length;
	int directions[4];
	int stopAnimDir[4];
};

struct bomb* bomb_plant(struct map* map, struct player* player)
{
	struct bomb* bomb = malloc( sizeof(*bomb) );
	bomb->x = player_get_x(player);
	bomb->y = player_get_y(player);
	player_dec_nb_bomb(player);

	bomb->timer = SDL_GetTicks();

	bomb->curAnimBomb = ANIM_4;
	bomb->length = 1;
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

void bomb_display(struct map* map, struct player* player, struct bomb* bomb) {
	assert(bomb);
	int anim = 1;
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

	if(anim)
		window_display_image(sprite_get_bomb(bomb->curAnimBomb),
				bomb->x * SIZE_BLOC, bomb->y * SIZE_BLOC);
	else
		bomb_explosion(map, player, bomb);
}

void bomb_explosion(struct map* map, struct player* player, struct bomb* bomb)
{
	if(SDL_GetTicks() - bomb->timer < 5000.f) {
		bomb->curAnimBomb = ANIM_5;
		window_display_image(sprite_get_bomb(bomb->curAnimBomb),
				bomb->x * SIZE_BLOC, bomb->y * SIZE_BLOC);

		int stop = 0;

		int cellType = 0;
		for(int i=0; i <= bomb->length; i++)
		{
			if(bomb->directions[NORTH]>i && bomb->y-i >= 0) {
				cellType = map_get_cell_type(map, bomb->x,bomb->y-i);
				if(cellType != CELL_SCENERY) {
					window_display_image(sprite_get_bomb(bomb->curAnimBomb), bomb->x * SIZE_BLOC, (bomb->y-i) * SIZE_BLOC);
					if(!bomb->stopAnimDir[NORTH]) {
						if(cellType == CELL_CASE) {
							map_set_cell_type(map, bomb->x,bomb->y-i, CELL_EMPTY);
							bomb->stopAnimDir[NORTH] = 1;
						} else {
							bomb->directions[NORTH]++;
						}
					}
				}
				else{
					bomb->directions[NORTH] = i;
					bomb->stopAnimDir[NORTH] = 1;
				}
			}
			printf("%d", bomb->directions[0]);

			if(bomb->directions[SOUTH]>i && bomb->y+i < map_get_height(map)) {
				cellType = map_get_cell_type(map,bomb->x,bomb->y+i);
				if(cellType != CELL_SCENERY) {
					window_display_image(sprite_get_bomb(bomb->curAnimBomb), bomb->x * SIZE_BLOC, (bomb->y+i) * SIZE_BLOC);
					if(!bomb->stopAnimDir[SOUTH]) {
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

			if(bomb->directions[EAST]>i && bomb->x+i < map_get_width(map)) {
				cellType = map_get_cell_type(map,bomb->x+i,bomb->y);
				if(cellType != CELL_SCENERY) {
					window_display_image(sprite_get_bomb(bomb->curAnimBomb), (bomb->x+i) * SIZE_BLOC, bomb->y * SIZE_BLOC);
					if(!bomb->stopAnimDir[EAST]) {
						if(cellType == CELL_CASE) {
							map_set_cell_type(map, bomb->x+i,bomb, CELL_EMPTY);
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

			if(bomb->directions[WEST]>i && bomb->x-i >= 0) {
				cellType = map_get_cell_type(map,bomb->x-i,bomb->y);
				if(cellType != CELL_SCENERY) {
					window_display_image(sprite_get_bomb(bomb->curAnimBomb), (bomb->x-i) * SIZE_BLOC, bomb->y * SIZE_BLOC);
					if(!bomb->stopAnimDir[WEST]) {
						if(cellType == CELL_CASE) {
							map_set_cell_type(map, bomb->x-i,bomb, CELL_EMPTY);
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