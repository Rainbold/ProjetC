#include <SDL/SDL_image.h> // IMG_Load
#include <SDL/SDL_ttf.h>
#include <assert.h>

#include <constant.h> 
#include <misc.h>

SDL_Surface* window;

void window_create(int width, int height) {
	assert(width > 0 && height > 0);

	if (SDL_Init(SDL_INIT_EVERYTHING) == -1) {
		error("Can't init SDL:  %s\n", SDL_GetError());
		exit(EXIT_FAILURE);
	}

	if(TTF_Init() == -1)
	{
	    fprintf(stderr, "Erreur d'initialisation de TTF_Init : %s\n", TTF_GetError());
	    exit(EXIT_FAILURE);
	}

	SDL_WM_SetCaption(WINDOW_NAME, NULL );
	window = SDL_SetVideoMode(width, height, 0, // If bits-per-pixel is 0, it is treated as the current display bits per pixel.
			SDL_HWSURFACE);

	if (window == NULL ) {
		error("Can't set video mode: %s\n", SDL_GetError());
		exit(EXIT_FAILURE);
	}
}

void window_free() {
	assert(window);
	SDL_FreeSurface(window);
}

void window_resize(int width, int height) {
	if(window->w != width || window->h != height) {
		window_free(window);
		window = SDL_SetVideoMode(width, height, 0, // If bits-per-pixel is 0, it is treated as the current display bits per pixel.
					SDL_HWSURFACE);
	}
}

void window_display_image(SDL_Surface* sprite, int x, int y) {
	assert(window);
	assert(sprite);

	SDL_Rect place;
	place.x = x;
	place.y = y;
	
	SDL_BlitSurface(sprite, NULL, window, &place);
}

void window_display_sprite(SDL_Surface* sprite, SDL_Rect rect, int x, int y) {
	assert(window);
	assert(sprite);
	SDL_SetColorKey(sprite, SDL_SRCCOLORKEY, SDL_MapRGB(sprite->format, 32, 56, 144));
	SDL_Rect place;
	place.x = x;
	place.y = y;
	place.h = 40;
	place.w = 40;

	SDL_BlitSurface(sprite, &rect, window, &place);
}

void window_clear() {
	assert(window);
	SDL_FillRect(window, NULL, SDL_MapRGB(window->format, 255, 255, 255));
}

void window_refresh() {
	assert(window);
	SDL_Flip(window);
}
