#include "game.h"

void Preload() {
	BoardConfig temp_configs[DIFFICULTY_COUNT] = {
		{9, 9, 11, 11, 10, 1000},
		{16, 16, 18, 18, 40, 2000},
		{24, 24, 26, 26, 99, 3000}
	};
	board_configs = (BoardConfig*)malloc(DIFFICULTY_COUNT * sizeof(BoardConfig));
	for (int i = 0; i < DIFFICULTY_COUNT; ++i) {
		board_configs[i] = temp_configs[i];
	}
	cell_unexplored = 46;  // .
	cell_empty = 48;  // 0
	cell_mine = 42;  // *
	cell_flagged = 70;  // F
	// 预加载玩家缺省信息
	player = (Player*)malloc(sizeof(Player));
	if (player == NULL) return;
	strcpy(player->name, "无名大侠");
	strcpy(player->gender, "");
	player->score = 0;
	player->best_score = 0;
	player->error_flag = 0;
	player->right_flag = 0;
	// 创建游戏对象
	game = CreateGame(GAME_INIT, EASY);
	// 预加载排行榜
	leaderboard = (Leaderboard*)malloc(sizeof(Leaderboard));
	leaderboard->player_count = 0;
}

Game* CreateGame(GameState state, Diffuculty level) {
	Game* game = (Game*)malloc(sizeof(Game));  // 动态创建
	if (game == NULL) return;

	game->time = 0; // 初始化时间为0
	game->state = state;  // 对接上外部赋予的状态,一般是 GAME_INIT
	game->difficult = level;  // 传递难度等级,我们这里会传入 EASY
	board_config = board_configs[game->difficult];  // 根据难度等级的枚举值,获取对应的配置

	// 接下来就利用配置去动态创建游戏板
	game->board = (Cell**)malloc(board_config.real_rows * sizeof(Cell*));
	if (game->board == NULL) {
		free(game);
		return;
	}

	for (int i = 0; i < board_config.real_rows; ++i) {
		game->board[i] = (Cell*)malloc(board_config.real_cols * sizeof(Cell));
		if (game->board[i] == NULL) {
			for (int j = 0; j < i; j++)
				free(game->board[j]);
			free(game->board);
			free(game);
			return;
		}
	}

	return game;  // 动态创建成功后返回game指针
}

void InitGame() {
	// 我们每个过程都是检测游戏状态的,这样更加严格的对过程控制
	if (game->state != GAME_INIT) return;
	// 新的游戏时初始化参数
	game->time = 0;
	player->score = 0;
	player->error_flag = 0;
	player->right_flag = 0;
	// 初始化行号、列号、单元格
	int rows = board_config.rows;
	int cols = board_config.cols;
	int real_rows = board_config.real_rows;
	int real_cols = board_config.real_cols;
	for (int i = 0; i < real_rows; ++i) {
		Cell edge_cell = { false, false, false, 0, i };
		game->board[i][0] = edge_cell;
	}
	for (int i = 0; i < real_cols; ++i) {
		Cell edge_cell = { false, false, false, 0, i };
		game->board[0][i] = edge_cell;
	}
	for (int row = 1; row < real_rows; ++row) {
		for (int col = 1; col < real_cols; ++col) {
			Cell init_cell = { false, false, false, 0, cell_unexplored };
			game->board[row][col] = init_cell;
		}
	}

	// 埋雷
	int mine_count = board_config.mine_count;  // 获取当前配置指定的雷数量
	int mine_row = 0, mine_col = 0;  // 初始化埋雷的行列坐标
	srand((unsigned int)time(NULL));
	while (mine_count > 0) {  // 当数量为0,就不再埋雷
		mine_row = rand() % rows + 1;  // 获取 1 ~ rows 范围的值(千万不要忘记,我们的游戏板是什么样的!!!)
		mine_col = rand() % cols + 1;  // 获取 1 ~ cols 范围的值
		if (game->board[mine_row][mine_col].is_mine) continue;  // 这个位置已经埋雷了,就跳过
		game->board[mine_row][mine_col].is_mine = true;  // 对没有布置过的地方布置雷
		game->board[mine_row][mine_col].value = cell_unexplored;  // 显示未探索,也就是隐藏嘛
		//game->board[mine_row][mine_col].value = cell_mine;  // 测试代码,解开注释后你能看到布置的雷
		mine_count--;  // 布置一颗地雷那就自减一
	}

	// 统计雷
	for (int row = 1; row < real_rows; ++row) {
		for (int col = 1; col < real_cols; ++col) {
			game->board[row][col].adjacent_mines = GetMineNearCell(row, col);  // TODO 我们要实现GetMineNearCell函数
		}
	}

	game->state = GAME_RUNNING;  // 初始化好后,更换游戏状态到运行中,等待输入
}

