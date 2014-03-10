#ifndef MONSTER_H_
#define MONSTER_H_

#include <stdlib.h>
#include <map.h>
#include <player.h>
#include <constant.h>
#include <misc.h>

typedef enum monster_type {
	MONSTER_NORMAL,
} m_type;

struct monster;

void monster_init(struct map* map, int x, int y, m_type type, int size, int life, int aggr);

int monster_get_currentway(struct monster* monster);
void monster_set_currentway(struct monster* monster, way_t dir);
int monster_get_movetimer(struct monster* monster);
void monster_set_movetimer(struct monster* monster, float timer);
int monster_get_aggr(struct monster* monster);

int monster_pathfinding(struct map* map, struct player* player, struct list* mList, int* getDist);

int monster_move(struct list* mList, struct map* map, struct player* player);

void monster_display(struct map* map, struct player* player);

struct list* monster_kill(struct list* mList, int x, int y);




#endif