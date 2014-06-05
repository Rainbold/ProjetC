#include <stdlib.h>
#include <stdio.h>
#include <map.h>
#include <file.h>
#include <assert.h>
#include <dirent.h>
#include <misc.h>
#include <game.h>
#include <level.h>
#include <list.h>
#include <monster.h>

typedef enum mode {
	READ = 0,
	WRITE // 1
}mode_file;

FILE* file_open(char* lien, mode_file mode) {
	assert(lien);
	switch(mode) {
	case READ:
		return(fopen(lien, "r"));
		break;
	case WRITE:
		return(fopen(lien, "w+"));
		break;
	}
	return(NULL);
}

struct map* file_read_map1(const char* mapFile) {
   FILE* f = NULL;
   unsigned char byte = 0;

   int i;
   int j;

   f = fopen(mapFile, "rb");

   if(!f)
	   return(NULL);

   if( fread(&byte, 1, sizeof(byte), f) == 0 || byte != 0x6c )
      return 0;
   if( fread(&byte, 1, sizeof(byte), f) == 0 || byte != 0x76 )
      return 0;
   if( fread(&byte, 1, sizeof(byte), f) == 0 || byte != 0x6c )
      return 0;

   if( fread(&byte, 1, sizeof(byte), f) == 0 )
	   return(NULL);
   int map_width = byte;

   if( fread(&byte, 1, sizeof(byte), f) == 0 )
	   return(NULL);
   int map_height = byte;

   struct map* map = map_new(map_width, map_height);

	for(j=0; j<map_height; j++) {
   		for(i=0; i<map_width; i++) {
		   	if( fread(&byte, 1, sizeof(byte), f) == 0 )
				return(NULL);
		  	map_set_cell_type(map, i, j, byte);
	   	}
   	}

   fclose(f);

   return map;
}

struct map* file_read_map2(char* lien) {
	int map_width = 0;
	int map_height = 0;
	int type = 0;
	FILE* map_file = file_open(lien, READ);

	if(map_file == NULL)
		return NULL;

	fscanf(map_file, "%d:%d", &map_width, &map_height);

	if(map_width <= 0 || map_height <= 0) // todo : taille max des maps
		return NULL;

	struct map* map = map_new(map_width, map_height);

	for(int y = 0; y < map_height - 1; y++) {
		for(int x = 0; x < map_width - 1; x++) {
			fscanf(map_file, "%d ", &type);
			//printf("%d.%d:%d ", x, y, type);
			map_set_cell_type(map, x, y, type);
		}
		fscanf(map_file, "%d\n", &type);
		map_set_cell_type(map, map_width - 1, y, type);
		//printf("%d.%d:%d\n", map_width - 1, y, type);
	}

	for(int x = 0; x < map_width - 1; x++) {
		fscanf(map_file, "%d ", &type);
		map_set_cell_type(map, x, map_height - 1, type);
		//printf("%d.%d:%d ", x, map_height - 1, type);
	}
	fscanf(map_file, "%d", &type);
	map_set_cell_type(map, map_width - 1, map_height - 1, type);
	//printf("%d.%d:%d\n", map_width - 1, map_height - 1, type);

	fclose(map_file);
	return map;
}

struct map* file_load_map(int n_level, int n_map) {

	char lien[250];

	sprintf(lien, "data/single/map_%d_%d.lvl", n_level, n_map);

	int verif1 = map_is_valid_format1(lien);
	int verif2 = map_is_valid_format2(lien);

	if(verif1 && verif2) {
		// Binary file
		return(file_read_map1(lien));
	}
	else if(!verif1 && verif2) {
		// Text file
		return(file_read_map2(lien));
	}
	else
		return NULL;
}

struct map* file_load_map_multi(int n_map, int nb_players) {

	assert(1 <= nb_players && 4>= nb_players);

	int map_width = 0;
	int map_height = 0;
	int type = 0;
	char lien[250];

	DIR* dir = NULL;
	struct dirent* readfile = NULL;
	dir = opendir("data/multi");

	if(!dir)
		printf("Error : unable to open data/multi\n");

	for(int i = 0; i <= n_map; i++) {
		readfile = readdir(dir);
		if(!map_is_valid_format2(readfile->d_name)) {
			i--;
		}
	}

	sprintf(lien, "data/multi/%s", readfile->d_name);

	if(closedir(dir) == -1)
		printf("Problème à la fermeture");

	FILE* map_file = file_open(lien, READ);

	if(map_file == NULL)
		return NULL;

	fscanf(map_file, "%d:%d\n", &map_width, &map_height);

	if(map_width <= 0 || map_height <= 0) // todo : taille max des maps
		return NULL;

	struct map* map = map_new(map_width, map_height);

	for(int y = 0; y < map_height - 1; y++) {
		for(int x = 0; x < map_width - 1; x++) {
			fscanf(map_file, "%d ", &type);
			//printf("%d.%d:%d ", x, y, type);
			map_set_cell_type(map, x, y, type);
		}
		fscanf(map_file, "%d\n", &type);
		map_set_cell_type(map, map_width - 1, y, type);
		//printf("%d.%d:%d\n", map_width - 1, y, type);
	}

