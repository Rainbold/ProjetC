#ifndef FILE_H_
#define FILE_H_

#include <game.h>

// Load a map (level: n_level, map: n_map)
struct map* file_load_map(int n_level, int n_map);

// Load a multiplayer map
struct map* file_load_map_multi(int n_map, int nb_players);

// Return if the file exist or not
int file_map_exist(int n_level, int n_map);

// Create a savegame of the current state of the game, level, map and player.
void file_savegame(struct game* game);

// Load the game from a savegame
int file_loadgame(struct game* game);

#endif /* FILE_H_ */
