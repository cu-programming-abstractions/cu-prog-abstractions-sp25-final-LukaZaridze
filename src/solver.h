#pragma once
#include <vector>
#include <string>
#include "cell.h"

std::vector<Cell> bfsPath(const std::vector<std::string>& dungeon);
std::vector<Cell> bfsPathKeys(const std::vector<std::string>& dungeon);
Cell findPosition(const std::vector<std::string>& dungeon, char target);
bool isPassable(const std::vector<std::string>& dungeon, int row, int col);
bool canPassDoor(char door, int keyMask);
int collectKey(char key, int keyMask);

//#define IMPLEMENT_OPTIONAL_FUNCTIONS

#ifdef IMPLEMENT_OPTIONAL_FUNCTIONS
int countReachableKeys(const std::vector<std::string>& dungeon);
#endif
