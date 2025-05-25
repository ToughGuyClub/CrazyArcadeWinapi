#pragma once

#include "pch.h"



#include"Board.h"
static bool keyStates[256] = { 0 };
class Player {
    int x, y;
    int destX, destY;
    int dir;
    float px, py;
    int bomb_distance;
    int maxBomb;
    int currrentApple;
    HBITMAP hSprite;
public:
    Player();
    ~Player();
    Player get_player(Player player);
   
    void Init_Player(POINT pos);

    void Draw_Player(HDC hdc, Player* p, COLORREF color);

public:
    // Getter
    int GetX() const { return x; }
    int GetY() const { return y; }
    int GetDestX() const { return destX; }
    int GetDestY() const { return destY; }
    int GetDir() const { return dir; }
    float GetPx() const { return px; }
    float GetPy() const { return py; }
    int GetBomb_distance()const { return bomb_distance; }
    int GetMaxBomb()const { return maxBomb; }
	int GetCurrentApple() const { return currrentApple; }
    // Setter
    void SetX(int value) { x = value; }
    void SetY(int value) { y = value; }
    void SetDestX(int value) { destX = value; }
    void SetDestY(int value) { destY = value; }
    void SetDir(int value) { dir = value; }
    void SetPx(float value) { px = value; }
    void SetPy(float value) { py = value; }
    void SetBomb_distance(int value) { bomb_distance = value; }
    void SetMaxBomb(int value) { maxBomb = value; }
	void SetCurrentApple(int value) { currrentApple = value; }
    // 위치 비교
    bool IsStopped() const { return (px == x && py == y); }
    bool IsMoving() const { return (px != destX || py != destY); }

 
    void SnapToDest() {
        x = destX;
        y = destY;
        px = destX;
        py = destY;
    }
};

   //이동 관리 함수
void HandlePlayerMovement(Player& player, const bool* keyStates, int upKey, int downKey, int leftKey, int rightKey, Board board[MAP_TILES][MAP_TILES]);
void UpdatePlayerPosition(Player& player, float speed);

