#ifndef MISC_H_
#define MISC_H_

#include <SDL/SDL.h>

// Print an error message on stderr and exit
void error(const char *s, ...);

// Load an image, raise an error in case of failure
SDL_Surface* load_image(const char *filename);

#endif /* MISC_H_ */
