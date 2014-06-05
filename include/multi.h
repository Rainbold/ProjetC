#ifndef MULTI_H_
#define MULTI_H_

#include <game.h>

// Display the multiplayer game and menu
void multi_display(struct game* game);

// Change the state of the game and manage this changement
void multi_change_state(struct game* game, game_state_t state_to);

#endif /* MULTI_H_ */
