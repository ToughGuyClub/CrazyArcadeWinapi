#pragma once
#include <windows.h>
#include <map>

enum class BitmapID {
    //ÆøÅº°ü·Ã
    BombSet,
    BombCenter,
    BombPop,
    BombDown2,
    BombUp2,
    BombLeft2,
    BombRight2,
    //½£¸Ê°ü·Ã
    ForestTile1,
    ForestTile2,
    ForestTile3,
    ForestTile4,
    ForestTile5,
    ForestTile6,
    ForestObstacle1,
    ForestObstacle2,
    ForestObstacle3,
    ForestObstacle4,
    ForestObstacle5,
    ForestTree,
    // ÇÊ¿äÇÑ ¸®¼Ò½º °è¼Ó Ãß°¡
};

class ResourceManager {
public:
    static void LoadAll(HINSTANCE hInst);
    static void UnloadAll();
    static HBITMAP GetBitmap(BitmapID id);

private:
    static std::map<BitmapID, HBITMAP> bitmapMap;
};