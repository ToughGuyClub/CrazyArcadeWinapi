#include "Board.h"
#include"ResourceManager.h"
#include"Player.h"
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
                if (rand() % 10 > 5) {
                    board[i][j].item = rand() % 3+1;
                }
				
            }
           
           board[i][j].tile=rand()%6+1;
        }
    }
}


void DrawDarkOverlay(HDC hDC, int width, int height) {
    // 1. 반투명 블렌딩 설정
    BLENDFUNCTION blend = {};
    blend.BlendOp = AC_SRC_OVER;
    blend.SourceConstantAlpha = 128; // 투명도 조절 (0=완전투명, 255=완전불투명)

    // 2. 호환 DC 및 비트맵 생성
    HDC hMemDC = CreateCompatibleDC(hDC);
    HBITMAP hBitmap = CreateCompatibleBitmap(hDC, width, height);
    HBITMAP hOldBitmap = (HBITMAP)SelectObject(hMemDC, hBitmap);

    // 3. 검은색으로 채우기
    HBRUSH hBrush = CreateSolidBrush(RGB(0, 0, 0));
    RECT rect = { 0, 0, width, height };
    FillRect(hMemDC, &rect, hBrush);

    // 4. 원본 DC에 반투명하게 출력
    AlphaBlend(hDC, 0, 0, width, height, hMemDC, 0, 0, width, height, blend);

    // 5. 리소스 해제
    SelectObject(hMemDC, hOldBitmap);
    DeleteObject(hBrush);
    DeleteObject(hBitmap);
    DeleteDC(hMemDC);
}