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
#include <multi.h>

struct game {
	int nb_curr_level;
	struct level* curr_level; // current level
	struct player* players[4];
	enum game_state game_state;
	int nb_player;
	int pos;
	int scores[4];
	int score_obj;
};

struct game* game_new(int curr_lvl, int nb_player) {
	struct game* game = malloc(sizeof(*game));
	game->nb_curr_level = curr_lvl;

	game->nb_player = nb_player;
	game->curr_level = level_get_level(game, game->nb_curr_level, 0); // get maps of the level 0, map 0

	if(nb_player == 1) {
		game->players[0] = player_init(1, 1, 2, 1); // player init : #1, nb_bomb, nb_life and nb_range
		players_from_map(game, level_get_curr_map(game->curr_level)); // get x,y of the player on the first map
	}
	else {
		for(int i=0; i<nb_player; i++) {
			game->players[i] = player_init(i+1, 1, 1, 2); // player init : #1, nb_bomb, nb_life and nb_range
			game->scores[i] = 0;
		}
	}

	game->game_state = PLAYING;
	game->pos = 0;
	game->score_obj = 3;

	return game;
}

struct level* game_next_lvl(struct game* game) {
	assert(game);
	game->nb_curr_level++;
	game->curr_level = level_get_level(game, game->nb_curr_level, 0);
	return(game->curr_level);
}

void game_reset_lvl_map(struct game* game) {
	assert(game);
	int n_map = level_get_curr_nb_map(game_get_curr_level(game));
	level_free(game->curr_level);
	game->curr_level = level_get_level(game, game->nb_curr_level, n_map);
}

void game_free(struct game* game) {
	assert(game);

	for(int i=0; i<game->nb_player; i++)
		player_free(game->players[i]);
	level_free(game->curr_level);
	free(game);
}

enum game_state game_get_state(struct game* game) {
	assert(game);
	return(game->game_state);
}

void game_set_state(struct game* game, enum game_state state) {
	assert(game);
	game->game_state = state;
}

int game_get_nb_player(struct game* game) {
	assert(game);
	return game->nb_player;
}

int game_get_pos(struct game* game) {
	assert(game);
	return game->pos;
}

struct player** game_get_players(struct game* game) {
	assert(game && game->players);
	return(game->players);
}

struct player* game_get_player(struct game* game, int id) {
	assert(game);
	assert(id > 0 && id <= game->nb_player);
	return game->players[id-1];
}

struct level* game_get_curr_level(struct game* game) {
	return game->curr_level;
}

int* game_get_scores(struct game* game) {
	assert(game);
	return game->scores;
}

void game_inc_score(struct game* game, int id) {
	assert(game);
	game->scores[id-1]++;
}

void game_reset_scores(struct game* game) {
	assert(game);
	for(int i = 0; i < 4; i++)
		game->scores[i] = 0;
}

/* Display the game's interface */
void game_banner_display(struct game* game) {
	assert(game);
	struct level* level = game->curr_level;
	struct map* map = level_get_curr_map(level);

	if(game->nb_player <= 1) {
		struct player* player = game->players[0];

		int y = (map_get_height(map)) * SIZE_BLOC;
		for (int i = 0; i < map_get_width(map); i++)
			window_display_image(sprite_get_banner_line(), i * SIZE_BLOC, y);

		int white_bloc = ((map_get_width(map) * SIZE_BLOC) - 9 * SIZE_BLOC) / 6;
		int x = white_bloc;
		y = (map_get_height(map) * SIZE_BLOC) + LINE_HEIGHT;

		window_display_image(sprite_get_number(game->nb_curr_level), x, y);

		x += SIZE_BLOC - 5;
		window_display_image(sprite_get_number(10), x, y);

		x += 10;
		window_display_image(sprite_get_number(level_get_curr_nb_map(level)),x, y);

		x += white_bloc + SIZE_BLOC;
		window_display_image(sprite_get_banner_life(), x, y); // sprite life

		x += SIZE_BLOC;
		window_display_image(
				sprite_get_number(player_get_nb_life(game_get_player(game, 1))), x, y); // life number

		x += white_bloc + SIZE_BLOC;
		window_display_image(sprite_get_banner_bomb(), x, y); // bomb sprite

		x += SIZE_BLOC;
		window_display_image(
				sprite_get_number(player_get_nb_bomb(game_get_player(game, 1))), x, y); // bomb number

		x += white_bloc + SIZE_BLOC;
		window_display_image(sprite_get_banner_range(), x, y); // range sprite

		x += SIZE_BLOC;
		window_display_image(
				sprite_get_number(player_get_nb_range(game_get_player(game, 1))), x, y); // range number

		x += white_bloc + SIZE_BLOC;
		if(player_get_key(player))
			window_display_image(sprite_get_key(), x, y);
	}
	else {
		int y = (map_get_height(map)) * SIZE_BLOC;
		for (int i = 0; i < map_get_width(map); i++)
			window_display_image(sprite_get_banner_line(), i * SIZE_BLOC, y);

		y = (map_get_height(map) * SIZE_BLOC) + LINE_HEIGHT;
		int white_bloc = ((map_get_width(map) * SIZE_BLOC) - 2*game->nb_player * SIZE_BLOC) / (game->nb_player + 1);
		int x = 0;
		for(int i = 0; i < game->nb_player; i++) {
			x+= white_bloc;
			window_display_sprite(	sprite_get_players(i+1),
									sprite_get_rect_player_anim(0, i+1, SOUTH),
									x, y + 2);

			x+= SIZE_BLOC;
			window_display_image(sprite_get_number(game->scores[i]), x, y);

			x+=SIZE_BLOC;
		}
	}
}

