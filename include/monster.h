#ifndef MONSTER_H_
#define MONSTER_H_

#include <stdlib.h>
#include <map.h>
#include <player.h>
#include <constant.h>
#include <misc.h>

struct monster;

void monster_init(struct map* map, int x, int y, m_type type);

int monster_get_currentway(struct monster* monster);
void monster_set_currentway(struct monster* monster, way_t dir);
int monster_get_movetimer(struct monster* monster);
void monster_set_movetimer(struct monster* monster, int timer);
int monster_get_aggr(struct monster* monster);

m_type monster_get_type(struct monster* monster);

int monster_pathfinding(struct map* map, struct player* player, struct list* mList, int* getDist);

void monster_move(struct map* map, struct player* player);

void monster_display(struct map* map);

void monster_update(struct map* map);

int monster_get_nb_life(struct monster* monster);
void monster_set_nb_life(struct monster* monster, int life);
void monster_dec_nb_life(struct monster* monster);
void monster_set_invincibility(struct monster* monster, int invicibility_time);
int monster_get_life_timer(struct monster* monster);
void monster_set_life_timer(struct monster* monster, int lifeTimer);

struct list* monster_hit(struct list* mList, int x, int y, struct game* game);

struct list* monster_kill(struct list* mList, int x, int y, struct map* map);

#endif
