#include <SDL/SDL_image.h> // IMG_Load
#include <misc.h>
#include <time.h>
#include <assert.h>

void error(const char *s, ...) {
	va_list ap; 
	va_start(ap, s);

	vfprintf(stderr, s, ap);
	va_end(ap);
	exit(EXIT_FAILURE);
}

SDL_Surface* load_image(const char* filename) {
	SDL_Surface* img = IMG_Load(filename);
	if (!img)
		error("IMG_Load: %s\n", IMG_GetError());

	return img;
}

int rand_ab(int a, int b)
{
	assert(a<b);
	
	int divisor = RAND_MAX/(b-a+1);
	int res;

	do {
		res = rand() / divisor;
	} while(res > b);

	return res+a;
}