void game_order_players_array(struct game* game, struct player* player[4])
{
	int min = 9999999;
	int posPLayer = 0;
	struct player* auxPlayer;
	int id = 0;
	for(int i=0; i<game->nb_player; i++) {
		for(int j=i; j<game->nb_player; j++) {
			posPLayer = player_get_y_real(player[j]);
			if( posPLayer < min) {
				min = posPLayer;
				id = j;
			}
		}
		auxPlayer = player[id];
		player[id] = player[i];
		player[i] = auxPlayer;
		min = 9999999;
	}
}

void game_display(struct game* game) {
	assert(game);

	window_clear();

	level_display(game_get_curr_level(game));

	monster_display(level_get_curr_map(game->curr_level));

	bomb_display(game, level_get_curr_map(game->curr_level));
	
	game_banner_display(game);

	if(game->nb_player == 1) { // Single player
		struct player* player = game->players[0];

		// Always display
		player_display(player);

		if(game->game_state == PLAYING) {
			player_move(game, player, level_get_curr_map(game->curr_level));
			monster_move(level_get_curr_map(game->curr_level), player);

			player_update(player);
			monster_update(level_get_curr_map(game->curr_level));
		}
	}
	else { // Multi player
		struct player* players_in_order[game->nb_player];
		for(int i=0; i<game->nb_player; i++)
			players_in_order[i] = game->players[i];
		game_order_players_array(game, players_in_order);

		for(int i = 0; i < game->nb_player; i++) {

			player_display(players_in_order[i]);

			if(game->game_state == PLAYING) {
				player_move(game, players_in_order[i], level_get_curr_map(game->curr_level));

				player_update(players_in_order[i]);
			}
		} // end for each player


	} // end Multi player

	if(game->game_state == PLAYING) {

		bomb_update(level_get_curr_map(game->curr_level));

	}
	else if(game->game_state == PAUSED) {
		int map_w = map_get_width(level_get_curr_map(game->curr_level));
		int map_h = map_get_height(level_get_curr_map(game->curr_level));
		int mid_w = map_w / 2 * SIZE_BLOC + map_w%2 * SIZE_BLOC / 2;
		int mid_h = map_h / 2 * SIZE_BLOC + map_h%2 * SIZE_BLOC / 2;
		menu_display(mid_w, mid_h);

	}
	window_refresh();
}

/* Return score max if select = 0, id of player max score if select = 1 */
int game_get_score_max(struct game* game, int select) {
	assert(game);
	int max = game->scores[0];
	int id = 0;

	for(int i = 1; i < game->nb_player; i++) {
		if(game->scores[i] > max) {
			max = game->scores[i];
			id = i+1;
		}
	}
	if(select)
		return id;
	else
		return max;
}

enum state game_update(enum state state, struct game* game, int key, key_event_t key_event) {
	//struct player* player = game_get_player(game, 1);
	struct player* player[game->nb_player];
	for(int i=0; i<game->nb_player; i++) 
	{
		player[i] = game->players[i];
	}
	struct map* map = level_get_curr_map(game_get_curr_level(game));

	if(key_event == DOWN) {
		switch (key) {
		case SDLK_p:
			if(game->game_state == CHOOSE_MAP)
				break;
		/* no break */
		case SDLK_ESCAPE:
		 // Pause
			if(game->game_state == PLAYING) {
				if(game->nb_player == 1)
					new_menu(PAUSE_SINGLE);
				else
					new_menu(PAUSE_MULTI);
				game->game_state = PAUSED;
			}
			else if(game->game_state == PAUSED){
				menu_free(NULL);
				game->game_state = PLAYING;
			}
			else if(game->game_state == CHOOSE_MAP) {
				return ENDGAME;
			}
			return GAME;
			break;
		case SDLK_RETURN:
		case SDLK_KP_ENTER:
			switch(game->game_state) {
			case PAUSED:

				switch(menu_update(state, key, key_event)) {
				case KEEP:
					menu_free(NULL);
					game->game_state = PLAYING;
					break;
				case MAINMENU:
					return ENDGAME;
					break;
				case QUIT:
					return(QUIT);
					break;
				case CHANGEMAP:
					multi_change_state(game, CHOOSE_MAP);
					break;
				default:
					break;
				}

			break;
			case CHOOSE_MAP:
				multi_change_state(game, PLAYING);

				break;
			case SCORE:
				if(game_get_score_max(game, 0) < game->score_obj)
					multi_change_state(game, PLAYING);
				else
					multi_change_state(game, CHOOSE_MAP);

				break;
			default:
				break;
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
			else if (game->game_state == PAUSED)
				return(menu_update(state, key, key_event));
			else if(game->game_state == CHOOSE_MAP) {
				if(game->pos > 0) {
					game->pos--;
					level_set_cur_map(game_get_curr_level(game), game->pos);
				}
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
			else if(game->game_state == CHOOSE_MAP) {
				if(game->pos < sprite_get_nb_map_multi() - 1) {
					game->pos++;
					level_set_cur_map(game_get_curr_level(game), game->pos);
				}
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
		case SDLK_F1:
			player_reset_way_mov(game->players[0]);
			break;
		case SDLK_F2:
			player_reset_way_mov(game->players[1]);
			break;
		case SDLK_F3:
			player_reset_way_mov(game->players[2]);
			break;
		case SDLK_F4:
			player_reset_way_mov(game->players[3]);
			break;
		}
	}
	return GAME;
}
