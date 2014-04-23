#include <assert.h>
#include <time.h>
#include <SDL/SDL.h>
#include <game.h>
#include <misc.h>
#include <window.h>
#include <sprite.h>
#include <bomb.h>
#include <monster.h>
#include <menu.h>

struct game {
	struct level* curr_level; // current level
	struct player* player[4];
	enum game_state game_state;
	int nb_player;
	int	frame;
	int pos;
	int nb_curr_level;
};

struct game* game_new(int curr_lvl, int nb_player) {
	struct game* game = malloc(sizeof(*game));
	game->nb_curr_level = curr_lvl;

	game->nb_player = nb_player;
	game->curr_level = level_get_level(game->nb_curr_level, game); // get maps of the level 0

	for(int i=0; i<nb_player; i++) {
		game->player[i] = player_init(i, 1, 2, 1); // player init : #1, nb_bomb, nb_life and nb_range
		player_from_map(game->player[i], level_get_map(game->curr_level, 0)); // get x,y of the player on the first map
	}

	game->game_state = PLAYING;
	game->frame = 0;
	game->pos = 0;

	return game;
}

struct level* game_next_lvl(struct game* game) {
	assert(game);
	game->nb_curr_level++;
	game->curr_level = level_get_level(game->nb_curr_level, game);
	return(game->curr_level);
}

void game_free(struct game* game) {
	assert(game);

	for(int i=0; i<game->nb_player; i++)
		player_free(game->player[i]);
	level_free(game->curr_level);
}

int game_get_nb_player(struct game* game) {
	assert(game);
	return game->nb_player;
}

struct player* game_get_player(struct game* game, int id) {
	assert(game);
	assert(id > 0 && id <= game->nb_player);
	return game->player[id-1];
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
	struct player* player = game->player[0];

	int y = (map_get_height(map)) * SIZE_BLOC;
	for (int i = 0; i < map_get_width(map); i++)
		window_display_image(sprite_get_banner_line(), i * SIZE_BLOC, y);

	int white_bloc = ((map_get_width(map) * SIZE_BLOC) - 6 * SIZE_BLOC) / 4;
	int x = white_bloc;
	y = (map_get_height(map) * SIZE_BLOC) + LINE_HEIGHT;
	window_display_image(sprite_get_banner_life(), x, y); // sprite life

	x = white_bloc + SIZE_BLOC;
	window_display_image(
			sprite_get_number(player_get_nb_life(game_get_player(game, 1))), x, y); // life number

	x = 2 * white_bloc + 2 * SIZE_BLOC;
	window_display_image(sprite_get_banner_bomb(), x, y); // bomb sprite

	x = 2 * white_bloc + 3 * SIZE_BLOC;
	window_display_image(
			sprite_get_number(player_get_nb_bomb(game_get_player(game, 1))), x, y); // bomb number

	x = 3 * white_bloc + 4 * SIZE_BLOC;
	window_display_image(sprite_get_banner_range(), x, y); // range sprite

	x = 3 * white_bloc + 5 * SIZE_BLOC;
	window_display_image(
			sprite_get_number(player_get_nb_range(game_get_player(game, 1))), x, y); // range number

	if(player_get_key(player))
		window_display_image(sprite_get_key(), (map_get_width(map) -1) * SIZE_BLOC, y);
}

