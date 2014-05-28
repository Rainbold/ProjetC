#ifndef FILE_H_
#define FILE_H_

#include <game.h>

struct map* file_load_map(int n_level, int n_map);
struct map* file_load_map_multi(int n_map, int nb_players);

int file_map_exist(int n_level, int n_map);

void file_savegame(struct game* game);

int file_loadgame(struct game* game);

#endif /* FILE_H_ */