	for(int x = 0; x < map_width - 1; x++) {
		fscanf(map_file, "%d ", &type);
		map_set_cell_type(map, x, map_height - 1, type);
		//printf("%d.%d:%d ", x, map_height - 1, type);
	}
	fscanf(map_file, "%d", &type);
	map_set_cell_type(map, map_width - 1, map_height - 1, type);
	//printf("%d.%d:%d\n", map_width - 1, map_height - 1, type);

	fclose(map_file);
	return map;
}

int file_map_exist(int n_level, int n_map) {
	char lien[50];
	sprintf(lien, "data/single/map_%d_%d.lvl", n_level, n_map);

	FILE* file = file_open(lien, READ);

	if(file == NULL) {
		return 0;
	}
	else {
		fclose(file);
		return 1;
	}
}

void file_savegame(struct game* game) {
	assert(game);

	struct player* player = game_get_player(game, 1);
	struct level* level = game_get_curr_level(game);
	struct map* map = level_get_curr_map(level);

	FILE* file_map = file_open("data/savegame/map.save", WRITE);
	FILE* file_data = file_open("data/savegame/data.save", WRITE);

	if(file_map == NULL || file_data == NULL) {
		printf("Error : Savegame - Cannot open files\n");
		return;
	}

	// game
	int nb_curr_level = game_get_nb_curr_level(game);
	fprintf(file_data, "%d\n", nb_curr_level);

	// player
	int x = player_get_x(player);
	int y = player_get_y(player);
	int nb_bomb = player_get_nb_bomb(player);
	int nb_life = player_get_nb_range(player);
	int nb_range = player_get_nb_range(player);
	int velocity = player_get_velovity(player);
	int key = player_get_key(player);
	fprintf(file_data, "%d %d %d %d %d %d %d\n", x, y, nb_bomb, nb_life, nb_range, velocity, key);

	//level
	int cur_map = level_get_curr_nb_map(level);
	fprintf(file_data, "%d", cur_map);

	// Monsters
	struct list* mList = map_get_monsters(map);
	while(mList != NULL) {
		struct monster* monster = list_get_data(mList);
		map_set_cell_type(map, list_get_x(mList), list_get_y(mList), CELL_MONSTER | (monster_get_type(monster) << 4));
		mList = list_get_next(mList);
	}

	int map_width = map_get_width(map);
	int map_height = map_get_height(map);

	fprintf(file_map, "%d:%d\n", map_width, map_height);

	for(int j = 0; j < map_height - 1; j++) {
		for(int i = 0; i < map_width - 1; i++)
			fprintf(file_map, "%d ", map_get_cell_compose_type(map, i, j));

		fprintf(file_map, "%d\n", map_get_cell_compose_type(map, map_width - 1, j));
	}

	for(int i = 0; i < map_width - 1; i++)
		fprintf(file_map, "%d ", map_get_cell_compose_type(map, i, map_height - 1));

	fprintf(file_map, "%d", map_get_cell_compose_type(map, map_width - 1, map_height - 1));

	fclose(file_map);
	fclose(file_data);
}

int file_loadgame(struct game* game) {
	assert(game);

	FILE* file_data = file_open("data/savegame/data.save", READ);

	if(file_data == NULL) {
		printf("Error : Loadgame - Cannot open files\n");
		return 1;
	}
	// game
	int nb_curr_level;
	fscanf(file_data, "%d\n", &nb_curr_level);

	// player
	int x, y, nb_bomb, nb_life, nb_range, velocity, key;
	fscanf(file_data, "%d %d %d %d %d %d %d\n", &x, &y, &nb_bomb, &nb_life, &nb_range, &velocity, &key);

	//level
	int cur_map;
	fscanf(file_data, "%d", &cur_map);

	if(nb_curr_level < 0 || x < 0 || y < 0 || nb_bomb < 0 || nb_range < 0 || nb_life <= 0 || velocity < 0 || key < 0 || cur_map < 0) {
		printf("Savegame corrupted\n");
		return 2;
	}

	game_set_nb_curr_level(game, nb_curr_level);
	game_set_curr_level(game, level_get_level(game, nb_curr_level, cur_map));
	game_set_player(game, 0, player_init(1, nb_bomb, nb_life, nb_range, velocity)); // player init : #1, nb_bomb, nb_life, nb_range and velocity

	player_set_x(game_get_player(game, 1), x);
	player_set_y(game_get_player(game, 1), y);

	struct map* map = level_get_curr_map(game_get_curr_level(game));
	map_free(map);

	map = file_read_map2("data/savegame/map.save");
	level_set_cur_map_struct(game_get_curr_level(game), cur_map, map);

	map_load_monsters(map, game);

	fclose(file_data);
	return 0;
}