void game_display(struct game* game) {
	assert(game);

	window_clear();

	int frameChanged = 0;

	if(game->nb_player < 2)
		game_banner_display(game);
	level_display(game_get_curr_level(game));

	struct player* player[game->nb_player];
	
	int minYLast = -1;
	int maxY = -1;


	for(int i=0; i<game->nb_player; i++) 
	{
		player[i] = game->player[i];
		if(game->game_state == PLAYING) {
				if(player_get_moving(player[i]))
					player_move(player[i], level_get_curr_map(game->curr_level), game);

		if(player_get_y(game->player[i]) > maxY)
			maxY = player_get_y(game->player[i]);
				if(map_get_bombs(level_get_curr_map(game->curr_level)) != NULL) // if there is at least one bomb
					bomb_update(game, level_get_curr_map(game->curr_level), player[i]);

				player_update(player[i]);
				if(!frameChanged)
				{
					frameChanged = !frameChanged;
					game->frame++;
				}
			}
			else if(game->game_state == PAUSED) {
		/*		window_display_image(sprite_get_menu(M_BG_GREY), 0, 0);
				window_display_image(sprite_get_menu(M_H_PAUSE), MAP_WIDTH *  SIZE_BLOC / 2 - 185, 0);
				window_display_image(sprite_get_menu(M_B_KEEP), MAP_WIDTH *  SIZE_BLOC / 2 - 75, 170);
				window_display_image(sprite_get_menu(M_B_MAINMENU), MAP_WIDTH *  SIZE_BLOC / 2 - 75, 220);
				window_display_image(sprite_get_menu(M_B_QUIT), MAP_WIDTH *  SIZE_BLOC / 2 - 75, 270);
				window_display_image(sprite_get_menu(M_SELECT), MAP_WIDTH *  SIZE_BLOC / 2 - 75 - 40, 170 + 50 * game->pos);
		*/
				menu_display(map_get_width(level_get_curr_map(game->curr_level)) / 2 * SIZE_BLOC, map_get_height(level_get_curr_map(game->curr_level)) / 2 * SIZE_BLOC);
			}
	}

	/*
	 * In order to make players pass through each others smootly,
	 * the initial minimum y-position is first set to the maximum one
	 * (because setting it to a greater value doesn't work).
	 * Then, the minimum y-position is set to the actual one and players with the same y-position
	 * are displayed.
	 * This value is then recorded and stored into minYLast.
	 * The algorithm is repeated with the minimum y-position that is greater than minYLast 
	 * and all players are sorted to be displayed in the correct order.
	 */

	for(int i=0; i<game->nb_player; i++) 
	{
		if(player_get_y_real(game->player[i]) > maxY)
			maxY = player_get_y_real(game->player[i]);
	}

	int minY = maxY;

	for(int k=0; k<game->nb_player; k++) 
	{
		for(int j=0; j<game->nb_player; j++) 
		{
			if(player_get_y_real(game->player[j]) < minY && player_get_y_real(game->player[j]) > minYLast)
				minY = player_get_y_real(game->player[j]);
		}

		for(int i=0; i<game->nb_player; i++)
		{
			player[i] = game->player[i];
			if(player_get_y_real(player[i]) == minY)
			{

				bomb_display(
						game,
						level_get_curr_map(game->curr_level), // map
						player[i]);

				monster_display(level_get_curr_map(game->curr_level), player[i], game);

				player_display(player[i], game);
			}
		}
		minYLast = minY;
		minY = maxY;
	}
	window_refresh();
}

enum state game_update(enum state state, struct game* game, int key, key_event_t key_event) {
	//struct player* player = game_get_player(game, 1);
	struct player* player[game->nb_player];
	for(int i=0; i<game->nb_player; i++) 
	{
		player[i] = game->player[i];
	}
	struct map* map = level_get_curr_map(game_get_curr_level(game));