int GetMineNearCell(int row, int col) {
	int mine_count = 0, new_row = 0, new_col = 0;
	int row_offsets[] = { -1, -1, -1, 0, 0, 1, 1, 1 };
	int col_offsets[] = { -1, 0, 1, -1, 1, -1, 0, 1 };
	for (int i = 0; i < 8; ++i) {
		new_row = row + row_offsets[i];
		new_col = col + col_offsets[i];
		if (new_row < 1 || new_row > board_config.rows || new_col < 1 || new_col > board_config.cols)
			continue;  // 周围的8个坐标中出现不在范围的,即不合法坐标会跳过
		if (game->board[new_row][new_col].is_mine) {
			mine_count++;  // 如果周围出现雷,就让变量+1进行统计
		}

	}
	return mine_count;
}

int HandleInput(char operate, int row, int col) {
	// 该函数接收的参数分别是 操作符 行号 列号
	if (row < 1 || row > board_config.rows || col < 1 || col > board_config.cols) {
		DisplayErrorMsg("输入的行号和列号并不在有效范围内！");
		return -1;  // 校验行号和列号是否有效 返回-1表示不得行，外部检测到可以要求重新输入
	}

	switch (operate)
	{
	case 'e':
		if (game->state == GAME_RUNNING) {
			game->state = GAME_ENTER;  // 修改游戏状态
			UpdateGameState(row, col);  // 更新游戏界面信息
			if (CheckGameOver()) {  // 判断游戏是否结束
				EndGame();  // 这局游戏结束时干什么,本小节不讲解
			}
			else {
				game->state = GAME_RUNNING;  // 游戏没有结束，恢复游戏状态
			}
		}
		break;
	case 'f': {
		if (game->state == GAME_RUNNING) {
			game->state = GAME_ENTER;
			if (!game->board[row][col].is_revealed) {
				game->board[row][col].is_flagged = !game->board[row][col].is_flagged;
				// 如果是标记
				if (game->board[row][col].is_flagged) {
					game->board[row][col].value = cell_flagged;
					if (game->board[row][col].is_mine)
						player->right_flag += 1;
					else
						player->error_flag += 1;

				}
				else {  // 如果是取消标记
					game->board[row][col].value = cell_unexplored;
					if (game->board[row][col].is_mine)
						player->right_flag -= 1;
					else
						player->error_flag -= 1;
				}
			}
			game->state = GAME_RUNNING;
		}
		break;
	}
	case 'q':  // 退出时依然需要给坐标（有点不合理，忍忍老铁！）
		game->state = GAME_INIT;  // 退出游戏，意味着状态恢复到待初始化
		break;
	default:
		break;
	}
	return game->state;  // 将游戏状态抛出去，根据状态干事
}

