#ifndef BOMB_H_
#define BOMB_H_

#include <game.h>
#include <map.h>
#include <player.h>

struct bomb; 

void bomb_plant(struct map* map, struct player* player, struct game* game); // bomb planting function
void bomb_display(struct map* map, struct player* player, struct bomb* bombs[], struct game* game);
void bomb_update(struct map* map, struct player* player, struct bomb* bombs[], struct game* game);
void bomb_explo_event(struct map* map, struct player* player, struct bomb* bomb, struct bomb* bombs[], struct game* game);
void bomb_explo_init(struct map* map, struct player* player, struct bomb* bomb);
void bomb_free(struct bomb* bomb);


int bomb_get_x(struct bomb* bomb);
int bomb_get_y(struct bomb* bomb);
float bomb_get_timer(struct bomb* bomb);

void bomb_set_timer(struct bomb* bomb, float timer);

#endif /* BOMB_H_ */
