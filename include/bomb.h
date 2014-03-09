#ifndef BOMB_H_
#define BOMB_H_

#include <map.h>
#include <player.h>

struct bomb; 

struct bomb* bomb_plant(struct map* map, struct player* player); // bomb planting function
void bomb_explosion(struct map* map, struct player* player, struct bomb* bomb, struct bomb* bombs[], int bombCounter); // environment destruction
void bomb_display(struct map* map, struct player* player, struct bomb* bomb, struct bomb* bombs[], int bombCounter);

int bomb_get_x(struct bomb* bomb);
int bomb_get_y(struct bomb* bomb);
float bomb_get_timer(struct bomb* bomb);

void bomb_set_timer(struct bomb* bomb, float timer);

#endif /* BOMB_H_ */