#include <multi.h>
#include <sprite.h>
#include <assert.h>
#include <window.h>
#include <misc.h>

void multi_display(struct game* game) {
	assert(game);

	if(game_get_state(game) == PLAYING || game_get_state(game) == PAUSED) {
		game_display(game);
	}
	else if(game_get_state(game) == CHOOSE_MAP) {

		struct map* map = level_get_curr_map(game_get_curr_level(game));

		int w = 10 + 15 + sprite_get_max_width() + 50 + SIZE_BLOC * map_get_width(map);
		int h = max(30 * (sprite_get_nb_map_multi() + 1), SIZE_BLOC * map_get_height(map));

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
	}
}
