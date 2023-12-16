#include "game.h"
#include "storage.h"


bool LoadGame() {
    FILE* file = fopen(GAME_FILE, "rb");
    if (file == NULL) return false;
    // 读取当前配置
    fread(&board_config, sizeof(BoardConfig), 1, file);
    // 读取玩家
    player = (Player*)malloc(sizeof(Player));
    if (player == NULL) {
        printf("Failed to allocate memory for player.\n");
        return false;
    }
    fread(player, sizeof(Player), 1, file);
    // 读取游戏对象包括游戏板等数据
    game = (Game*)malloc(sizeof(Game));
    if (game == NULL) {
        printf("Failed to allocate memory for game.\n");
        return false;
    }
    fread(game, sizeof(Game), 1, file);
    game->board = (Cell**)malloc(board_config.real_rows * sizeof(Cell*));
    if (game->board == NULL) {
        printf("Failed to allocate memory for game board.\n");
        return false;
    }
    for (int i = 0; i < board_config.real_rows; i++) {
        game->board[i] = (Cell*)malloc(board_config.real_cols * sizeof(Cell));
        if (game->board[i] == NULL) {
            printf("Failed to allocate memory for game board row.\n");
            return false;
        }
        fread(game->board[i], sizeof(Cell), board_config.real_cols, file);
    }

    fclose(file);
    return true;
}

bool SaveGame() {
    FILE* file = fopen(GAME_FILE, "wb");
    if (file == NULL) return false;
    // 向文件中写入当前配置
    fwrite(&board_config, sizeof(BoardConfig), 1, file);
    // 向文件中写入当前用户
    fwrite(player, sizeof(Player), 1, file);
    // 向文件中写入游戏对象以及游戏板
    fwrite(game, sizeof(Game), 1, file);
    for (int i = 0; i < board_config.real_rows; i++) {
        fwrite(game->board[i], sizeof(Cell), board_config.real_cols, file);
    }

    fclose(file);
    return true;
}

bool SaveLeaderboard() {
    FILE* file = fopen(BOARD_FILE, "wb");
    if (file == NULL) return false;

    fwrite(leaderboard, sizeof(Leaderboard), 1, file);
    fwrite(leaderboard->players, sizeof(Player), leaderboard->player_count, file);

    fclose(file);
    return true;
}

bool LoadLeaderboard() {
    FILE* file = fopen(BOARD_FILE, "rb");
    if (file == NULL) return false;

    fread(leaderboard, sizeof(Leaderboard), 1, file);
    fread(leaderboard->players, sizeof(Player), leaderboard->player_count, file);

    fclose(file);
    return true;
}