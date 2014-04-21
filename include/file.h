#ifndef FILE_H_
#define FILE_H_

struct map* file_load_map(int n_level, int n_map, int nb_players);
int file_map_exist(int n_level, int n_map);

#endif /* FILE_H_ */
