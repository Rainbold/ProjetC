#ifndef PLAYER_H_
#define PLAYER_H_

#include <game.h>
#include <map.h>
#include <constant.h>

struct player;
 
// Creates a new player with a given number of available bombs and other parameters
struct player* player_init(int id, int bomb_number, int life_number, int range_number, int velocity);

// Free a player
void player_free(struct player* player);

// Returns the current position of the player
int player_get_x(struct player* player);
int player_get_y(struct player* player);

// Set the current position of the player
void player_set_x(struct player* player, int x);
void player_set_y(struct player* player, int y);


// Returns the current position of the player
int player_get_x_real(struct player* player);
int player_get_y_real(struct player* player);

// Reset the movement of a player
void player_reset_way_mov(struct player* player);

// Reset a player
void player_reset(struct game* game, int bomb_number, int life_number, int range_number, int velocity, int key);

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

// Set, Increase, Decrease the number of life that the player has
int player_get_nb_life(struct player* player);
void player_inc_nb_life(struct player* player);
void player_dec_nb_life(struct player* player);

// Hit a player, no effect if he is invincible
void player_hit(struct player* player, int i);

// Set an invisibility (in seconds)
void player_set_invicibility(struct player* player, int invicibility_time);

// Set, Increase the number of range that player has
int player_get_nb_range(struct player* player);
void player_inc_nb_range(struct player* player);
void player_dec_nb_range(struct player* player);

// Load the players positions from the map
void players_from_map(struct game* game, struct map* map);

// Move the player according to the current direction
int player_move(struct game* game, struct player* player, struct map* map);

// Display the player on the screen
void player_display(struct player* player);

// Set, Increase, Decrease the velocity of a player
void player_inc_velocity(struct player* player);
void player_dec_velocity(struct player* player);
int player_get_velovity(struct player * player);

// Return the number of key a player has
int player_get_key(struct player* player);

// Return the id of a player
int player_get_id(struct player* player);

// Update player variables
void player_update(struct player* player);

// Return the number of player alive
int player_get_nb_player_alive(struct game* game);

// Return the id of the player still alive
int player_get_id_player_alive(struct game* game);

#endif /* PLAYER_H_ */
