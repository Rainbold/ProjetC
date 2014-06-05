#ifndef WINDOW_H_
#define WINDOW_H_

#include <SDL/SDL.h>

// Create the main windows
void window_create(int width, int height);

// Free th main windows
void window_free();

// Resize the main windows
void window_resize(int width, int height);

// Refresh the main window
void window_refresh();

// display a SDL_Surface at location (x,y)
void window_display_image(SDL_Surface* surface, int x, int y);

// display a SDL_Surface at location (x,y) and use a SDL_Rect
void window_display_sprite(SDL_Surface* sprite, SDL_Rect rect, int x, int y);

// set every pixel of the window to white
void window_clear();

#endif /* WINDOW_H_ */
