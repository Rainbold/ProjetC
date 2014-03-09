#include <assert.h>
#include <time.h>

#include <game.h>
#include <misc.h>
#include <window.h>
#include <sprite.h>
#include <bomb.h>

struct game {
	struct level* curr_level; // current level
	struct player* player;
	struct bomb* bombs[MAP_WIDTH*MAP_HEIGHT];
	int bombCounter;
};

struct game* game_new(void) {
	sprite_load(); // load sprites into process memory

	struct game* game = malloc(sizeof(*game));
	game->curr_level = level_get_level(0); // get maps of the first level

	game->player = player_init(1, 2, 1); // player init with nb_bomb, nb_life and nb_range
	player_from_map(game->player, level_get_map(game->curr_level, 0)); // get x,y of the player on the first map

	game->bombCounter = 0; // Bombs' number initialized to 0
	for( int i=0; i<MAP_WIDTH*MAP_HEIGHT; i++ ) {
		game->bombs[i] = NULL;
	}

	return game;
}

void game_free(struct game* game) {
	assert(game);

	player_free(game->player);
	level_free(game->curr_level);
}

struct player* game_get_player(struct game* game) {
	assert(game);
	return game->player;
}

struct level* game_get_curr_level(struct game* game) {
	return game->curr_level;
}

/* Display the game's interface */
void game_banner_display(struct game* game) {
	assert(game);

	struct map* map = level_get_curr_map(game_get_curr_level(game));

	int y = (map_get_height(map)) * SIZE_BLOC;
	for (int i = 0; i < map_get_width(map); i++)
		window_display_image(sprite_get_banner_line(), i * SIZE_BLOC, y);

	int white_bloc = ((map_get_width(map) * SIZE_BLOC) - 6 * SIZE_BLOC) / 4;
	int x = white_bloc;
	y = (map_get_height(map) * SIZE_BLOC) + LINE_HEIGHT;
	window_display_image(sprite_get_banner_life(), x, y); // sprite life

	x = white_bloc + SIZE_BLOC;
	window_display_image(
			sprite_get_number(player_get_nb_life(game_get_player(game))), x, y); // life number

	x = 2 * white_bloc + 2 * SIZE_BLOC;
	window_display_image(sprite_get_banner_bomb(), x, y); // bomb sprite

	x = 2 * white_bloc + 3 * SIZE_BLOC;
	window_display_image(
			sprite_get_number(player_get_nb_bomb(game_get_player(game))), x, y); // bomb number

	x = 3 * white_bloc + 4 * SIZE_BLOC;
	window_display_image(sprite_get_banner_range(), x, y); // range sprite

	x = 3 * white_bloc + 5 * SIZE_BLOC;
	window_display_image(
			sprite_get_number(player_get_nb_range(game_get_player(game))), x, y); // range number, todo : range variable
}

void game_display(struct game* game) {
	assert(game);

	window_clear();

	game_banner_display(game);
	level_display(game_get_curr_level(game));

	for(int i=0; i<game->bombCounter; i++)
		bomb_display(level_get_curr_map(game->curr_level), game->player, game->bombs[i], game->bombs, game->bombCounter);

	player_display(game->player);

	window_refresh();
}

short input_keyboard(struct game* game, int isPaused) { // todo : P for pause, space for bomb
	SDL_Event event;
	struct player* player = game_get_player(game);
	struct map* map = level_get_curr_map(game_get_curr_level(game));

	while (SDL_PollEvent(&event)) {
		switch (event.type) {
		case SDL_QUIT: // keyboard : esc
			return 1;
		case SDL_KEYDOWN:
			switch (event.key.keysym.sym) {
			case SDLK_ESCAPE: 
				return 1;
			case SDLK_a:
				player_inc_nb_life(player);
				break;
			case SDLK_z:
				player_dec_nb_life(player);
				break;
			case SDLK_q:
				player_inc_nb_bomb(player);
				break;
			case SDLK_s:
				player_dec_nb_bomb(player);
				break;
			case SDLK_w:
				player_inc_nb_range(player);
				break;
			case SDLK_x:
				player_dec_nb_range(player);
				break;
			case SDLK_UP:
				if(!isPaused) {
					player_set_current_way(player, NORTH);
					player_move(player, map);
				}
				break;
			case SDLK_DOWN:
				if(!isPaused) {
					player_set_current_way(player, SOUTH);
					player_move(player, map);
				}
				break;
			case SDLK_RIGHT:
				if(!isPaused) {
					player_set_current_way(player, EAST);
					player_move(player, map);
				}
				break;
			case SDLK_LEFT:
				if(!isPaused) {
					player_set_current_way(player, WEST);
					player_move(player, map);
				}
				break;
			case SDLK_SPACE:
				if(player_get_nb_bomb(player) > 0) { // If the player still has at least one bomb...
					game->bombs[game->bombCounter] = bomb_plant(level_get_curr_map(game->curr_level), player); // ...the bomb is planted
					if(game->bombCounter < MAP_WIDTH*MAP_HEIGHT - 1)
						game->bombCounter++;
				}
				break;
			case SDLK_p:
				return 2;
				break;
			default:
				break;
			}

			break;
		}
	}
	return 0;
}

int game_update(struct game* game, int isPaused) {
	int state = input_keyboard(game, isPaused);
	if (state == 1)
		return 1; // exit game
	if (state == 2)
		return 2; // paused
	return 0;
}
