#ifndef MISC_H_
#define MISC_H_

#include <SDL/SDL.h>

// Print an error message on stderr and exit
void error(const char *s, ...);

// Load an image, raise an error in case of failure
SDL_Surface* load_image(const char *filename);

// Return a random number between a and b (a must be lesser than b)
int rand_ab(int a, int b);

// Check if the file is a valid map file
int map_is_valid_format1(const char* mapFile);
int map_is_valid_format2(const char* mapFile);

#endif /* MISC_H_ */
