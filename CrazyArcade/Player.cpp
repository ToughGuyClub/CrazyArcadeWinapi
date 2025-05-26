#include "Player.h"
#include "ResourceManager.h"
Player::Player()
{
}

Player::~Player()
{
}

void Player::Init_Player(POINT pos)
{
    int mapSize = min(WINDOW_WIDTH, WINDOW_HEIGHT);
    int tempx = 85;
    int tempy= mapSize / MAP_TILES / 2;
    this->x = pos.x;
    this->y = pos.y;

    // 초기 목적지 좌표는 현재 좌표와 동일하게
    this->destX = pos.x;
    this->destY = pos.y;

    // 실수형 좌표(px, py)도 현재 좌표로 초기화
    this->px = static_cast<float>(pos.x);
    this->py = static_cast<float>(pos.y);

    // 방향 초기화
    this->dir = DIR_DOWN;

    //폭탄 사거리 초기화
    this->bomb_distance = 3;
    this->maxBomb = 3;
    this->currrentApple = 0;

    //선택 캐릭터 초기화
    this->selected_character = 1;
   
	this->move_cnt = 0;
    this->player_num = 1;
    // 스프라이트 핸들 초기화
    this->hSprite = NULL;
}


void Player::Draw_Player(HDC hdc, Player* p, int character)
{
    int mapSize = min(WINDOW_WIDTH, WINDOW_HEIGHT);
    int tile = mapSize / MAP_TILES;
    int ox = (WINDOW_WIDTH - mapSize) / 2;
    int oy = (WINDOW_HEIGHT - mapSize) / 2;
    float drawX = 85 + ox + p->px * tile;
    float drawY = oy + p->py * tile + 20;
    
    HBITMAP hbitmapPlayer = ResourceManager::GetBitmap(BitmapID::Player1Moving);
  if(p->GetPlayerNum() == 2)
      hbitmapPlayer = ResourceManager::GetBitmap(BitmapID::Player2Moving);
    HDC memDC = CreateCompatibleDC(hdc);
    HBITMAP oldBitmap = (HBITMAP)SelectObject(memDC, hbitmapPlayer);
    // 현재 방향에 따라 스프라이트의 위치를 계산
    int spriteX = 0;
    int spriteY = 0;
    if (p->dir == DIR_UP) {
        spriteY = 60 * 1; // 위쪽 스프라이트
    }
    else if (p->dir == DIR_DOWN) {
        spriteY = 60 * 2; // 아래쪽 스프라이트
    }
    else if (p->dir == DIR_LEFT) {
        spriteY = 60 * 3; // 왼쪽 스프라이트
    }
    else if (p->dir == DIR_RIGHT) {
        spriteY = 60 * 4; // 오른쪽 스프라이트
    }

    spriteX = p->move_cnt / 4 * 50; // 스프라이트의 X 위치 (애니메이션 프레임)

    TransparentBlt(
        hdc, drawX, drawY - 2 * MAP_TILES + 10, MAP_TILES * 2, MAP_TILES * 3,
        memDC, spriteX, spriteY, 50, 60,
        RGB(255, 0, 255)
    );
    HBITMAP hbitmapPlayermask = ResourceManager::GetBitmap(BitmapID::PlayerMovingMask);
    HBITMAP oldBitmapmask = (HBITMAP)SelectObject(memDC, hbitmapPlayermask);
    TransparentBlt(
        hdc, drawX, drawY - 2 * MAP_TILES + 10, MAP_TILES * 2, MAP_TILES * 3,
        memDC, spriteX, spriteY, 50, 60,
        RGB(255, 0, 255)
    );
    SelectObject(memDC, oldBitmap);
    SelectObject(memDC, oldBitmapmask);
    DeleteDC(memDC);
}

void HandlePlayerMovement(Player& player, const bool* keyStates, int upKey, int downKey, int leftKey, int rightKey, Board board[MAP_TILES][MAP_TILES]) {
    if (player.IsStopped()) {
        int x = player.GetX();
        int y = player.GetY();

        if (keyStates[upKey]) {
            if (y > 0 && board[y - 1][x].obstacle == 0) {
                player.SetDir(DIR_UP);
                player.SetDestY(y - 1);
            }
        }
        else if (keyStates[downKey]) {
            if (y < MAP_TILES - 1 && board[y + 1][x].obstacle == 0) {
                player.SetDir(DIR_DOWN);
                player.SetDestY(y + 1);
            }
        }
        else if (keyStates[leftKey]) {
            if (x > 0 && board[y][x - 1].obstacle == 0) {
                player.SetDir(DIR_LEFT);
                player.SetDestX(x - 1);
            }
        }
        else if (keyStates[rightKey]) {
            if (x < MAP_TILES - 1 && board[y][x + 1].obstacle == 0) {
                player.SetDir(DIR_RIGHT);
                player.SetDestX(x + 1);
            }
        }
    }
}

