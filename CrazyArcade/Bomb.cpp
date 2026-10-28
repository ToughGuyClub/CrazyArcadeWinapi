#include "AudioManager.h"
#include"Bomb.h"
#include "ResourceManager.h"
void Bomb::make_Bomb(Player player, Board board[MAP_TILES][MAP_TILES])
{
	
	this->pos.x = board[(int)player.GetY()][(int)player.GetX()].pos.x;
	this->pos.y = board[(int)player.GetY()][(int)player.GetX()].pos.y;
    this->logicalPos.x = (int)player.GetY();
    this->logicalPos.y = (int)player.GetX();
	this->reach = player.GetBomb_distance();
    this->reachLeft = this->reachRight = this->reachUp = this->reachDown = this->reach;
	this->state = 1;
	this->time = 20;
    this->isCollisionLeft = false;
    this->isCollisionRight = false;
    this->isCollisionUp = false;
    this->isCollisionDown = false;
    this->is_collision_player = false;
    board[(int)player.GetY()][(int)player.GetX()].obstacle = 9;
}

bool Bomb::tick_Bomb()
{
    if (state == 0) return true;
    if (--time > 0) return false;
    if (state == 1) {
        state = 2;
        time = 10;
        Audio::PlayEffect("Bubble.wav");
        return false;
    }
    state = 0;
    return true;
}

void Bomb::ResolveExplosion(Board board[MAP_TILES][MAP_TILES], Player& owner)
{
    const int row = logicalPos.x, col = logicalPos.y;
    board[row][col].obstacle = 0;
    auto trace = [&](int dx, int dy) {
        int distance = 0;
        for (int step = 1; step <= reach; ++step) {
            int x = col + dx * step, y = row + dy * step;
            if (x < 0 || x >= MAP_TILES || y < 0 || y >= MAP_TILES) break;
            int obstacle = board[y][x].obstacle;
            if (obstacle == 1) {
                board[y][x].obstacle = 0;
                distance = step;
                break;
            }
            if (obstacle == 2) {
                owner.SetCurrentApple(owner.GetCurrentApple() + 1);
                break;
            }
            if (obstacle != 0 && obstacle != 9) break;
            distance = step;
        }
        return distance;
    };
    reachLeft = trace(-1, 0); reachRight = trace(1, 0);
    reachUp = trace(0, -1); reachDown = trace(0, 1);
}

int Bomb::collision_bomb_player(Bomb bomb, Player player)
{
    int row = bomb.GetLogicalX(), col = bomb.GetLogicalY();
    int x = player.GetX(), y = player.GetY();
    return (y == row && x >= col - bomb.GetReachLeft() && x <= col + bomb.GetReachRight())
        || (x == col && y >= row - bomb.GetReachUp() && y <= row + bomb.GetReachDown());
}

// Rendering only: explosion/map changes are resolved by the simulation.
void draw_Bomb(HDC hDC, HINSTANCE, Bomb bombs[], int n, Board[MAP_TILES][MAP_TILES], Player*)
{
    if (n == 0) return;
    HDC memDC = CreateCompatibleDC(hDC);
    auto draw = [&](BitmapID id, int x, int y, int frame, int stride, int width) {
        HGDIOBJ old = SelectObject(memDC, ResourceManager::GetBitmap(id));
        TransparentBlt(hDC, x - MAP_TILES, y - MAP_TILES, MAP_TILES * 2, MAP_TILES * 2,
            memDC, frame * stride, 0, width, width, RGB(255, 0, 255));
        SelectObject(memDC, old);
    };
    for (int i = 0; i < n; ++i) {
        const Bomb& bomb = bombs[i];
        int x = bomb.GetX(), y = bomb.GetY();
        if (bomb.GetState() == 1) {
            draw(BitmapID::BombSet, x, y, bomb.GetTime() % 4, 56, 54);
        } else if (bomb.GetState() == 2) {
            int frame = bomb.GetTime() % 3;
            draw(BitmapID::BombCenter, x, y, frame, 52, 52);
            for (int j = 1; j <= bomb.GetReachLeft(); ++j) draw(BitmapID::BombLeft2, x - 40*j, y, frame, 52, 52);
            for (int j = 1; j <= bomb.GetReachRight(); ++j) draw(BitmapID::BombRight2, x + 40*j, y, frame, 52, 52);
            for (int j = 1; j <= bomb.GetReachUp(); ++j) draw(BitmapID::BombUp2, x, y - 40*j, frame, 52, 52);
            for (int j = 1; j <= bomb.GetReachDown(); ++j) draw(BitmapID::BombDown2, x, y + 40*j, frame, 52, 52);
        }
    }
    DeleteDC(memDC);
}