void UpdateGameState(int row, int col) {
	game->state = GAME_UPDATE;
	Cell* cell = &(game->board[row][col]);
	// 探索过了 不能探索
	if (cell->is_revealed) {
		DisplayErrorMsg("你已经探索过这个区域咯！");
		return;
	}
	// 标记过了 不能探索
	if (cell->is_flagged) {
		DisplayErrorMsg("你已经标记了这个区域，不能探索哦！");
		return;
	}
	// 老铁踩雷了。其实你应该发现，踩雷了好像没做多少工作
	// 但是你往后继续研究，我是通过游戏状态去做工作的！
	if (cell->is_mine) {
		game->state = GAME_LOSE;  // 踩雷后修改游戏状态
		cell->is_revealed = true;  // 修改为已探索
	}
	else
	{
		// 下方都是没踩雷的情况
		// 当前格子附近有雷
		if (cell->adjacent_mines != 0) {
			cell->value = 48 + cell->adjacent_mines;
			cell->is_revealed = true;
		}
		else  // 当前格子附近没有雷，那还用玩家动脑吗，无脑点四周8个，我们这里程序代劳
		{
			cell->value = cell_empty;  // 当前格子附近没雷，给 cell_empty
			cell->is_revealed = true;  // 当前格子改为已探索
			int new_row = 0, new_col = 0;  // 附近格子的行列号初始化
			int row_offsets[] = { -1, -1, -1, 0, 0, 1, 1, 1 };
			int col_offsets[] = { -1, 0, 1, -1, 1, -1, 0, 1 };
			for (int i = 0; i < 8; ++i) {
				// 四周导出偏移1位，然后挨个该状态 
				new_row = row + row_offsets[i];
				new_col = col + col_offsets[i];
				Cell new_cell = game->board[new_row][new_col];
				// 提前处理边界以及已探索和已标记等情况
				if (new_row < 1 || new_row > board_config.rows || new_col < 1 || new_col > board_config.cols) continue;
				if (new_cell.is_revealed || new_cell.is_flagged) continue;
				// 递归的更新周围格子
				UpdateGameState(new_row, new_col);
			}
		}
	}
}

bool CheckGameOver() {
	// 判断游戏失败
	if (game->state == GAME_LOSE) {
		DisplayPlayer();
		printf("踩雷咯，游戏失败！！！\n");
		return true;
	}
	if (game->state != GAME_UPDATE) return;
	// 检测是否胜利
	for (int row = 1; row <= board_config.rows; ++row) {
		for (int col = 1; col <= board_config.cols; ++col) {
			if (!game->board[row][col].is_mine && !game->board[row][col].is_revealed)
				return false;   // 只要有一个非雷元素没有被探索，就属于没胜利情况
		}
	}
	// 胜利的情况
	game->state = GAME_WIN;
	DisplayPlayer();
	printf("好厉害哦，人家好喜欢~\n");
	return true;
}

void ShowAllMines() {
	// 调用：游戏结束时调用 EndGame函数中会调用它
	for (int row = 1; row <= board_config.rows; ++row) {
		for (int col = 1; col <= board_config.cols; ++col) {
			if (game->board[row][col].is_mine)
				game->board[row][col].value = cell_mine;
			// 游戏结束时，将游戏板上是雷的value全部改为雷
		}
	}
}

void StartGame() {
	// 调用：在main.c中被调用 也就是开始游戏的时候
	char operate = 'e';  // 初始化操作符
	int row = 0, col = 0, game_state = 0;  // 初始化行列号、游戏状态
	InitGame();  // 初始化游戏，得到布满雷的游戏板
	start_time = time(NULL);
	while (true)
	{
		// 保证操作前 能看到棋盘
		DisplayGameState();
		printf("操作符：e 探索\tf 标记\tq 终止\t 格式[操作符 行号 列号]\n");
		printf("操作：");
		// 接收输入并让相应函数处理操作
		scanf(" %c %d %d", &operate, &row, &col);
		game_state = HandleInput(operate, row, col);
		if (game_state == 0) break;  // 游戏状态回到 GAME_INIT 就退出游戏咯
		if (game_state == -1) continue;  // 输入的行列不合法跳过
	}
}

void EndGame() {
	end_time = time(NULL);
	game->time += (int)(end_time - start_time);
	// 调用：胜利或者失败后调用 HandleInput函数中调用它
	// 显示游戏板的完整信息
	ShowAllMines();
	DisplayGameState();

	// TODO 计分并总结成绩
	CalFinalScore();  // 这个方法计算得分
	DisplayGameOver();  // 这个方法结算游戏结束成绩

	// TODO 计分后计入排行榜
	AddPlayerToLeaderboard(player);
	SaveLeaderboard();

	// 状态恢复到待初始化
	// 上下初始状态是为了防止继续游戏后出现的还是游戏结束的画面
	game->state = GAME_INIT;
	InitGame();  // 清空游戏板并恢复到初始
	game->state = GAME_INIT;
}

void ModifyDifficulty(int choice) {
	game = CreateGame(GAME_INIT, (Diffuculty)(choice - 1));
}

