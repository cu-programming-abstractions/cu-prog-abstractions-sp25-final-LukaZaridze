#pragma once
#include <vector>
#include <string>

std::vector<std::string> generateDungeon(int rows, int cols, int roomRate = 20);
bool isValidCell(int row, int col, int rows, int cols);
void carvePassage(std::vector<std::string>& dungeon, int fromRow, int fromCol, int toRow, int toCol);
