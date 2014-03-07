#ifndef LEVEL_H_
#define LEVEL_H_

#include <map.h>

struct level;

// return level number num
struct level*  level_get_level(int num);

// return current map of the level
struct map* level_get_curr_map(struct level* level);

// return map number num of the level
struct map*  level_get_map(struct level* level, int num);

// free the level
void  level_free(struct level* level);

// display the level
void  level_display(struct level* level);

// free level struct
void   level_free(struct level* level);

#endif /* LEVEL_H_ */
