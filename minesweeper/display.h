#pragma once
void DisplayGameState();  // 这个函数用于显示当前棋盘信息
void DisplayErrorMsg(const char* message);  // 当玩家输入坐标不合法时提示有误
void DisplayPlayer();  // 打印 玩家性别  不换行，比如：图图女士、兔兔男士等等
void DisplayPlayerInfo();  // 打印 用户的分数性别和当前是哪个用户
void DisplayGameOver();
void DisplayLeaderboard();