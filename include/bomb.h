#ifndef BOMB_H_
#define BOMB_H_

#include <map.h>
#include <player.h>

struct bomb; 

struct bomb* bomb_plant(struct map* map, struct player* player); // bomb planting function
void bomb_explosion(struct map* map, struct player* player, struct bomb* bomb); // environment destruction
void bomb_display(struct map* map, struct player* player, struct bomb* bomb);

#endif /* BOMB_H_ */