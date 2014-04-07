#include <monster.h>
#include <list.h>
#include <assert.h>
#include <math.h>

struct monster {
	m_type type;
	way_t currentWay;
	int size;
	int life;
	int aggr;
	int moveTimer;
	int lifeTimer;
	int invincibility;
};


void monster_init(struct map* map, int x, int y, m_type type, int size, int life, int aggr, struct game* game)
{
	struct monster* monster = malloc( sizeof(*monster) );
	monster->type = type;
	monster->size = size;
	monster->life = life;
	monster->aggr= aggr;
	monster->currentWay = SOUTH;
	monster->moveTimer = game_get_frame(game);
	monster->lifeTimer = -1;
	monster->invincibility = 0;

	s_type typeL = LIST_MONSTER;

	map_insert_monster(map, x, y, typeL, monster);
}

int monster_get_currentway(struct monster* monster)
{
	assert(monster);
	return monster->currentWay;
}

void monster_set_currentway(struct monster* monster, way_t dir)
{
	assert(monster);
	monster->currentWay = dir;
}

int monster_get_movetimer(struct monster* monster)
{
	if(monster == NULL)
		return 0;
	return monster->moveTimer;
}

void monster_set_movetimer(struct monster* monster, int timer)
{
	assert(monster);
	monster->moveTimer = timer;
}

int monster_get_aggr(struct monster* monster)
{
	assert(monster);
	return monster->aggr;
}

int monster_get_nb_life(struct monster* monster) { // get nb_life
	if(monster == NULL)
		return 0;
	return monster->life;
}

void monster_set_nb_life(struct monster* monster, int life) { // get nb_life
	assert(monster);
	monster->life = life;
}

int monster_get_invincibility(struct monster* monster) { // get nb_life
	if(monster == NULL)
		return 0;
	return monster->invincibility;
}

void monster_set_invincibility(struct monster* monster, int invincibility) { // get nb_life
	assert(monster);
	monster->invincibility = invincibility;
}

int monster_get_life_timer(struct monster* monster) { // get nb_life
	if(monster == NULL)
		return 0;
	return monster->lifeTimer;
}

void monster_set_life_timer(struct monster* monster, int lifeTimer) { // get nb_life
	assert(monster);
	monster->lifeTimer = lifeTimer;
}

struct list* monster_dec_nb_life(struct list* mList, int x, int y, struct game* game) { // nb_life
	assert(game);

	if(mList == NULL) 
		return NULL;

	struct list* monster = list_find(mList, x, y);

	if(!monster) 
		return NULL;
	
	if(monster_get_nb_life(monster->data) > 0 && (monster_get_invincibility(monster->data) != 1 || monster_get_life_timer(monster->data) == -1) ) {
		monster_set_nb_life(monster->data, monster_get_nb_life(monster->data)-1);
		monster_set_life_timer(monster->data, game_get_frame(game));
		monster_set_invincibility(monster->data, 1);
	}
	if(monster_get_nb_life(monster->data) <= 0)
		mList = monster_kill(mList, x, y, level_get_curr_map(game_get_curr_level(game)));

	return mList;
}

struct list* monster_kill(struct list* mList, int x, int y, struct map* map)
{
	assert(mList);
	map_set_cell_type(map, x, y, CELL_EMPTY);
	return list_remove(mList, x, y);
}

static int monster_move_aux(struct map* map, int x, int y) {

	if (!map_is_inside(map, x, y))
		return 0;
	
	switch (map_get_cell_type(map, x, y)) {
	case CELL_SCENERY:
		return 0; // stop the function and the monster's movements
		break;

	case CELL_CASE: 
		return 0;
		break;

	case CELL_GOAL:
		break;

	case CELL_BOMB:
		return 0;
		break;

	case CELL_MONSTER:
		return 0;
		break;
	case CELL_DOOR:
		return 0;
		break;
	case CELL_BONUS:
		return 0;
		break;
	default:
		break;
	}

	// Monster has moved
	return 1;
}

int monster_move(struct list* mList, struct map* map, struct player* player, struct game* game) {
	int x = mList->x;
	int y = mList->y;
	int move = 0;
	way_t dir;

	int distMP = -1;

	// We set the cell type to monster again in case something erased it
	map_set_cell_type(map, mList->x, mList->y, CELL_MONSTER);


	// A monster moves every second
	if(!mList) 
		return 0;

	if(game_get_frame(game) - monster_get_movetimer(mList->data) < DEFAULT_GAME_FPS * 1)
		return 0;

	// We get the next direction for the monster and its distance between it and the player
	dir = monster_pathfinding(map, player, mList, &distMP);

	// If the distance is grater than the agressivity of the monster or if the player is unreachable, 
	// then the monster moves randomly
	if(distMP > monster_get_aggr(mList->data) || dir == -1)
		dir = rand_ab(0, 3);

	switch (dir) {
	case NORTH:
		if (monster_move_aux(map, x, y - 1)) {
			mList->y--;
			move = 1;
		}
		break;

	case SOUTH:
		if (monster_move_aux(map, x, y + 1)) {
			mList->y++;
			move = 1;
		}
		break;

	case WEST:
		if (monster_move_aux(map, x - 1, y)) {
			mList->x--;
			move = 1;
		}
		break;

	case EAST:
		if (monster_move_aux(map, x + 1, y)) {
			mList->x++;
			move = 1;
		}
		break;
	}

	if (move) {
		monster_set_currentway(mList->data, dir);
		monster_set_movetimer(mList->data, game_get_frame(game));
		if(map_get_cell_type(map, x, y) == CELL_MONSTER)
			map_set_cell_type(map, x, y, CELL_EMPTY);
		map_set_cell_type(map, mList->x, mList->y, CELL_MONSTER);
	}
	return move;
}

