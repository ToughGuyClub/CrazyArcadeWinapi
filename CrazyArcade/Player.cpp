#include "Player.h"

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
    this->dir = DIR_RIGHT;

    //폭탄 사거리 초기화
    this->bomb_distance = 3;
    this->maxBomb = 3;
    this->currrentApple = 0;
    // 스프라이트 핸들 초기화
    this->hSprite = NULL;
}


void Player::Draw_Player(HDC hdc, Player* p, COLORREF color)
{
    int mapSize = min(WINDOW_WIDTH, WINDOW_HEIGHT);
    int tile = mapSize / MAP_TILES;
    int ox = (WINDOW_WIDTH - mapSize) / 2;
    int oy = (WINDOW_HEIGHT - mapSize) / 2;
    float drawX =85+ ox + p->px * tile;
    float drawY = oy + p->py * tile+20;
    HBRUSH brush = CreateSolidBrush(color);
    HBRUSH oldB = (HBRUSH)SelectObject(hdc, brush);
    Ellipse(hdc,
        (int)drawX, (int)drawY,
        (int)(drawX + tile), (int)(drawY + tile));
    SelectObject(hdc, oldB);
    DeleteObject(brush);
    int cx = (int)(drawX + tile / 2);
    int cy = (int)(drawY + tile / 2);
    int len = tile / 4;
    HPEN arrowPen = CreatePen(PS_SOLID, 2, RGB(0, 0, 0));
    HPEN oldP = (HPEN)SelectObject(hdc, arrowPen);
    MoveToEx(hdc, cx, cy, NULL);
    switch (p->dir) {
    case DIR_UP:    LineTo(hdc, cx, cy - len); break;
    case DIR_DOWN:  LineTo(hdc, cx, cy + len); break;
    case DIR_LEFT:  LineTo(hdc, cx - len, cy); break;
    case DIR_RIGHT: LineTo(hdc, cx + len, cy); break;
    }
    SelectObject(hdc, oldP);
    DeleteObject(arrowPen);
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
    }
}
