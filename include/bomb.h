#ifndef BOMB_H_
#define BOMB_H_

#include <stdlib.h>
#include <map.h>
#include <player.h>
#include <constant.h>
#include <misc.h>

typedef enum bomb_type {
	BOMB_NORMAL,
} b_type;

struct bomb;

void bomb_init(struct map* map, int x, int y, b_type type, int range);
void bomb_plant(struct map* map, struct player* player);
void bomb_display(struct map* map, struct player* player);
void bomb_update(struct map* map, struct player* player);
void bomb_explo_init(struct map* map, struct player* player, struct list* bList);
void bomb_free(struct bomb* bomb);

/*
void bomb_plant(struct map* map, struct player* player); // bomb planting function

void bomb_update(struct map* map, struct player* player, struct bomb* bombs[]);
void bomb_explo_event(struct map* map, struct player* player, struct bomb* bomb, struct bomb* bombs[]);
void bomb_explo_init(struct map* map, struct player* player, struct bomb* bomb);
void bomb_free(struct bomb* bomb);


int bomb_get_x(struct bomb* bomb);
int bomb_get_y(struct bomb* bomb);
float bomb_get_timer(struct bomb* bomb);

void bomb_set_timer(struct bomb* bomb, float timer);
*/


#endif /* BOMB_H_ */
