#ifndef MONSTER_H_
#define MONSTER_H_

#include <stdlib.h>
#include <map.h>
#include <player.h>
#include <constant.h>
#include <misc.h>

struct monster;

// Initiate a monster
void monster_init(struct map* map, int x, int y, m_type type);

// Return the current way of a monster
int monster_get_currentway(struct monster* monster);

// Return the type of a monster
m_type monster_get_type(struct monster* monster);

// Pathfinding
int monster_pathfinding(struct map* map, struct player* player, struct list* mList, int* getDist);

// Move monsters
void monster_move(struct map* map, struct player* player);

// Display monsters
void monster_display(struct map* map);

// Update monsters
void monster_update(struct map* map);

// Set the invisibility of a monster
void monster_set_invisibility(struct monster* monster, int invicibility_time);

// Hit a monster
struct list* monster_hit(struct list* mList, int x, int y, struct game* game);

// Kill a monster
struct list* monster_kill(struct list* mList, int x, int y, struct map* map);

#endif