void monster_display(struct map* map, struct player* player, struct game* game)
{
	struct list* mList = map_get_monsters(map);

	while(mList != NULL) {
		if( monster_get_invincibility(mList->data) == 1 ) {
			if( (int)floor( (game_get_frame(game) - monster_get_life_timer(mList->data) ) )%4 == 0 )
				SDL_SetAlpha(sprite_get_monster(monster_get_currentway(mList->data)), SDL_SRCALPHA, 128);
			else
				SDL_SetAlpha(sprite_get_monster(monster_get_currentway(mList->data)), SDL_SRCALPHA, 192);
		}

		if( game_get_frame(game) - monster_get_life_timer(mList->data) > DEFAULT_GAME_FPS * 3 ) {
			monster_set_invincibility(mList->data, 0);
			SDL_SetAlpha(sprite_get_monster(monster_get_currentway(mList->data)), SDL_SRCALPHA, 255);
		}

		
		monster_move(mList, map, player, game);
		if(mList->x == player_get_x(player) && mList->y == player_get_y(player))
			player_hit(player, 3*DEFAULT_GAME_FPS);
		window_display_image(sprite_get_monster( monster_get_currentway(mList->data) ), mList->x * SIZE_BLOC, mList->y * SIZE_BLOC);
		mList = mList->next;
	}
}


int monster_pathfinding(struct map* map, struct player* player, struct list* mList, int* getDist)
{
	const int width = map_get_width(map);
	const int height = map_get_height(map);

	int loop = 1;

	int xSrc = mList->x;
	int ySrc = mList->y;
	int xDest = player_get_x(player);
	int yDest = player_get_y(player);

	int x = xSrc;
	int y = ySrc;

	int min = 99999999;
	int xMin = 0;
	int yMin = 0;

	int verif = 0;

	int weightArr[height][width][2]; 
		// weightArr[x coord][y coord][0] = node weight 
		// and weightArr[x coord][y coord][1] = true (respectively false) 
		// if we have already visited the node (respectively if we have not visited the node yet)
	int prevArr[height][width][2];
		// prevArr[x coord][y coord][0] = x coord of the previous node
		// and prevArr[x coord][y coord][1] = y coord of the previous node

	for(int i=0; i<height; i++)
	{
		for(int j=0; j<width; j++)
		{
			weightArr[i][j][1] = 0;

			if(j == xSrc && i == ySrc) {
				weightArr[i][j][0] = 0;
				weightArr[i][j][1] = 2;
			}
			else if( map_get_cell_type(map, j ,i) != CELL_MONSTER && map_get_cell_type(map, j ,i) != CELL_EMPTY) //&& map_get_cell_type(map, j, i) != CELL_PLAYER )
				weightArr[i][j][0] = -2;
			else
				weightArr[i][j][0] = -1;


			prevArr[i][j][0] = -1;
			prevArr[i][j][1] = -1;
		}
	}


	while(loop)
	{
		for(int i=-1; i<=1; i+=2)
		{
			for(int j=-1; j<=1; j+=2)
			{
				if( !((i<0 && (x+j < 0 || x+j >= width)) || (i>0 && (y+j < 0 || y+j >= height))) )
				{
					if(i<0)
					{
						if( !(weightArr[y][x+j][1] == 2 || weightArr[y][x+j][1] == 1 || weightArr[y][x+j][0] == -2) )
						{
							if( weightArr[y][x+j][0] == -1 || weightArr[y][x+j][0] > weightArr[y][x][0] + 1 )
							{
								weightArr[y][x+j][0] = weightArr[y][x][0] + 1;
								weightArr[y][x+j][1] = 1;
								prevArr[y][x+j][0] = x;
								prevArr[y][x+j][1] = y;
							}
						}
						if( x+j == xDest && y == yDest )
							loop = 0;
					}
					else
					{
						if( !(weightArr[y+j][x][1] == 2 || weightArr[y+j][x][0] == -2) )
						{
							if( weightArr[y+j][x][0] == -1 || weightArr[y+j][x][0] > weightArr[y][x][0] + 1 )
							{
								weightArr[y+j][x][0] = weightArr[y][x][0] + 1;
								weightArr[y+j][x][1] = 1;
								prevArr[y+j][x][0] = x;
								prevArr[y+j][x][1] = y;
							}
						}
						if( x == xDest && y+j == yDest )
							loop = 0;
					}
				}
			}
		}

		min = 999999;
		xMin = -1;
		yMin = -1;
		verif = 0;

		for(int i=0; i<height; i++)
		{
			for(int j=0; j<width; j++)
			{
				if(verif == 0 && weightArr[i][j][0] < min && weightArr[i][j][1] == 1)
				{
					min = weightArr[i][j][0];
					xMin = j;
					yMin = i;
				}
			}
		}

		x = xMin;
		y = yMin;

		if(x == -1 && y == -1)
			return -1;

		weightArr[yMin][xMin][1] = 2;
	}

	int loop2 = 1;
	x = xDest;
	y = yDest;

	int yInt = 0;
	int i = 0;

	while(loop2)
	{
		if( (prevArr[y][x][0] == xSrc && prevArr[y][x][1] == ySrc) || (prevArr[y][x][0] == -1 && prevArr[y][x][1] == -1) )
			loop2 = 0;
		else {
			i++;
			yInt = prevArr[y][x][1];
			x = prevArr[y][x][0];
			y = yInt;
		}
	}

	*getDist = i;

	if(xSrc > x)
		return WEST;
	else if(xSrc < x)
		return EAST;
	else if(ySrc > y)
		return NORTH;
	else if(ySrc < y)
		return SOUTH;
	else
		return -2;
}
