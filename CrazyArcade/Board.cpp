#include "Board.h"

void init_board(Board board[MAP_TILES][MAP_TILES]) {
    int mapSize = min(WINDOW_WIDTH, WINDOW_HEIGHT);
    int tile = mapSize / MAP_TILES/2;
    int ox = (WINDOW_WIDTH - mapSize) / 2;
    for (int i = 0; i < MAP_TILES; ++i) {
        for (int j = 0;j < MAP_TILES;++j) {
            board[i][j].pos.x = 85+ox+tile + j * tile * 2;
            board[i][j].pos.y = 2*tile + i * tile * 2;
        }
    }
    // 하드코딩된 맵 데이터 (0: 흰색, 1: 초록색, 2: 노란색, 3: 갈색)
    int obstacle_map[MAP_TILES][MAP_TILES] = {
     {0,0,1,1,0,1,0,0,0,0,0,0,0,0,1,0,1,1,0,0},
     {0,0,0,1,0,0,0,0,0,2,2,0,0,0,0,0,1,0,0,0},
     {1,0,0,0,0,0,1,0,0,2,2,0,0,1,0,0,0,0,0,1},
     {0,0,0,1,1,1,1,0,0,0,0,0,0,1,1,1,1,0,0,0},
     {0,0,1,1,1,1,1,1,1,0,0,1,1,1,1,1,1,1,0,0},
     {0,0,0,0,1,1,1,1,1,1,1,1,1,1,1,1,0,0,0,0},
     {0,0,0,0,0,0,0,0,1,1,1,1,0,0,0,0,0,0,0,0},
     {0,1,0,0,1,0,0,0,0,1,1,0,0,0,0,1,0,0,1,0},
     {1,1,1,0,1,1,1,0,0,1,1,0,0,1,1,1,0,1,1,1},
     {1,1,1,1,1,1,0,0,1,1,1,1,0,0,1,1,1,1,1,1},
     {1,1,1,1,1,1,0,0,1,1,1,1,0,0,1,1,1,1,1,1},
     {1,0,0,0,0,1,1,0,0,1,1,0,0,1,1,0,0,0,0,1},
     {0,0,0,0,0,0,0,0,0,1,1,0,0,0,0,0,0,0,0,0},
     {0,0,1,0,0,1,0,0,0,1,1,0,0,0,1,0,0,1,0,0},
     {0,1,1,1,0,1,1,0,0,0,0,0,0,1,1,0,1,1,1,0},
     {0,0,0,1,1,1,1,0,1,1,1,1,0,1,1,1,1,0,0,0},
     {0,0,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,0,0},
     {0,0,0,0,1,0,0,1,1,1,1,1,1,0,0,1,0,0,0,0},
     {0,0,0,0,0,0,0,0,0,1,1,0,0,0,0,0,0,0,0,0},
     {1,1,0,0,0,0,0,0,3,1,1,3,0,0,0,0,0,0,1,1}
    };
    
    for (int i = 0; i < MAP_TILES; ++i) {
        for (int j = 0; j < MAP_TILES; ++j) {
         
            board[i][j].obstacle = obstacle_map[i][j];
            if (board[i][j].obstacle == 1) {
				board[i][j].obstacle_tpye = rand() % 5 + 1; 
            }
           board[i][j].tile=rand()%6+1;
        }
    }
}
void DrawGrid(HDC hdc, HINSTANCE g_hInst, Board board[MAP_TILES][MAP_TILES]) {
    int mapSize = min(WINDOW_WIDTH, WINDOW_HEIGHT);
    int tile = mapSize / MAP_TILES / 2;
    int ox = (WINDOW_WIDTH - mapSize) / 2;
    int oy = (WINDOW_HEIGHT - mapSize) / 2;

    HPEN pen = CreatePen(PS_SOLID, 1, RGB(0, 0, 0));
    HPEN oldPen = (HPEN)SelectObject(hdc, pen);
  
    HDC tileDC= CreateCompatibleDC(hdc);
    HBITMAP tilebit[6];
    tilebit[0] = LoadBitmap(g_hInst, MAKEINTRESOURCE(IDB_BITMAP_GRASS_1));
    tilebit[1] = LoadBitmap(g_hInst, MAKEINTRESOURCE(IDB_BITMAP_GRASS_2));
    tilebit[2] = LoadBitmap(g_hInst, MAKEINTRESOURCE(IDB_BITMAP_GRASS_3));
    tilebit[3] = LoadBitmap(g_hInst, MAKEINTRESOURCE(IDB_BITMAP_GRASS_4));
    tilebit[4] = LoadBitmap(g_hInst, MAKEINTRESOURCE(IDB_BITMAP_GRASS_5));
    tilebit[5] = LoadBitmap(g_hInst, MAKEINTRESOURCE(IDB_BITMAP_GRASS_6));
    HBITMAP  oldTile = (HBITMAP)SelectObject(tileDC, tilebit[0]);
    HBITMAP obstacleBitmap[5];
    obstacleBitmap[0] = LoadBitmap(g_hInst, MAKEINTRESOURCE(IDB_BITMAP_FOREST_OBSTACLE_1));
    obstacleBitmap[1] = LoadBitmap(g_hInst, MAKEINTRESOURCE(IDB_BITMAP_FOREST_OBSTACLE_2));
    obstacleBitmap[2] = LoadBitmap(g_hInst, MAKEINTRESOURCE(IDB_BITMAP_FOREST_OBSTACLE_3));
    obstacleBitmap[3] = LoadBitmap(g_hInst, MAKEINTRESOURCE(IDB_BITMAP_FOREST_OBSTACLE_4));
    obstacleBitmap[4] = LoadBitmap(g_hInst, MAKEINTRESOURCE(IDB_BITMAP_FOREST_OBSTACLE_5));
    HDC obstacleDC = CreateCompatibleDC(hdc);
    HBITMAP oldObstacleBitmap = (HBITMAP)SelectObject(obstacleDC, obstacleBitmap[0]);

    for (int i = 0; i < MAP_TILES; ++i) {
        for (int j = 0; j < MAP_TILES; ++j) {
            COLORREF color;
            if (board[i][j].tile == 1) {
                oldTile = (HBITMAP)SelectObject(tileDC, tilebit[0]);
                StretchBlt(hdc, board[i][j].pos.x - MAP_TILES, board[i][j].pos.y - MAP_TILES, 2 * MAP_TILES, 2 * MAP_TILES, tileDC, 0, 0, 40, 40, SRCCOPY);
            }
            else if (board[i][j].tile == 2) {
                oldTile = (HBITMAP)SelectObject(tileDC, tilebit[1]);
                StretchBlt(hdc, board[i][j].pos.x - MAP_TILES, board[i][j].pos.y - MAP_TILES, 2 * MAP_TILES, 2 * MAP_TILES, tileDC, 0, 0, 40, 40, SRCCOPY);
            }
            else if (board[i][j].tile == 3) {
                oldTile = (HBITMAP)SelectObject(tileDC, tilebit[2]);
                StretchBlt(hdc, board[i][j].pos.x - MAP_TILES, board[i][j].pos.y - MAP_TILES, 2 * MAP_TILES, 2 * MAP_TILES, tileDC, 0, 0, 40, 40, SRCCOPY);
            }
            else if (board[i][j].tile == 4) {
                oldTile = (HBITMAP)SelectObject(tileDC, tilebit[3]);
                StretchBlt(hdc, board[i][j].pos.x - MAP_TILES, board[i][j].pos.y - MAP_TILES, 2 * MAP_TILES, 2 * MAP_TILES, tileDC, 0, 0, 40, 40, SRCCOPY);
            }
            else if (board[i][j].tile == 5) {
                oldTile = (HBITMAP)SelectObject(tileDC, tilebit[4]);
                StretchBlt(hdc, board[i][j].pos.x - MAP_TILES, board[i][j].pos.y - MAP_TILES, 2 * MAP_TILES, 2 * MAP_TILES, tileDC, 0, 0, 40, 40, SRCCOPY);
            }
            else if (board[i][j].tile == 6) {
                oldTile = (HBITMAP)SelectObject(tileDC, tilebit[5]);
                StretchBlt(hdc, board[i][j].pos.x - MAP_TILES, board[i][j].pos.y - MAP_TILES, 2 * MAP_TILES, 2 * MAP_TILES, tileDC, 0, 0, 40, 40, SRCCOPY);
            }
            
            switch (board[i][j].obstacle) {
            case 3:  // 갈색 (바구니)
                color = RGB(139, 69, 19); // 나무색
                break;
            default:
                color = RGB(255, 255, 255);
                break; // 0번 장애물은 그리지 않음
            }

           
          
            if (board[i][j].obstacle == 1) {
				
				(HBITMAP)SelectObject(obstacleDC, obstacleBitmap[board[i][j].obstacle_tpye - 1]);
				TransparentBlt(hdc, board[i][j].pos.x - MAP_TILES-5, board[i][j].pos.y - MAP_TILES-5, 2 * MAP_TILES+10, 2 * MAP_TILES+10, obstacleDC, 0, 0, 68, 67, RGB(255, 0, 255));
                SelectObject(obstacleDC, oldObstacleBitmap);
            }
			
           
          
        }
       
    }
    HBITMAP treebit = LoadBitmap(g_hInst, MAKEINTRESOURCE(IDB_BITMAP_BIG_TREE));
    HDC treeDC = CreateCompatibleDC(hdc);
    HBITMAP oldTreeBitmap = (HBITMAP)SelectObject(treeDC, treebit);
    TransparentBlt(hdc, board[1][8].pos.x-10, board[1][9].pos.y - 3*MAP_TILES, 7*MAP_TILES, 6 * MAP_TILES, treeDC, 0, 0, 100, 80, RGB(255, 0, 255));
    SelectObject(treeDC, oldTreeBitmap);
    DeleteObject(treebit);
    DeleteDC(treeDC);
    for (int i = 0;i < 6;++i) {
		DeleteObject(tilebit[i]);
    }
   
    for (int k = 0;k < 5;++k) {
        DeleteObject(obstacleBitmap[k]);
    }
    DeleteDC(obstacleDC);
    SelectObject(hdc, oldPen);
    DeleteObject(pen);
    SelectObject(tileDC, oldTile);
    DeleteDC(tileDC);

}
