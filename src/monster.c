#include <monster.h>
#include <list.h>
#include <assert.h>
#include <math.h>

struct monster {
	// Common data
	m_type type;
	way_t currentWay;

	// Timers
	int moveTimer;		// timer used to change direction when not following a player
	int invicibility;	// timer used to invicibility
	int replicator_timer;

	// Display data
	int moving;
	int x_sprite, y_sprite;
	int velocity;
	int anim;

	// Specific data
	int size;
	int nb_life;
	int aggr;			// path max to find the player
	int velocity_aggr;
	int velocity_norm;
	int replicator;
};

void monster_init(struct map* map, int x, int y, m_type type)
{
	// Common data
	struct monster* monster = malloc( sizeof(*monster) );
	monster->type = type; // type of monster
	monster->currentWay = SOUTH;

	monster->moveTimer = 0;
	monster->invicibility = 0;

	monster->moving = 1;
	monster->x_sprite = 0;
	monster->y_sprite = 0;
	monster->anim = 0;

	switch(type) {
	case MONSTER_NORMAL:
		monster->size = 1;
		monster->nb_life = 1;
		monster->aggr = 5;
		monster->velocity_norm = 1;
		monster->velocity_aggr = 2;
		monster->replicator_timer = 0;
		monster->replicator = 0;

		break;
	case MONSTER_ALIEN1:
		monster->size = 1;
		monster->nb_life = 2;
		monster->aggr = 5;
		monster->velocity_norm = 0;
		monster->velocity_aggr = 3;
		monster->replicator_timer = 0;
		monster->replicator = 0;

		break;
	case MONSTER_ALIEN2:
		monster->size = 1;
		monster->nb_life = 1;
		monster->aggr = 2;
		monster->velocity_norm = 4;
		monster->velocity_aggr = 4;
		monster->replicator_timer = 0;
		monster->replicator = 0;

		break;
	case MONSTER_ALIEN3:
		monster->size = 1;
		monster->nb_life = 2;
		monster->aggr = 50;
		monster->velocity_norm = 1;
		monster->velocity_aggr = 1;
		monster->replicator_timer = 0;
		monster->replicator = 0;

		break;
	case MONSTER_ALIEN4:
		monster->size = 1;
		monster->nb_life = 1;
		monster->aggr = 0;
		monster->velocity_norm = 1;
		monster->velocity_aggr = 1;
		monster->replicator_timer = 10 * DEFAULT_GAME_FPS;
		monster->replicator = monster->replicator_timer;

			break;
	default:
		break;
	}

	monster->velocity = monster->velocity_norm;

	map_insert_monster(map, x, y, LIST_MONSTER, monster);
}

m_type monster_get_type(struct monster* monster) {
	assert(monster);
	return monster->type;
}

struct list* monster_hit(struct list* mList, int x, int y, struct game* game) { // nb_life
	if(mList == NULL) 
		return NULL;
	struct list* l_return = mList;
	struct list* l = list_find(mList, x, y);

	if(!l)
		return NULL;
	
	struct monster* monster = list_get_data(l);

	if(!monster->invicibility) {
		if(monster->nb_life > 1) {
			monster->nb_life--;
			monster->invicibility = 3 * DEFAULT_GAME_FPS;
			monster->moving = 0;
		}
		else
			l_return = monster_kill(mList, x, y, level_get_curr_map(game_get_curr_level(game)));
	}
	return l_return;
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
	int cell = map_get_cell_type(map, x, y);
	switch (cell) {
	case CELL_SCENERY:
	case CELL_CASE:
	case CELL_BOMB:
	case CELL_MONSTER:
	case CELL_DOOR:
	case CELL_BONUS:
		return 0; // stop the function and the monster's movements
		break;
	case CELL_GOAL:
		break;
	default:
		break;
	}

	// Monster has moved
	return 1;
}

