#include "ResourceManager.h"
#include "resource2.h" // 리소스 ID 정의된 헤더 포함

std::map<BitmapID, HBITMAP> ResourceManager::bitmapMap;

void ResourceManager::LoadAll(HINSTANCE hInst) {
    //폭탄
    bitmapMap[BitmapID::BombSet] = LoadBitmap(hInst, MAKEINTRESOURCE(IDB_BITMAP_BOMB_CENTER));
    bitmapMap[BitmapID::BombCenter] = LoadBitmap(hInst, MAKEINTRESOURCE(IDB_BITMAP_BOMB_POP));
    bitmapMap[BitmapID::BombPop] = LoadBitmap(hInst, MAKEINTRESOURCE(IDB_BITMAP_BOMB_POP));
    bitmapMap[BitmapID::BombDown2] = LoadBitmap(hInst, MAKEINTRESOURCE(IDB_BITMAP_BOMB_DOWN2));
    bitmapMap[BitmapID::BombUp2] = LoadBitmap(hInst, MAKEINTRESOURCE(IDB_BITMAP_BOMB_UP2));
    bitmapMap[BitmapID::BombLeft2] = LoadBitmap(hInst, MAKEINTRESOURCE(IDB_BITMAP_BOMB_LEFT2));
    bitmapMap[BitmapID::BombRight2] = LoadBitmap(hInst, MAKEINTRESOURCE(IDB_BITMAP_BOMB_RIGHT2));
    //숲맵
    bitmapMap[BitmapID::ForestTile1] = LoadBitmap(hInst, MAKEINTRESOURCE(IDB_BITMAP_GRASS_1));
    bitmapMap[BitmapID::ForestTile2] = LoadBitmap(hInst, MAKEINTRESOURCE(IDB_BITMAP_GRASS_2));
    bitmapMap[BitmapID::ForestTile3] = LoadBitmap(hInst, MAKEINTRESOURCE(IDB_BITMAP_GRASS_3));
    bitmapMap[BitmapID::ForestTile4] = LoadBitmap(hInst, MAKEINTRESOURCE(IDB_BITMAP_GRASS_4));
    bitmapMap[BitmapID::ForestTile5] = LoadBitmap(hInst, MAKEINTRESOURCE(IDB_BITMAP_GRASS_5));
    bitmapMap[BitmapID::ForestTile6] = LoadBitmap(hInst, MAKEINTRESOURCE(IDB_BITMAP_GRASS_6));
    bitmapMap[BitmapID::ForestObstacle1] = LoadBitmap(hInst, MAKEINTRESOURCE(IDB_BITMAP_FOREST_OBSTACLE_1));
    bitmapMap[BitmapID::ForestObstacle2] = LoadBitmap(hInst, MAKEINTRESOURCE(IDB_BITMAP_FOREST_OBSTACLE_2));
    bitmapMap[BitmapID::ForestObstacle3] = LoadBitmap(hInst, MAKEINTRESOURCE(IDB_BITMAP_FOREST_OBSTACLE_3));
    bitmapMap[BitmapID::ForestObstacle4] = LoadBitmap(hInst, MAKEINTRESOURCE(IDB_BITMAP_FOREST_OBSTACLE_4));
    bitmapMap[BitmapID::ForestObstacle5] = LoadBitmap(hInst, MAKEINTRESOURCE(IDB_BITMAP_FOREST_OBSTACLE_5));
    bitmapMap[BitmapID::ForestTree] = LoadBitmap(hInst, MAKEINTRESOURCE(IDB_BITMAP_BIG_TREE));



    // 여기에 추가 비트맵도 계속 등록
}

void ResourceManager::UnloadAll() {
    for (auto& pair : bitmapMap) {
        DeleteObject(pair.second);
    }
    bitmapMap.clear();
}

HBITMAP ResourceManager::GetBitmap(BitmapID id) {
    return bitmapMap[id];
}
