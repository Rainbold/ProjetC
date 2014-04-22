#ifndef BOMB_H_
#define BOMB_H_

#include <map.h>
#include <player.h>
#include <constant.h>
#include <misc.h>

typedef enum bomb_type {
	BOMB_NORMAL,
} b_type;


struct bomb;

void bomb_init(struct game* game, struct map* map, int x, int y, b_type type, int range);
void bomb_plant(struct game* game, struct map* map, struct player* player);
void bomb_display(struct game* game,struct map* map, struct player* player);
void bomb_update(struct game* game, struct map* map, struct player* player);
void bomb_explo_init(struct map* map, struct player* player, struct list* bList);
void bomb_free(struct map* map, struct list* bList);
struct bomb* search_bomb(struct map* map, int x, int y, int state);

#endif /* BOMB_H_ */