	if(key_event == DOWN) {
		switch (key) {
		case SDLK_ESCAPE:
		case SDLK_p: // Pause
			if(game->game_state == PLAYING) {
				new_menu(PAUSE_SINGLE);
				game->game_state = PAUSED;
			}
			else {
				menu_free(NULL);
				game->game_state = PLAYING;
			}
			return GAME;
			break;
		case SDLK_RETURN:
		case SDLK_KP_ENTER:
			if(game->game_state == PAUSED) {
				switch(menu_update(state, key, key_event)) {
				case KEEP:
					menu_free(NULL);
					game->game_state = PLAYING;
					break;
				case MAINMENU:
					return(ENDGAME);
					break;
				case QUIT:
					return(QUIT);
					break;
				default:
					break;
				}
			}
			break;
		// case SDLK_a:
		// 	player_inc_nb_life(player);
		// 	break;
		// case SDLK_z:
		// 	player_dec_nb_life(player);
		// 	break;
		// case SDLK_q:
		// 	player_inc_nb_bomb(player);
		// 	break;
		// case SDLK_s:
		// 	player_dec_nb_bomb(player);
		// 	break;
		// case SDLK_w:
		// 	player_inc_nb_range(player);
		// 	break;
		// case SDLK_x:
		// 	player_dec_nb_range(player);
		// 	break;
		// case SDLK_d:
		// 	player_dec_velocity(player);
		// 	break;
		// case SDLK_f:
		// 	player_inc_velocity(player);
		// 	break;
		case SDLK_UP:
			if(game->game_state == PLAYING){
				player_set_way(player[0], NORTH);
				player_inc_moving(player[0]);
			}
			else if (game->game_state == PAUSED) {
				return(menu_update(state, key, key_event));
			}
			break;
		case SDLK_DOWN:
			if(game->game_state == PLAYING){
				player_set_way(player[0], SOUTH);
				player_inc_moving(player[0]);
			}
			else if (game->game_state == PAUSED) {
				return(menu_update(state, key, key_event));
			}
			break;
		case SDLK_RIGHT:
			if(game->game_state == PLAYING){
				player_set_way(player[0], EAST);
				player_inc_moving(player[0]);
			}
			break;
		case SDLK_LEFT:
			if(game->game_state == PLAYING){
				player_set_way(player[0], WEST);
				player_inc_moving(player[0]);
			}
			break;
		case SDLK_SPACE:
			if(game->game_state == PLAYING)
				bomb_plant(game, map, player[0]); // the bomb is planted if it is possible
			break;
		case SDLK_z:
			if(game->nb_player >= 2)
			{
				if(game->game_state == PLAYING){
					player_set_way(player[1], NORTH);
					player_inc_moving(player[1]);
				}
				else if (game->game_state == PAUSED) {
					return(menu_update(state, key, key_event));
				}
			}
			break;
		case SDLK_s:
			if(game->nb_player >= 2)
			{
				if(game->game_state == PLAYING){
					player_set_way(player[1], SOUTH);
					player_inc_moving(player[1]);
				}
				else if (game->game_state == PAUSED) {
					return(menu_update(state, key, key_event));
				}
			}
			break;
		case SDLK_d:
			if(game->nb_player >= 2)
			{
				if(game->game_state == PLAYING){
					player_set_way(player[1], EAST);
					player_inc_moving(player[1]);
				}
			}
			break;
		case SDLK_q:
			if(game->nb_player >= 2)
			{
				if(game->game_state == PLAYING){
					player_set_way(player[1], WEST);
					player_inc_moving(player[1]);
				}
			}
			break;
		case SDLK_a:
			if(game->nb_player >= 2)
			{
				if(game->game_state == PLAYING)
					bomb_plant(game, map, player[1]); // the bomb is planted if it is possible
			}
			break;
		case SDLK_t:
			if(game->nb_player >= 3)
			{
				if(game->game_state == PLAYING){
					player_set_way(player[2], NORTH);
					player_inc_moving(player[2]);
				}
				else if (game->game_state == PAUSED) {
					return(menu_update(state, key, key_event));
				}
			}
			break;
		case SDLK_g:
			if(game->nb_player >= 3)
			{
				if(game->game_state == PLAYING){
					player_set_way(player[2], SOUTH);
					player_inc_moving(player[2]);
				}
				else if (game->game_state == PAUSED) {
					return(menu_update(state, key, key_event));
				}
			}
			break;
		case SDLK_h:
			if(game->nb_player >= 3)
			{
				if(game->game_state == PLAYING){
					player_set_way(player[2], EAST);
					player_inc_moving(player[2]);
				}
			}
			break;
		case SDLK_f:
			if(game->nb_player >= 3)
			{
				if(game->game_state == PLAYING){
					player_set_way(player[2], WEST);
					player_inc_moving(player[2]);
				}
			}
			break;
		case SDLK_r:
			if(game->nb_player >= 3)
			{
				if(game->game_state == PLAYING)
					bomb_plant(game, map, player[2]); // the bomb is planted if it is possible
			}
			break;
		case SDLK_i:
			if(game->nb_player >= 4)
			{
				if(game->game_state == PLAYING){
					player_set_way(player[3], NORTH);
					player_inc_moving(player[3]);
				}
				else if (game->game_state == PAUSED) {
					return(menu_update(state, key, key_event));
				}
			}
			break;
		case SDLK_k:
			if(game->nb_player >= 4)
			{
				if(game->game_state == PLAYING){
					player_set_way(player[3], SOUTH);
					player_inc_moving(player[3]);
				}
				else if (game->game_state == PAUSED) {
					return(menu_update(state, key, key_event));
				}
			}
			break;
		case SDLK_l:
			if(game->nb_player >= 4)
			{
				if(game->game_state == PLAYING){
					player_set_way(player[3], EAST);
					player_inc_moving(player[3]);
				}
			}
			break;
		case SDLK_j:
			if(game->nb_player >= 4)
			{
				if(game->game_state == PLAYING){
					player_set_way(player[3], WEST);
					player_inc_moving(player[3]);
				}
			}
			break;
		case SDLK_u:
			if(game->nb_player >= 4)
			{
				if(game->game_state == PLAYING)
					bomb_plant(game, map, player[3]); // the bomb is planted if it is possible
			}
			break;
		default:
			break;
		}
	}
	else if(key_event == UP) {
		switch (key) {
		case SDLK_UP:
				player_dec_moving(player[0]);
				player_unset_way(player[0], NORTH);
			break;
		case SDLK_DOWN:
				player_dec_moving(player[0]);
				player_unset_way(player[0], SOUTH);
			break;
		case SDLK_RIGHT:
				player_dec_moving(player[0]);
				player_unset_way(player[0], EAST);
			break;
		case SDLK_LEFT:
				player_dec_moving(player[0]);
				player_unset_way(player[0], WEST);
			break;
		case SDLK_z:
			if(game->nb_player >= 2)
			{
				player_dec_moving(player[1]);
				player_unset_way(player[1], NORTH);
			}
			break;
		case SDLK_s:
			if(game->nb_player >= 2)
			{
				player_dec_moving(player[1]);
				player_unset_way(player[1], SOUTH);
			}
			break;
		case SDLK_d:
			if(game->nb_player >= 2)
			{
				player_dec_moving(player[1]);
				player_unset_way(player[1], EAST);
			}
			break;
		case SDLK_q:
			if(game->nb_player >= 2)
			{
				player_dec_moving(player[1]);
				player_unset_way(player[1], WEST);
			}
			break;
		case SDLK_t:
			if(game->nb_player >= 3)
			{
				player_dec_moving(player[2]);
				player_unset_way(player[2], NORTH);
			}
			break;
		case SDLK_g:
			if(game->nb_player >= 3)
			{
				player_dec_moving(player[2]);
				player_unset_way(player[2], SOUTH);
			}
			break;
		case SDLK_h:
			if(game->nb_player >= 3)
			{
				player_dec_moving(player[2]);
				player_unset_way(player[2], EAST);
			}
			break;
		case SDLK_f:
			if(game->nb_player >= 3)
			{
				player_dec_moving(player[2]);
				player_unset_way(player[2], WEST);
			}
			break;
		case SDLK_i:
			if(game->nb_player >= 4)
			{
				player_dec_moving(player[3]);
				player_unset_way(player[3], NORTH);
			}
			break;
		case SDLK_k:
			if(game->nb_player >= 4)
			{
				player_dec_moving(player[3]);
				player_unset_way(player[3], SOUTH);
			}
			break;
		case SDLK_l:
			if(game->nb_player >= 4)
			{
				player_dec_moving(player[3]);
				player_unset_way(player[3], EAST);
			}
			break;
		case SDLK_j:
			if(game->nb_player >= 4)
			{
				player_dec_moving(player[3]);
				player_unset_way(player[3], WEST);
			}
			break;
		}
	}
	return GAME;
}
