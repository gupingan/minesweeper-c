#pragma once
#define GAME_FILE "minesweeper.dat"
#define BOARD_FILE "leapboard.dat"

bool LoadGame();
bool SaveGame();
bool SaveLeaderboard();
bool LoadLeaderboard();