void UpdatePlayerPosition(Player& player, float speed) {
    if (player.IsMoving()) {
        float dx = player.GetDestX() - player.GetPx();
        float dy = player.GetDestY() - player.GetPy();

        if (fabs(dx) > speed) player.SetPx(player.GetPx() + (dx > 0 ? speed : -speed));
        else player.SetPx(player.GetDestX());

        if (fabs(dy) > speed) player.SetPy(player.GetPy() + (dy > 0 ? speed : -speed));
        else player.SetPy(player.GetDestY());

        if (!player.IsMoving()) player.SnapToDest();
        
        player.SetMoveCount((player.GetMoveCount() + 1) % 16);
        
    }
}
void DrawGrid(HDC hdc, HINSTANCE g_hInst, Board board[MAP_TILES][MAP_TILES], Player* p1, Player* p2) {
    int mapSize = min(WINDOW_WIDTH, WINDOW_HEIGHT);
    int tile = mapSize / MAP_TILES / 2;
    int ox = (WINDOW_WIDTH - mapSize) / 2;
    int oy = (WINDOW_HEIGHT - mapSize) / 2;

    HPEN pen = CreatePen(PS_SOLID, 1, RGB(0, 0, 0));
    HPEN oldPen = (HPEN)SelectObject(hdc, pen);

    HDC tileDC = CreateCompatibleDC(hdc);
    HBITMAP tilebit[6];
    tilebit[0] = ResourceManager::GetBitmap(BitmapID::ForestTile1);
    tilebit[1] = ResourceManager::GetBitmap(BitmapID::ForestTile2);
    tilebit[2] = ResourceManager::GetBitmap(BitmapID::ForestTile3);
    tilebit[3] = ResourceManager::GetBitmap(BitmapID::ForestTile4);
    tilebit[4] = ResourceManager::GetBitmap(BitmapID::ForestTile5);
    tilebit[5] = ResourceManager::GetBitmap(BitmapID::ForestTile6);
    HBITMAP  oldTile = (HBITMAP)SelectObject(tileDC, tilebit[0]);
    HBITMAP obstacleBitmap[5];
    obstacleBitmap[0] = ResourceManager::GetBitmap(BitmapID::ForestObstacle1);
    obstacleBitmap[1] = ResourceManager::GetBitmap(BitmapID::ForestObstacle2);
    obstacleBitmap[2] = ResourceManager::GetBitmap(BitmapID::ForestObstacle3);
    obstacleBitmap[3] = ResourceManager::GetBitmap(BitmapID::ForestObstacle4);
    obstacleBitmap[4] = ResourceManager::GetBitmap(BitmapID::ForestObstacle5);
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
			if (j == p1->GetX()) {
				p1->Draw_Player(hdc, p1, p1->GetSelectedCharacter());
			}
			if (j == p2->GetX()) {
				p2->Draw_Player(hdc, p2, p2->GetSelectedCharacter());
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
                TransparentBlt(hdc, board[i][j].pos.x - MAP_TILES - 5, board[i][j].pos.y - MAP_TILES - 5, 2 * MAP_TILES + 10, 2 * MAP_TILES + 10, obstacleDC, 0, 0, 68, 67, RGB(255, 0, 255));
                SelectObject(obstacleDC, oldObstacleBitmap);
            }



        }

    }
    HBITMAP treebit = LoadBitmap(g_hInst, MAKEINTRESOURCE(IDB_BITMAP_BIG_TREE));
    HDC treeDC = CreateCompatibleDC(hdc);
    HBITMAP oldTreeBitmap = (HBITMAP)SelectObject(treeDC, treebit);
    TransparentBlt(hdc, board[1][8].pos.x - 10, board[1][9].pos.y - 3 * MAP_TILES, 7 * MAP_TILES, 6 * MAP_TILES, treeDC, 0, 0, 100, 80, RGB(255, 0, 255));
    SelectObject(treeDC, oldTreeBitmap);

    DeleteDC(treeDC);

    DeleteDC(obstacleDC);
    SelectObject(hdc, oldPen);
    DeleteObject(pen);
    SelectObject(tileDC, oldTile);
    DeleteDC(tileDC);

}