void ModifyPlayerName(char name[50]) {
	if (strcmp(player->name, name) != 0) {
		// 如果昵称和之前的不一样，重新初始化用户的相关信息
		strcpy(player->gender, "");
		player->score = 0;
		player->best_score = 0;
		player->right_flag = 0;
		player->error_flag = 0;
	}
	strcpy(player->name, name);

}

void ModifyPlayerGender(int gender) {
	switch (gender)
	{
	case 1: {
		// 修改结构体中的字符串，务必使用 strcpy 函数
		strcpy(player->gender, "男士");
		break;
	}
	case 2: {
		strcpy(player->gender, "女士");
		break;
	}
	default: {
		strcpy(player->gender, "");
		break;
	}
	}
}

int CalFinalScore() {
	int game_time = game->time;
	int base_score = 0;
	if (game->state == GAME_WIN)
		base_score = board_config.base_score;
	short int right_flag = player->right_flag, error_flag = player->error_flag;
	int score = (int)(base_score * (pow(0.995, game_time)) + 50 * right_flag - 100 * error_flag);
	if (score < 0) score = 0;
	if (score > player->best_score) player->best_score = score;
	player->score = score;
	return score;
}


void AddPlayerToLeaderboard(Player* player) {
	if (leaderboard->player_count < MAX_PLAYERS) {
		leaderboard->players[leaderboard->player_count] = *player;
		leaderboard->player_count++;
		SortLeaderboard(); // 增加完后，进行倒序排序
	}
	else {
		// 先倒序排序，然后进行判断和移动
		SortLeaderboard();
		int last_index = MAX_PLAYERS - 1;
		if (leaderboard->players[last_index].score >= player->score)
			return;  // 上榜资格的认定
		for (last_index; last_index >= 0; last_index--) {
			if (leaderboard->players[last_index].score < player->score) {
				continue;
			}
			else {
				int get_index = last_index + 1;
				MovePlayerToEnd(player, get_index);
				return; // 分数比前个玩家高
			}
		}
		// 当上述不满足，即分数霸榜
		MovePlayerToEnd(player, 0);
	}
}

void SwapPlayer(Player* a, Player* b) {
	Player t = *a;
	*a = *b;
	*b = t;
}

int Partition(Player arr[], int low, int high) {
	int pivot = arr[low].score;
	int i = low, j = high;
	while (i < j) {
		while (i < j && arr[j].score < pivot)
		{
			j--;
		}
		if (i < j) {
			SwapPlayer(&arr[j], &arr[i]);
			i++;
		}
		while (i < j && arr[i].score > pivot)
		{
			i++;
		}
		if (i < j) {
			SwapPlayer(&arr[i], &arr[j]);
			j--;
		}
		if (i >= j) {
			return j;
		}

	}
	return j;
}

void QuickSort(Player arr[], int low, int high) {
	if (low < high) {
		int pi = Partition(arr, low, high);

		QuickSort(arr, low, pi - 1);
		QuickSort(arr, pi + 1, high);
	}
}

void SortLeaderboard() {
	QuickSort(leaderboard->players, 0, leaderboard->player_count - 1);
}

void MovePlayerToEnd(Player* player, int index) {
	for (int cur = MAX_PLAYERS - 1; cur > index; cur--) {
		leaderboard->players[cur] = leaderboard->players[cur - 1];
	}
	leaderboard->players[index] = *player;
}

void ContinueGame() {
	char operate = 'e';
	int row = 0, col = 0, game_state = 0;
	start_time = time(NULL);
	bool load_state = LoadGame();
	if (!load_state) {  // 加载成功的状态
		InitGame();
	}
	else {
		// 如果加载成功更改状态，因为改为 GAME_RUNNING，我们才能对其操作
		game->state = GAME_RUNNING;
	}
	while (true)
	{
		DisplayGameState();
		printf("操作符：e 探索\tf 标记\tq 终止\t 格式[操作符 行号 列号]\n");
		printf("操作：");
		scanf(" %c %d %d", &operate, &row, &col);
		game_state = HandleInput(operate, row, col);
		if (game_state == 0) {
			SaveGame();  // 保存游戏
			break;
		}
		if (game_state == -1) continue;
	}
}