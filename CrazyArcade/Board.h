#pragma once

#include"pch.h"



typedef struct Board {
	POINT pos;
	int obstacle;
	int obstacle_tpye;
	int tile;
	int item;	// 0없음 1 물풍선사거리 2물풍선 개수 3 이동속도

}Board;
#define MAP_TILES      20
void init_board(Board board[MAP_TILES][MAP_TILES]);
void DrawDarkOverlay(HDC hDC, int width, int height);