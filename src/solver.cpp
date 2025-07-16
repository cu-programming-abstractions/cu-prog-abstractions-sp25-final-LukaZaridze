#include "solver.h"
#include "cell.h"

#include <vector>
#include <queue>
#include <unordered_map>
#include <unordered_set>
#include <algorithm>
#include <functional>

using namespace std;

Cell findPosition(const vector<string>& dungeon, char target) {
    for (int r = 0; r < (int)dungeon.size(); r++) {
        for (int c = 0; c < (int)dungeon[r].size(); c++) {
            if (dungeon[r][c] == target) return Cell(r, c);
        }
    }
    return Cell(-1, -1);
}

bool isPassable(const vector<string>& dungeon, int row, int col) {
    if (row < 0 || row >= (int)dungeon.size() || col < 0 || col >= (int)dungeon[0].size()) return false;
    char cell = dungeon[row][col];
    if (cell == '#') return false;
    if (cell >= 'A' && cell <= 'F' && cell != 'E') return false;
    return true;
}

bool canPassDoor(char door, int keyMask) {
    if (door == 'E') return true;
    if (door < 'A' || door > 'F') return true;
    int keyBit = door - 'A';
    return (keyMask & (1 << keyBit)) != 0;
}

int collectKey(char key, int keyMask) {
    if (key < 'a' || key > 'f') return keyMask;
    int keyBit = key - 'a';
    return keyMask | (1 << keyBit);
}

vector<Cell> reconstructPath(const unordered_map<Cell, Cell, CellHash>& parents, const Cell& start, const Cell& goal) {
    vector<Cell> path;
    Cell current = goal;
    while (!(current == start)) {
        path.push_back(current);
        current = parents.at(current);
    }
    path.push_back(start);
    reverse(path.begin(), path.end());
    return path;
}

vector<Cell> getNeighbors(const vector<string>& dungeon, const Cell& current) {
    vector<Cell> neighbors;
    for (int i = 0; i < NUM_DIRECTIONS; i++) {
        int nr = current.r + DIRECTIONS[i][0];
        int nc = current.c + DIRECTIONS[i][1];
        if (isPassable(dungeon, nr, nc)) {
            neighbors.emplace_back(nr, nc);
        }
    }
    return neighbors;
}

vector<Cell> bfsPath(const vector<string>& dungeon) {
    Cell start = findPosition(dungeon, 'S');
    Cell exit = findPosition(dungeon, 'E');
    if (start.r == -1 || exit.r == -1) return {};

    queue<Cell> q;
    unordered_set<Cell, CellHash> visited;
    unordered_map<Cell, Cell, CellHash> parents;

    q.push(start);
    visited.insert(start);

    while (!q.empty()) {
        Cell current = q.front(); q.pop();
        if (current == exit) {
            return reconstructPath(parents, start, exit);
        }
        for (const Cell& neighbor : getNeighbors(dungeon, current)) {
            if (visited.count(neighbor) == 0) {
                q.push(neighbor);
                visited.insert(neighbor);
                parents[neighbor] = current;
            }
        }
    }
    return {};
}

struct KeyState {
    int r, c;
    int keyMask;

    KeyState(int row = 0, int col = 0, int keys = 0) : r(row), c(col), keyMask(keys) {}

    bool operator==(const KeyState& other) const {
        return r == other.r && c == other.c && keyMask == other.keyMask;
    }
};

struct KeyStateHash {
    size_t operator()(const KeyState& s) const {
        size_t res = 17;
        res = res * 31 + std::hash<int>()(s.r);
        res = res * 31 + std::hash<int>()(s.c);
        res = res * 31 + std::hash<int>()(s.keyMask);
        return res;
    }
};

vector<Cell> reconstructKeyPath(const unordered_map<KeyState, KeyState, KeyStateHash>& parents,
                                const KeyState& start, const KeyState& goal) {
    vector<Cell> path;
    KeyState current = goal;
    while (!(current == start)) {
        path.emplace_back(current.r, current.c);
        current = parents.at(current);
    }
    path.emplace_back(start.r, start.c);
    reverse(path.begin(), path.end());
    return path;
}

vector<Cell> bfsPathKeys(const vector<string>& dungeon) {
    Cell start = findPosition(dungeon, 'S');
    Cell exit = findPosition(dungeon, 'E');
    if (start.r == -1 || exit.r == -1) return {};

    queue<KeyState> q;
    unordered_set<KeyState, KeyStateHash> visited;
    unordered_map<KeyState, KeyState, KeyStateHash> parents;

    KeyState startState(start.r, start.c, 0);
    q.push(startState);
    visited.insert(startState);

    while (!q.empty()) {
        KeyState current = q.front(); q.pop();

        if (current.r == exit.r && current.c == exit.c) {
            return reconstructKeyPath(parents, startState, current);
        }

        for (int i = 0; i < NUM_DIRECTIONS; i++) {
            int nr = current.r + DIRECTIONS[i][0];
            int nc = current.c + DIRECTIONS[i][1];

            if (nr < 0 || nr >= (int)dungeon.size() || nc < 0 || nc >= (int)dungeon[0].size()) continue;

            char cellChar = dungeon[nr][nc];
            if (cellChar == '#') continue;
            if (cellChar >= 'A' && cellChar <= 'F' && cellChar != 'E' && !canPassDoor(cellChar, current.keyMask)) continue;

            int newKeyMask = collectKey(cellChar, current.keyMask);
            KeyState newState(nr, nc, newKeyMask);
            if (visited.count(newState) == 0) {
                q.push(newState);
                visited.insert(newState);
                parents[newState] = current;
            }
        }
    }

    return {};
}


