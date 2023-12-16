#pragma once
#define _CRT_SECURE_NO_WARNINGS

#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include <time.h>

// ���а��ϴ洢����ҷ����������
#define MAX_PLAYERS 10

// ��Ԫ��
typedef struct {
	bool is_mine;  // ��û����
	bool is_revealed;  // ��û��̽��
	bool is_flagged;  // ��û�з�С��(ɨ����Ϸ�е��淨: ����������ʱ,����Է�ֹ���ı��)
	short int adjacent_mines;  // ������������(0-8)
	short int value;  // ��ӡ��ʱ����ʾ��ֵ
} Cell;

// ��Ϸ�������
typedef struct {
	int rows;  // ��������
	int cols;  // ��������
	int real_rows;  // ʵ�ʹ���������
	int real_cols;  // ʵ�ʹ���������
	int mine_count;  // �׵�����
	int base_score;  // ��ǰ���õĻ�����
} BoardConfig;

// ��Ϸ״̬
typedef enum {
	GAME_INIT,  // ����ʼ��,��״̬�������µ�һ����Ϸ��û��ʼ
	GAME_RUNNING,  // �Ѿ���ʼ����,������,�ȴ������һ������
	GAME_ENTER,  // ��Ϸ����������,���ڴ�����,�����п��ܽ�����½���״̬
	GAME_UPDATE,  // ���½���״̬,�û���ĳ����Ԫ����в�����ķ���
	GAME_WIN,  // ��Ϸʤ��,����Ϸ���½�������Ϸʧ�ܵ��м�,����гɹ��ļ��
	GAME_LOSE  // ��Ϸʧ��,��״̬�ڸ��½���ʱ�����Ҳ��������Ϊ���
} GameState;

// ��Ϸ�Ѷ�
typedef enum {
	EASY, MEDIUM, HARD,  // 3���Ѷ�
	DIFFICULTY_COUNT  // ���� 3
} Diffuculty;

// ��Ϸ����
typedef struct {
	Cell** board;  // ��Ϸ�� ��ά����
	GameState state;  // ��Ϸ״̬
	Diffuculty difficult;  // ��Ϸ�Ѷȵȼ�
	int time;  // ĳ����Ϸ����ʱ
} Game;

// ���
typedef struct {
	char name[64];  	// ����ǳ�
	char gender[24];  	// ����Ա�
	int score;  		// ��ҵ��ַ���
	int best_score;  // �����ʷ��߷���
	short int right_flag;  // ��ұ�ǵ���ȷ��������
	short int error_flag;  // ��ұ�ǵĴ�����������
} Player;

// ���а�
typedef struct {
	Player players[MAX_PLAYERS]; // ����б�
	int player_count;	// ��ǰ�ϰ���������
} Leaderboard;

// cell��ʾֵ ʹ�õ���ASCII����
short int cell_unexplored;
short int cell_empty;
short int cell_mine;
short int cell_flagged;
// ��Ϸ��������б�
BoardConfig* board_configs;
// ѡ�����Ϸ����
BoardConfig board_config;
// ��Ϸ����
Game* game;
// ��Ҷ���
Player* player;
// ���а�
Leaderboard* leaderboard;
// ����ȫ�ֵ�ʱ�����
time_t start_time;
time_t end_time;

// Ԥ����
void Preload();

// ����:��ʼ����Ϸ
void InitGame();

// ������Ϸ����
Game* CreateGame(GameState state, Diffuculty level);

// ��ȡCell������������
int GetMineNearCell(int row, int col);

// ������Ϸ����ʱ������
int HandleInput(char operate, int row, int col);

// ���ݵ�Ԫ�����������Ϸ��
void UpdateGameState(int row, int col);

// ����ж���Ϸ����
bool CheckGameOver();

// ����ȫ����ʾ
void ShowAllMines();

// ��ʼ��Ϸ
void StartGame();

// ��Ϸ����
void EndGame();

// �޸��Ѷȵȼ�
void ModifyDifficulty(int choice);

// �޸�����ǳ�
void ModifyPlayerName(char name[50]);

// �޸�����Ա�
void ModifyPlayerGender(int gender);

// ����÷�
int CalFinalScore();

// ����û������а�
void AddPlayerToLeaderboard(Player* player);
// ָ���±�������
void MovePlayerToEnd(Player* player, int index);
// �����а��������
void SortLeaderboard();
// ��������Player
void SwapPlayer(Player* a, Player* b);
// ��������ķ�������
int Partition(Player arr[], int low, int high);
// ����������
void QuickSort(Player arr[], int low, int high);

// ������Ϸ
void ContinueGame();