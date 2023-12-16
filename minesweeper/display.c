#include "game.h"  # ��Ҫ���� game.h ��Ϊ���ǻ�ʹ�õ���ǰ���� board_config

void DisplayGameState() {
	for (int i = 0; i <= board_config.rows; ++i) {
		printf("%2d ", i);
	}
	printf("\n");
	for (int i = 1; i <= board_config.rows; ++i) {
		for (int j = 0; j <= board_config.cols; ++j) {
			if (j == 0) {
				printf("%2d ", game->board[i][j].value);
			}
			else {
				printf("%2c ", game->board[i][j].value);
			}
		}
		printf("\n");
	}
}

void DisplayErrorMsg(const char* message) {
	printf("����:");
	printf("%s\n", message);
}

void DisplayPlayer() {
	printf("%s%s", player->name, player->gender);
}

void DisplayPlayerInfo() {
	printf("�ǳƣ�%s\n�Ա�%s\n��߳ɼ���%d\n", player->name, player->gender, player->best_score);
}

void DisplayGameOver() {
	printf("����ɼ���\n");
	printf("���ֵ÷֡���%d\n", player->score);
	printf("��ʷ��ߡ���%d\n", player->best_score);
}

void DisplayLeaderboard() {
	for (int i = 0; i < leaderboard->player_count; i++) {
		printf("%d. %s: %d\n", i + 1, leaderboard->players[i].name, leaderboard->players[i].score);
	}
}