void RGBtoHSV(BYTE r, BYTE g, BYTE b, double& h, double& s, double& v) {
    double R = r / 255.0, G = g / 255.0, B = b / 255.0;
    double maxc = max(R, max(G, B));
    double minc = min(R, min(G, B));
    v = maxc;

    double delta = maxc - minc;
    if (delta == 0) {
        h = 0;
        s = 0;
        return;
    }

    s = delta / maxc;

    if (maxc == R)
        h = fmod((G - B) / delta, 6.0);
    else if (maxc == G)
        h = ((B - R) / delta) + 2;
    else
        h = ((R - G) / delta) + 4;

    h *= 60;
    if (h < 0) h += 360;
}
void HSVtoRGB(double h, double s, double v, BYTE& r, BYTE& g, BYTE& b) {
    double C = v * s;
    double X = C * (1 - fabs(fmod(h / 60.0, 2) - 1));
    double m = v - C;

    double r1, g1, b1;

    if (h < 60) { r1 = C; g1 = X; b1 = 0; }
    else if (h < 120) { r1 = X; g1 = C; b1 = 0; }
    else if (h < 180) { r1 = 0; g1 = C; b1 = X; }
    else if (h < 240) { r1 = 0; g1 = X; b1 = C; }
    else if (h < 300) { r1 = X; g1 = 0; b1 = C; }
    else { r1 = C; g1 = 0; b1 = X; }

    r = BYTE((r1 + m) * 255);
    g = BYTE((g1 + m) * 255);
    b = BYTE((b1 + m) * 255);
}
bool IsInColorRange(BYTE r, BYTE g, BYTE b, int hueMin, int hueMax) {
    double h, s, v;
    RGBtoHSV(r, g, b, h, s, v);
    return (h >= hueMin && h <= hueMax);
}

HBITMAP AdjustBitmapHueWithFilter(HBITMAP hBitmap, double hueShift, int hueFilterMin, int hueFilterMax) {
    BITMAP bmp;
    GetObject(hBitmap, sizeof(BITMAP), &bmp);
    int colorHueTable[] = {
    0,   // 빨강
    30,  // 주황
    60,  // 노랑
    120, // 초록
    150, // 민트
    210, // 파랑
    270, // 보라
    
    };
    hueShift = colorHueTable[(int)hueShift];
   
    BITMAPINFO bmi = { 0 };
    bmi.bmiHeader.biSize = sizeof(BITMAPINFOHEADER);
    bmi.bmiHeader.biWidth = bmp.bmWidth;
    bmi.bmiHeader.biHeight = -bmp.bmHeight;
    bmi.bmiHeader.biPlanes = 1;
    bmi.bmiHeader.biBitCount = 24;
    bmi.bmiHeader.biCompression = BI_RGB;

    void* pixels = nullptr;
    HDC hdc = GetDC(NULL);
    HBITMAP newBitmap = CreateDIBSection(hdc, &bmi, DIB_RGB_COLORS, &pixels, NULL, 0);
    ReleaseDC(NULL, hdc);

    if (!newBitmap || !pixels) return NULL;

    HDC srcDC = CreateCompatibleDC(NULL);
    HDC dstDC = CreateCompatibleDC(NULL);
    SelectObject(srcDC, hBitmap);
    SelectObject(dstDC, newBitmap);
    BitBlt(dstDC, 0, 0, bmp.bmWidth, bmp.bmHeight, srcDC, 0, 0, SRCCOPY);

    BYTE* bytePixels = (BYTE*)pixels;
    for (int y = 0; y < bmp.bmHeight; ++y) {
        for (int x = 0; x < bmp.bmWidth; ++x) {
            int idx = (y * bmp.bmWidth + x) * 3;
            BYTE& B = bytePixels[idx];
            BYTE& G = bytePixels[idx + 1];
            BYTE& R = bytePixels[idx + 2];

            // 마젠타 배경은 스킵
            if (R == 255 && G == 0 && B == 255) continue;

            if (IsInColorRange(R, G, B, hueFilterMin, hueFilterMax)) {
                double h, s, v;
                RGBtoHSV(R, G, B, h, s, v);
                h = fmod(h + hueShift, 360.0);
                if (h < 0) h += 360.0;
                HSVtoRGB(h, s, v, R, G, B);
            }
        }
    }

    DeleteDC(srcDC);
    DeleteDC(dstDC);
    return newBitmap;
}
