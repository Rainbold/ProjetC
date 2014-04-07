#include <assert.h>
#include <time.h>
#include <SDL/SDL.h>
#include <game.h>
#include <misc.h>
#include <window.h>
#include <sprite.h>
#include <bomb.h>
#include <monster.h>

struct game {
	struct level* curr_level; // current level
	struct player* player;
	enum game_state game_state;
	int	frame;
	int pos;
};

struct game* game_new(int i) {

	struct game* game = malloc(sizeof(*game));
	game->curr_level = level_get_level(i, game); // get maps of the level 0

	game->player = player_init(1, 2, 1); // player init with nb_bomb, nb_life and nb_range
	player_from_map(game->player, level_get_map(game->curr_level, 0)); // get x,y of the player on the first map

	game->game_state = PLAYING;
	game->frame = 0;
	game->pos = 0;

	return game;
}

struct level* game_next_lvl(struct game* game) {
	assert(game);
	game->curr_level = level_get_level(1, game);
	return(game->curr_level);
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

int game_get_frame(struct game* game)
{
	assert(game);
	return game->frame;
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

	struct player* player = game->player;

	if(player_get_moving(player) && game->game_state == PLAYING)
		player_move(player, level_get_curr_map(game->curr_level), game);

	window_clear();

	game_banner_display(game);
	level_display(game_get_curr_level(game));

	bomb_display(
			game,
			level_get_curr_map(game->curr_level), // map
			player);

	monster_display(level_get_curr_map(game->curr_level), player, game);

	player_display(player, game);

	if(game->game_state == PLAYING) {
		player_update(player);
		game->frame++;
	}
	if(game->game_state == PAUSED) {
		window_display_image(sprite_get_menu(M_BG_GREY), 0, 0);
		window_display_image(sprite_get_menu(M_H_PAUSE), MAP_WIDTH *  SIZE_BLOC / 2 - 185, 0);
		window_display_image(sprite_get_menu(M_B_KEEP), MAP_WIDTH *  SIZE_BLOC / 2 - 75, 170);
		window_display_image(sprite_get_menu(M_B_MAINMENU), MAP_WIDTH *  SIZE_BLOC / 2 - 75, 220);
		window_display_image(sprite_get_menu(M_B_QUIT), MAP_WIDTH *  SIZE_BLOC / 2 - 75, 270);
		//window_display_image(sprite_get_player(SOUTH), MAP_WIDTH *  SIZE_BLOC / 2 - 75 - 40, 170 + 50 * game->pos);
	}
	window_refresh();
}

enum state game_update(struct game* game, int key, key_event_t key_event) {
	struct player* player = game_get_player(game);
	struct map* map = level_get_curr_map(game_get_curr_level(game));

	if(key_event == DOWN) {
		switch (key) {
		case SDLK_ESCAPE:
		case SDLK_p: // Pause
			game->game_state = !(game->game_state);
			game->pos = 0;
			return GAME;
			break;
		case SDLK_RETURN:
		case SDLK_KP_ENTER:
			if(game->game_state == PAUSED) {
				switch(game->pos) {
				case 0:
					game->game_state = !(game->game_state);
					game->pos = 0;
					return GAME;
					break;
				case 1:
					return ENDGAME;
				case 2:
					return QUIT;
				}
			}
			break;
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
		case SDLK_d:
			player_dec_velocity(player);
			break;
		case SDLK_f:
			player_inc_velocity(player);
			break;
		case SDLK_UP:
			if(game->game_state == PLAYING){
				player_set_way(player, NORTH);
				player_set_current_way(player, NORTH);
				player_inc_moving(player);
			}
			else if (game->game_state == PAUSED && game->pos > 0) {
				game->pos--;
			}
			break;
		case SDLK_DOWN:
			if(game->game_state == PLAYING){
				player_set_way(player, SOUTH);
				player_set_current_way(player, SOUTH);
				player_inc_moving(player);
			}
			else if (game->game_state == PAUSED && game->pos < 2) {
				game->pos++;
			}
			break;
		case SDLK_RIGHT:
			if(game->game_state == PLAYING){
				player_set_way(player, EAST);
				player_set_current_way(player, EAST);
				player_inc_moving(player);
			}
			break;
		case SDLK_LEFT:
			if(game->game_state == PLAYING){
				player_set_way(player, WEST);
				player_set_current_way(player, WEST);
				player_inc_moving(player);
			}
			break;
		case SDLK_SPACE:
			if(game->game_state == PLAYING)
				bomb_plant(game, map, player); // the bomb is planted if it is possible
			break;
		default:
			break;
		}
	}
	else if(key_event == UP) {
		switch (key) {
		case SDLK_UP:
			if(game->game_state == PLAYING) {
				player_dec_moving(player);
				player_unset_way(player, NORTH);
			}
			break;
		case SDLK_DOWN:
			if(game->game_state == PLAYING) {
				player_dec_moving(player);
				player_unset_way(player, SOUTH);
			}
			break;
		case SDLK_RIGHT:
			if(game->game_state == PLAYING) {
				player_dec_moving(player);
				player_unset_way(player, EAST);
			}
			break;
		case SDLK_LEFT:
			if(game->game_state == PLAYING) {
				player_dec_moving(player);
				player_unset_way(player, WEST);
			}
			break;
		}
	}
	return GAME;
}
