#pragma once
#define _CRT_SECURE_NO_WARNINGS

#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include <time.h>

// 排行榜上存储的玩家分数最大数量
#define MAX_PLAYERS 10

// 单元格
typedef struct {
	bool is_mine;  // 有没有雷
	bool is_revealed;  // 有没有探索
	bool is_flagged;  // 有没有放小旗(扫雷游戏中的玩法: 当质疑是雷时,你可以防止旗帜标记)
	short int adjacent_mines;  // 附近的雷数量(0-8)
	short int value;  // 打印的时候显示的值
} Cell;

// 游戏板的配置
typedef struct {
	int rows;  // 理论行数
	int cols;  // 理论列数
	int real_rows;  // 实际构建的行数
	int real_cols;  // 实际构建的列数
	int mine_count;  // 雷的数量
	int base_score;  // 当前配置的基础分
} BoardConfig;

// 游戏状态
typedef enum {
	GAME_INIT,  // 待初始化,该状态象征着新的一局游戏还没开始
	GAME_RUNNING,  // 已经初始化了,运行中,等待玩家下一步操作
	GAME_ENTER,  // 游戏接收了输入,正在处理中,处理中可能进入更新界面状态
	GAME_UPDATE,  // 更新界面状态,用户对某个单元格进行操作后的反馈
	GAME_WIN,  // 游戏胜利,在游戏更新界面至游戏失败的中间,会进行成功的检查
	GAME_LOSE  // 游戏失败,该状态在更新界面时会对玩家操作后的行为检测
} GameState;

// 游戏难度
typedef enum {
	EASY, MEDIUM, HARD,  // 3个难度
	DIFFICULTY_COUNT  // 计数 3
} Diffuculty;

// 游戏配置
typedef struct {
	Cell** board;  // 游戏板 二维矩阵
	GameState state;  // 游戏状态
	Diffuculty difficult;  // 游戏难度等级
	int time;  // 某局游戏的用时
} Game;

// 玩家
typedef struct {
	char name[64];  	// 玩家昵称
	char gender[24];  	// 玩家性别
	int score;  		// 玩家当局分数
	int best_score;  // 玩家历史最高分数
	short int right_flag;  // 玩家标记的正确旗帜数量
	short int error_flag;  // 玩家标记的错误旗帜数量
} Player;

// 排行榜
typedef struct {
	Player players[MAX_PLAYERS]; // 玩家列表
	int player_count;	// 当前上榜的玩家数量
} Leaderboard;

// cell显示值 使用的是ASCII编码
short int cell_unexplored;
short int cell_empty;
short int cell_mine;
short int cell_flagged;
// 游戏板的配置列表
BoardConfig* board_configs;
// 选择的游戏配置
BoardConfig board_config;
// 游戏对象
Game* game;
// 玩家对象
Player* player;
// 排行榜
Leaderboard* leaderboard;
// 声明全局的时间变量
time_t start_time;
time_t end_time;

// 预加载
void Preload();

// 新增:初始化游戏
void InitGame();

// 创建游戏对象
Game* CreateGame(GameState state, Diffuculty level);

// 获取Cell附近的雷数量
int GetMineNearCell(int row, int col);

// 处理游戏运行时的输入
int HandleInput(char operate, int row, int col);

// 根据单元格情况更新游戏板
void UpdateGameState(int row, int col);

// 检查判断游戏结束
bool CheckGameOver();

// 将雷全部显示
void ShowAllMines();

// 开始游戏
void StartGame();

// 游戏结束
void EndGame();

// 修改难度等级
void ModifyDifficulty(int choice);

// 修改玩家昵称
void ModifyPlayerName(char name[50]);

// 修改玩家性别
void ModifyPlayerGender(int gender);

// 计算得分
int CalFinalScore();

// 添加用户到排行榜
void AddPlayerToLeaderboard(Player* player);
// 指定下标插入玩家
void MovePlayerToEnd(Player* player, int index);
// 对排行榜进行排序
void SortLeaderboard();
// 交换两个Player
void SwapPlayer(Player* a, Player* b);
// 快速排序的分区函数
int Partition(Player arr[], int low, int high);
// 快速排序函数
void QuickSort(Player arr[], int low, int high);

// 继续游戏
void ContinueGame();