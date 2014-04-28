#include <multi.h>
#include <sprite.h>
#include <assert.h>
#include <window.h>
#include <misc.h>
#include <level.h>
#include <player.h>
#include <bomb.h>
#include <wiimote.h>


void multi_display(struct game* game) {
	assert(game);
	struct map* map = NULL;
	int w, h;
	int* scores;

	switch(game_get_state(game)) {
	case PLAYING:
	case PAUSED:
		game_display(game);
		if(player_get_nb_player_alive(game) == 1)
			multi_change_state(game, SCORE);

		break;
	case CHOOSE_MAP:

		map = level_get_curr_map(game_get_curr_level(game));

		w = 10 + 15 + sprite_get_max_width() + 50 + SIZE_BLOC * map_get_width(map);
		h = max(30 * (sprite_get_nb_map_multi() + 1), SIZE_BLOC * map_get_height(map));

		window_resize( w, h);
		window_clear();

		for(int i = 0; i < sprite_get_nb_map_multi(); i++) {
			window_display_image(	sprite_get_map_multi(i),
									10 + 15,
									15 + 30 * i);
		}

		window_display_image(sprite_get_menu(M_SELECT_BLACK), 10, 15 + 30 * game_get_pos(game));

		map_display(	map,
						10 + 15 + sprite_get_max_width() + 50,
						(h-(SIZE_BLOC * map_get_height(map))) / 2);
		window_refresh();

		break;
	case SCORE:

		window_clear();
		level_display(game_get_curr_level(game));
		bomb_display(game, level_get_curr_map(game_get_curr_level(game)));
		for(int i = 0; i < game_get_nb_player(game); i++)
			player_display(game_get_player(game, i+1));

		int map_w = map_get_width(level_get_curr_map(game_get_curr_level(game)));
		int map_h = map_get_height(level_get_curr_map(game_get_curr_level(game)));
		int mid_w = map_w / 2 * SIZE_BLOC + map_w%2 * SIZE_BLOC / 2;
		int mid_h = map_h / 2 * SIZE_BLOC + map_h%2 * SIZE_BLOC / 2;

		window_display_image(	sprite_get_menu(M_BG_GREY),
								mid_w - 240,
								mid_h - 262);

		window_display_image(	sprite_get_score(player_get_id_player_alive(game)),
								mid_w - 200,
								mid_h - 222);

		scores = game_get_scores(game);
		for(int i = 0; i < game_get_nb_player(game); i++) {
			window_display_sprite(	sprite_get_players(i+1),
									sprite_get_rect_player_anim(0, i+1, SOUTH),
									mid_w - 200,
									mid_h - 222 + 80 + 80 * i);

			window_display_image(	sprite_get_number_white(scores[i]),
									mid_w - 140,
									mid_h - 222 + 100 + 80 * i);
		}

		window_refresh();

		break;
	}
}

void multi_change_state(struct game* game, game_state_t state_to) {
	switch(game_get_state(game)) {
	case PLAYING:
	case PAUSED:
		switch(state_to) {
		case SCORE:
			game_set_state(game, SCORE);
			if(player_get_nb_player_alive(game) == 1)
				game_inc_score(game, player_get_id_player_alive(game));

			break;
		case CHOOSE_MAP:
			game_set_state(game, CHOOSE_MAP);
			game_reset_lvl_map(game);
			game_reset_scores(game);

			break;
		default:
			break;
		}
		break;

	case CHOOSE_MAP:
		switch(state_to) {
		case PLAYING:
			game_set_state(game, PLAYING);
			window_resize(map_get_width(level_get_curr_map(game_get_curr_level(game))) * SIZE_BLOC, map_get_height(level_get_curr_map(game_get_curr_level(game))) * SIZE_BLOC + BANNER_HEIGHT + LINE_HEIGHT);
			players_from_map(game, level_get_curr_map(game_get_curr_level(game)));
			player_reset(game, 1, 1, 2, 4, 0);

			break;
		default:
			break;
		}
		break;

	case SCORE:
		switch(state_to) {
		case PLAYING:
			game_set_state(game, PLAYING);
			game_reset_lvl_map(game);
			level_set_cur_map(game_get_curr_level(game), game_get_pos(game));
			players_from_map(game, level_get_curr_map(game_get_curr_level(game)));
			player_reset(game, 1, 1, 2, 4, 0);


			break;
		case CHOOSE_MAP:
			game_set_state(game, CHOOSE_MAP);
			game_reset_lvl_map(game);
			game_reset_scores(game);

			break;
		default:
			break;
		}

		break;
	default:
		break;
	}
}
