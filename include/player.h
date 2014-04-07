#ifndef PLAYER_H_
#define PLAYER_H_

#include <game.h>
#include <map.h>
#include <constant.h>

struct player;
 
// Creates a new player with a given number of available bombs
struct player* player_init(int bomb_number, int life_number, int range_number);
void player_free(struct player* player);

// Returns the current position of the player
int player_get_x(struct player* player);
int player_get_y(struct player* player);


void player_reset_way_mov(struct player* player);

// Set and return the direction of the player
void player_set_current_way(struct player* player, way_t way);
void player_set_way(struct player* player, way_t way);
void player_unset_way(struct player* player, way_t way);

// Set and return if the player is moving or not
void player_inc_moving(struct player* player);
void player_dec_moving(struct player* player);
int player_get_moving(struct player* player);

// Set, Increase, Decrease the number of bomb that player can put
int player_get_nb_bomb(struct player* player);
void player_inc_nb_bomb(struct player* player);
void player_dec_nb_bomb(struct player* player);

// Set, Increase, Decrease the number of life that player has
int player_get_nb_life(struct player* player);
void player_inc_nb_life(struct player* player);
void player_dec_nb_life(struct player* player);

void player_hit(struct player* player, int i);
void player_set_invicibility(struct player* player, int invicibility_time);

// Set, Increase the number of range that player has
int player_get_nb_range(struct player* player);
void player_inc_nb_range(struct player* player);
void player_dec_nb_range(struct player* player);

// Load the player position from the map
void player_from_map(struct player* player, struct map* map);

// Move the player according to the current direction
int player_move(struct player* player, struct map* map, struct game* game);

// Display the player on the screen
void player_display(struct player* player, struct game* game);

void player_inc_velocity(struct player* player);
void player_dec_velocity(struct player* player);

void player_update(struct player* player);

#endif /* PLAYER_H_ */
