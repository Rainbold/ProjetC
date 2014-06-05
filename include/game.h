#ifndef GAME_H_
#define GAME_H_

#include <player.h>
#include <level.h>
#include <constant.h>

struct game;

// Create a new game
struct game* game_new(int curr_lvl, int nb_player);

// Change to next level
struct level* game_next_lvl(struct game* game);

// Free a game
void game_free(struct game* game);

// Return the state of the game
enum game_state game_get_state(struct game* game);

// Set the state of the game
void game_set_state(struct game* game, enum game_state state);

// Return the position of the cursor of the menu
int game_get_pos(struct game* game);

// Returns the array of players of the game
struct player** game_get_players(struct game* game);

// Return the player with this id
struct player* game_get_player(struct game* game, int id);

// Return the number of player
int game_get_nb_player(struct game* game);

// Set a player in the array players
void game_set_player(struct game* game, int i_tab, struct player* player);

// Returns the current level
struct level* game_get_curr_level(struct game* game);

// Return the number of the current level
int game_get_nb_curr_level(struct game* game);

// Set the level
void game_set_curr_level(struct game* game, struct level* level);

// Set the number of the level
void game_set_nb_curr_level(struct game* game, int nb_curr_level);

// Restart the game directly to the current map
void game_reset_lvl_map(struct game* game);

// Scores
int* game_get_scores(struct game* game);
void game_inc_score(struct game* game, int id);
void game_reset_scores(struct game* game);

// Display the game of the screen
void game_display(struct game* game);

// Order players by Y - Used to display players in a good order
void game_order_players_array(struct game* game, struct player** player);

// update the game
enum state game_update(enum state state, struct game* game, int key, key_event_t key_event);

#endif /* GAME_H_ */
