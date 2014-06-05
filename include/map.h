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
	BONUS_ROLLER, // 6
	BONUS_MONSTER, // 7
	BONUS_ALIEN1, // 8
	BONUS_ALIEN2, // 9
	BONUS_ALIEN3, // 10 (A)
	BONUS_ALIEN4, // 11 (B)
} bonus_type_t;

enum scenery_type {
	SCENERY_STONE, // 0
	SCENERY_TREE, // 1
	SCENERY_WALL_B, // 2
	SCENERY_WALL_G, // 3
	SCENERY_WALL_H, // 4
	SCENERY_WALL_D, // 5
	SCENERY_ANGLE_BG, // 6
	SCENERY_ANGLE_BD, // 7
	SCENERY_ANGLE_HG, // 8
	SCENERY_ANGLE_HD, // 9
	SCENERY_BLOC, // 10 (A)
};

enum door_type {
	CLOSED_DOOR = 0,
	OPENED_DOOR //1
};

typedef enum monster_type {
	MONSTER_NORMAL = 0,
	MONSTER_ALIEN1, // 1
	MONSTER_ALIEN2, // 2
	MONSTER_ALIEN3,  // 3
	MONSTER_ALIEN4  // 4
} m_type;

typedef enum players_numbers {
	PLAYER1 = 0,
	PLAYER2, //1
	PLAYER3, //2
	PLAYER4  //3
} players_t;

enum compose_type {
	CELL_STONE = CELL_SCENERY | (SCENERY_STONE << 4),			// 0000 0010 - 0 2
	CELL_TREE = CELL_SCENERY | (SCENERY_TREE << 4),				// 0001 0010 - 1 2

    CELL_CASE_RANGE_INC = CELL_CASE | (BONUS_RANGE_INC << 4),	// 0001 0100 - 1 4
    CELL_CASE_RANGE_DEC = CELL_CASE | (BONUS_RANGE_DEC << 4),	// 0010 0100 - 2 4
	CELL_CASE_BOMB_INC = CELL_CASE | (BONUS_BOMB_INC << 4),		// 0011 0100 - 3 4 = 52
    CELL_CASE_BOMB_DEC = CELL_CASE | (BONUS_BOMB_DEC << 4),		// 0100 0100 - 4 4 = 68
    CELL_CASE_LIFE = CELL_CASE | (BONUS_LIFE << 4),				// 0101 0100 - 5 4 = 84
    CELL_CASE_ROLLER = CELL_CASE | (BONUS_ROLLER << 4),			// 0111 0100 - 6 4 = 116
    CELL_CASE_MONSTER = CELL_CASE | (BONUS_MONSTER << 4),		// 0110 0100 - 7 4 = 100
    CELL_CASE_ALIEN1 = CELL_CASE | (BONUS_MONSTER << 4),		// 0110 0100 - 8 4
    CELL_CASE_ALIEN2 = CELL_CASE | (BONUS_MONSTER << 4),		// 0110 0100 - 9 4
    CELL_CASE_ALIEN3 = CELL_CASE | (BONUS_MONSTER << 4),		// 0110 0100 - A 4
    CELL_CASE_ALIEN4 = CELL_CASE | (BONUS_MONSTER << 4),		// 0110 0100 - B 4

    CELL_BONUS_RANGE_INC = CELL_BONUS | (BONUS_RANGE_INC << 4),	// 0001 0101 - 1 5
    CELL_BONUS_RANGE_DEC = CELL_BONUS | (BONUS_RANGE_DEC << 4),	// 0010 0101 - 2 5
    CELL_BONUS_BOMB_INC = CELL_BONUS | (BONUS_BOMB_INC << 4),	// 0011 0101 - 3 5
    CELL_BONUS_BOMB_DEC = CELL_BONUS | (BONUS_BOMB_DEC << 4),	// 0100 0101 - 4 5
    CELL_BONUS_LIFE = CELL_BONUS | (BONUS_LIFE << 4),			// 0101 0101 - 5 5
    CELL_BONUS_ROLLER = CELL_BONUS | (BONUS_ROLLER << 4),		// 0111 0101 - 6 5 = 117

    CELL_CLOSED_DOOR = CELL_DOOR | (CLOSED_DOOR << 7),			// 0000 1001 - 0 9
    CELL_OPENED_DOOR = CELL_DOOR | (OPENED_DOOR << 7),			// 1000 1001 - 1 9 128 +9 = 137

    CELL_MONSTER_NORMAL = CELL_MONSTER | (MONSTER_NORMAL << 4),	// 0000 0110 - 0 6 = 6
    CELL_MONSTER_ALIEN1 = CELL_MONSTER | (MONSTER_ALIEN1 << 4),	// 0001 0110 - 1 6 = 22
    CELL_MONSTER_ALIEN2 = CELL_MONSTER | (MONSTER_ALIEN2 << 4),	// 0010 0110 - 2 6
    CELL_MONSTER_ALIEN3 = CELL_MONSTER | (MONSTER_ALIEN3 << 4),	// 0011 0110 - 3 6
    CELL_MONSTER_ALIEN4 = CELL_MONSTER | (MONSTER_ALIEN4 << 4),	// 0011 0110 - 4 6

    CELL_PLAYER_1 = CELL_PLAYER | (PLAYER1 << 4),				// 0000 0011 - 0 3 = 3
    CELL_PLAYER_2 = CELL_PLAYER | (PLAYER2 << 4),				// 0001 0011 - 1 3 = 19
    CELL_PLAYER_3 = CELL_PLAYER | (PLAYER3 << 4),				// 0010 0011 - 2 3 = 35
    CELL_PLAYER_4 = CELL_PLAYER | (PLAYER4 << 4),				// 0011 0011 - 3 3 = 51

    CELL_FLAG = 0x01,
    CELL_PRINCESS = 0x11,
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
unsigned char map_get_cell_compose_type(struct map* map, int x, int y);

// Set the type of a cell
void  map_set_cell_type(struct map* map, int x, int y, cell_type_t type);

// Destroy a case and set the type of cell to a bonus cell
void map_case_destroyed(struct game* game, struct map* map, int x, int y);

// Test if (x,y) is within the map
int map_is_inside(struct map* map, int x, int y);

// Display the map on the screen
void map_display(struct map* map, int offset_x, int offset_y);


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



