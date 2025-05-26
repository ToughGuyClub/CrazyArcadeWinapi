#pragma once

#include"pch.h"



typedef struct Board {
	POINT pos;
	int obstacle;
	int obstacle_tpye;
	int tile;

}Board;
#define MAP_TILES      20
void init_board(Board board[MAP_TILES][MAP_TILES]);