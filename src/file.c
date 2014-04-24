#include <stdlib.h>
#include <stdio.h>
#include <map.h>
#include <file.h>
#include <assert.h>
#include <dirent.h>
#include <misc.h>

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
		return(fopen(lien, "w"));
		break;
	}
	return(NULL);
}

struct map* file_load_map(int n_level, int n_map) {

	int map_width = 0;
	int map_height = 0;
	int type = 0;
	char lien[250];

	sprintf(lien, "data/single/map_%d_%d.lvl", n_level, n_map);

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

struct map* file_load_map_multi(int n_map, int nb_players) {

	assert(1 <= nb_players && 4>= nb_players);

	int map_width = 0;
	int map_height = 0;
	int type = 0;
	char lien[250];

	DIR* dir = NULL;
	struct dirent* readfile = NULL;
	dir = opendir("./data/multi");

	if(!dir)
		printf("Error : unable to open data/multi\n");

	for(int i = 0; i <= n_map; i++) {
		readfile = readdir(dir);
	}

	//sprintf(lien, "data/multi/%s", readfile->d_name);
	sprintf(lien, "data/multi/test.lvl");

	if(closedir(dir) == -1)
		printf("Problème à la fermeture");

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

int file_map_exist(int n_level, int n_map) {
	char lien[50];
	sprintf(lien, "data/single/map_%d_%d.lvl", n_level, n_map);

	FILE* file = file_open(lien, READ);

	if(file == NULL)
		return 0;
	else
		return 1;
}