void monster_move(struct game* game, struct map* map, struct player* player) {
	struct list* mList = map_get_monsters(map);

	while(mList != NULL) {
		struct monster* monster = list_get_data(mList);
		int x = list_get_x(mList);
		int y = list_get_y(mList);

		if(monster->moving) {
			int distMP = -1;
			if(monster->moveTimer <= 0) {
				monster->currentWay = monster_pathfinding(map, player, mList, &distMP);

				// If the distance is grater than the agressivity of the monster or if the player is unreachable,
				// then the monster moves randomly
				if((distMP > monster->aggr || monster->currentWay == -1)) {
						monster->currentWay = rand_ab(0, 3);
						monster->velocity = monster->velocity_norm;
				}
				else
					monster->velocity = monster->velocity_aggr;

				if(monster->velocity != 0)
					monster->moveTimer = SIZE_BLOC / monster->velocity;
				else {
					monster->moveTimer = DEFAULT_GAME_FPS;
					monster->currentWay = SOUTH;
				}
			}

			if(monster->type == MONSTER_ALIEN4 && monster->replicator_timer <= 0) {
				int x = list_get_x(mList);
				int y = list_get_y(mList);
				switch(monster->currentWay) {
				case NORTH:
					y--;
					break;
				case SOUTH:
					y++;
					break;
				case EAST:
					x++;
					break;
				case WEST:
					x--;
					break;
				}
				if(x >= 0 && x < map_get_width(map) && y >= 0 && y < map_get_height(map)) {
					if(map_get_cell_type(map, x, y) == CELL_EMPTY && !player_is_here(game, x, y) && !monster_is_here(game, x, y)) {
						int type = rand_ab(0, 4);
						monster_init(map, x, y, type);
						monster->replicator_timer = monster->replicator;
					}
				}
			}


			switch (monster->currentWay) {
			case NORTH:
				if(monster_move_aux(map, x, y - 1) || (monster->y_sprite > 0 && monster->y_sprite - monster->velocity >= 0))
					monster->y_sprite-= monster->velocity;
				else if((monster->y_sprite > 0 && monster->y_sprite - monster->velocity <= 0))
					monster->y_sprite = 0;

				if(monster->x_sprite > 0 && monster->x_sprite - monster->velocity >= 0)
					monster->x_sprite-= monster->velocity;
				else if(monster->x_sprite > 0 && monster->x_sprite - monster->velocity < 0)
					monster->x_sprite = 0;

				if(monster->x_sprite < 0 && monster->x_sprite + monster->velocity <= 0)
					monster->x_sprite+= monster->velocity;
				else if(monster->x_sprite < 0 && monster->x_sprite + monster->velocity > 0)
					monster->x_sprite = 0;
				break;

			case SOUTH:
				if(monster_move_aux(map, x, y + 1) || (monster->y_sprite < 0 && monster->y_sprite + monster->velocity <= 0))
					monster->y_sprite+= monster->velocity;
				else if((monster->y_sprite < 0 && monster->y_sprite + monster->velocity >= 0))
					monster->y_sprite = 0;

				if(monster->x_sprite > 0 && monster->x_sprite - monster->velocity >= 0)
					monster->x_sprite-= monster->velocity;
				else if(monster->x_sprite > 0 && monster->x_sprite - monster->velocity < 0)
					monster->x_sprite = 0;

				if(monster->x_sprite < 0 && monster->x_sprite + monster->velocity <= 0)
					monster->x_sprite+= monster->velocity;
				else if(monster->x_sprite < 0 && monster->x_sprite + monster->velocity > 0)
					monster->x_sprite = 0;
				break;

			case WEST:
				if(monster_move_aux(map, x - 1, y) || (monster->x_sprite > 0 && monster->x_sprite - monster->velocity >= 0))
					monster->x_sprite-= monster->velocity;
				else if((monster->x_sprite > 0 && monster->x_sprite - monster->velocity <= 0))
					monster->x_sprite = 0;

				if(monster->y_sprite > 0 && monster->y_sprite - monster->velocity >= 0)
					monster->y_sprite-= monster->velocity;
				else if(monster->y_sprite > 0 && monster->y_sprite - monster->velocity < 0)
					monster->y_sprite = 0;

				if(monster->y_sprite < 0 && monster->y_sprite + monster->velocity <= 0)
					monster->y_sprite+= monster->velocity;
				else if(monster->y_sprite < 0 && monster->y_sprite + monster->velocity > 0)
					monster->y_sprite = 0;
				break;

			case EAST:
				if(monster_move_aux(map, x + 1, y) || (monster->x_sprite < 0 && monster->x_sprite + monster->velocity <= 0))
					monster->x_sprite+= monster->velocity;
				else if((monster->x_sprite < 0 && monster->x_sprite + monster->velocity >= 0))
					monster->x_sprite = 0;

				if(monster->y_sprite > 0 && monster->y_sprite - monster->velocity >= 0)
					monster->y_sprite-= monster->velocity;
				else if(monster->y_sprite > 0 && monster->y_sprite - monster->velocity < 0)
					monster->y_sprite = 0;

				if(monster->y_sprite < 0 && monster->y_sprite + monster->velocity <= 0)
					monster->y_sprite+= monster->velocity;
				else if(monster->y_sprite < 0 && monster->y_sprite + monster->velocity > 0)
					monster->y_sprite = 0;
				break;
			}
		} // end if moving

		if(monster->x_sprite > 20) {
			list_inc_x(mList);
			monster->x_sprite -= 40;
			if(map_get_cell_type(map, x, y) == CELL_MONSTER)
				map_set_cell_type(map, x, y, CELL_EMPTY);
			map_set_cell_type(map, list_get_x(mList), list_get_y(mList), CELL_MONSTER);
		}
		if(monster->x_sprite < -20) {
			list_dec_x(mList);
			monster->x_sprite += 40;
			if(map_get_cell_type(map, x, y) == CELL_MONSTER)
				map_set_cell_type(map, x, y, CELL_EMPTY);
			map_set_cell_type(map, list_get_x(mList), list_get_y(mList), CELL_MONSTER);
		}
		if(monster->y_sprite > 20) {
			list_inc_y(mList);
			monster->y_sprite -= 40;
			if(map_get_cell_type(map, x, y) == CELL_MONSTER)
				map_set_cell_type(map, x, y, CELL_EMPTY);
			map_set_cell_type(map, list_get_x(mList), list_get_y(mList), CELL_MONSTER);
		}
		if(monster->y_sprite < -20) {
			list_dec_y(mList);
			monster->y_sprite += 40;
			if(map_get_cell_type(map, x, y) == CELL_MONSTER)
				map_set_cell_type(map, x, y, CELL_EMPTY);
			map_set_cell_type(map, list_get_x(mList), list_get_y(mList), CELL_MONSTER);
		}

		if(list_get_x(mList) == player_get_x(player) && list_get_y(mList) == player_get_y(player))
			player_hit(player, 3*DEFAULT_GAME_FPS);

		//printf("dir: %d, x: %d, y: %d, x_sprite: %d, y_sprite: %d\n", monster->currentWay, list_get_x(mList), list_get_y(mList), monster->x_sprite, monster->y_sprite);
		mList = list_get_next(mList);
	} // end of while
		// We set the cell type to monster again in case something erased it
		//map_set_cell_type(map, list_get_x(mList), list_get_y(mList), CELL_MONSTER);
}

