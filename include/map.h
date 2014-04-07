#ifndef MAP_H_
#define MAP_H_

#define MAX_BOMB 9

typedef enum cell_type {
	CELL_EMPTY=0,
	CELL_GOAL,  // 1
	CELL_SCENERY, // 2
	CELL_PLAYER, // 3
	CELL_CASE,   // 4
	CELL_BONUS, // 5
	CELL_MONSTER, // 6
	CELL_BOMB, // 7
	CELL_KEY, // 8
	CELL_DOOR, // 9
} cell_type_t;

typedef enum bonus_type {
	BONUS_RANGE_INC=1,
	BONUS_RANGE_DEC, // 2
	BONUS_BOMB_INC, // 3
	BONUS_BOMB_DEC, // 4
	BONUS_LIFE, // 5
	BONUS_MONSTER // 6
} bonus_type_t;

enum scenery_type {
	SCENERY_STONE, // 0
	SCENERY_TREE, // 1
};

enum door_type {
	CLOSED_DOOR = 0,
	OPENED_DOOR //1
};

enum compose_type {
	CELL_STONE = CELL_SCENERY | (SCENERY_STONE << 4),			// 0000 0010 - 0 2
	CELL_TREE = CELL_SCENERY | (SCENERY_TREE << 4),				// 0001 0010 - 1 2

    CELL_CASE_RANGE_INC = CELL_CASE | (BONUS_RANGE_INC << 4),	// 0000 0100 - 1 4
    CELL_CASE_RANGE_DEC = CELL_CASE | (BONUS_RANGE_DEC << 4),	// 0001 0100 - 2 4
	CELL_CASE_BOMB_INC = CELL_CASE | (BONUS_BOMB_INC << 4),		// 0010 0100 - 3 4
    CELL_CASE_BOMB_DEC = CELL_CASE | (BONUS_BOMB_DEC << 4),		// 0011 0100 - 4 4
    CELL_CASE_LIFE = CELL_CASE | (BONUS_LIFE << 4),				// 0100 0100 - 5 4
    CELL_CASE_MONSTER = CELL_CASE | (BONUS_MONSTER << 4),		// 0101 0100 - 6 4

    CELL_BONUS_RANGE_INC = CELL_BONUS | (BONUS_RANGE_INC << 4),	// 0001 0101 - 1 5
    CELL_BONUS_RANGE_DEC = CELL_BONUS | (BONUS_RANGE_DEC << 4),	// 0010 0101 - 2 5
    CELL_BONUS_BOMB_INC = CELL_BONUS | (BONUS_BOMB_INC << 4),	// 0011 0101 - 3 5
    CELL_BONUS_BOMB_DEC = CELL_BONUS | (BONUS_BOMB_DEC << 4),	// 0100 0101 - 4 5
    CELL_BONUS_LIFE = CELL_BONUS | (BONUS_LIFE << 4),			// 0101 0100 - 5 5

    CELL_CLOSED_DOOR = CELL_DOOR | (CLOSED_DOOR << 7),			// 0000 1001 - 0 9
    CELL_OPENED_DOOR = CELL_DOOR | (OPENED_DOOR << 7)			// 1000 1001 - 1 9 128 +9 = 137
};

typedef enum type_struct {
	LIST_MONSTER,
	LIST_BOMB
} s_type;

struct map;
struct game;

// Create a new empty map
struct map* map_new(int width, int height);
void map_free(struct map* map);

// Return the height and width of a map
int map_get_width(struct map* map);
int map_get_height(struct map* map);

// Return the type of a cell
cell_type_t map_get_cell_type(struct map* map, int x, int y);

// Return the type of a cell without masking the least significant bit
char map_get_cell_compose_type(struct map* map, int x, int y);

// Set the type of a cell
void  map_set_cell_type(struct map* map, int x, int y, cell_type_t type);

// Destroy a case and set the type of cell to a bonus cell
void map_case_destroyed(struct game* game, struct map* map, int x, int y);

// Test if (x,y) is within the map
int map_is_inside(struct map* map, int x, int y);

// Return a default static map
struct map* map_get_default(int n);

// Display the map on the screen
void map_display(struct map* map);


// Monsters functions

// Insert a monster on the map
void map_insert_monster(struct map* map, int x, int y, s_type type, void* data);
// Load the monsters' list
struct list* map_load_monsters(struct map* map, struct game* game);
// Return the monsters' list
struct list* map_get_monsters(struct map* map);
// Change the monsters' list
void map_set_monsters(struct map* map, struct list* mList);

// Bombs functions

// Insert a bomb on the map
void map_insert_bomb(struct map* map, int x, int y, s_type type, void* data);
// Return the bombs' list
struct list* map_get_bombs(struct map* map);
// Change the bombs' list
void map_set_bombs(struct map* map, struct list* bList);

#endif /* MAP_H_ */