void monster_display(struct map* map)
{
	struct list* mList = map_get_monsters(map);

	while(mList != NULL) {

		struct monster* monster = list_get_data(mList);


		if(monster->invicibility > 0) {
			if(((monster->invicibility)/2)%2)
				SDL_SetAlpha(sprite_get_monster(monster->type, monster->currentWay), SDL_SRCALPHA, 128);
			else
				SDL_SetAlpha(sprite_get_monster(monster->type, monster->currentWay), SDL_SRCALPHA, 192);
		}
		else
			SDL_SetAlpha(sprite_get_monster(monster->type, monster->currentWay), SDL_SRCALPHA, 255);

		window_display_image(	sprite_get_monster(monster->type, monster->currentWay),
								list_get_x(mList) * SIZE_BLOC + monster->x_sprite,
								list_get_y(mList) * SIZE_BLOC + monster->y_sprite);
		//printf("type: %d, way: %d\n", monster->type, monster->currentWay);
		mList = list_get_next(mList);
	}
}

void monster_update(struct map* map) {
	assert(map);
	struct list* mList = map_get_monsters(map);

	while(mList != NULL) {
		struct monster* monster = list_get_data(mList);

		if(monster->invicibility > 1)
			monster->invicibility--;
		else if(monster->invicibility == 1) {
			monster->invicibility--;
			monster->moving = 1;
		}

		if(monster->moveTimer >  0)
			monster->moveTimer--;

		if(monster->replicator_timer >  0)
			monster->replicator_timer--;

		mList = list_get_next(mList);
	}
}

int monster_is_here(struct game* game, int x, int y) {
	assert(game);
	struct map* map = level_get_curr_map(game_get_curr_level(game));
	struct list* mList = map_get_monsters(map);
	while(mList != NULL) {
		if(list_get_x(mList) == x && list_get_y(mList) == y)
			return 1;
		mList = list_get_next(mList);
	}
	return 0;
}

int monster_pathfinding(struct map* map, struct player* player, struct list* mList, int* getDist)
{
	const int width = map_get_width(map);
	const int height = map_get_height(map);

	int loop = 1;

	int xSrc = list_get_x(mList);
	int ySrc = list_get_y(mList);
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
		return -1;
}
