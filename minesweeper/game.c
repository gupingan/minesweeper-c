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
	// Ԥ�������ȱʡ��Ϣ
	player = (Player*)malloc(sizeof(Player));
	if (player == NULL) return;
	strcpy(player->name, "��������");
	strcpy(player->gender, "");
	player->score = 0;
	player->best_score = 0;
	player->error_flag = 0;
	player->right_flag = 0;
	// ������Ϸ����
	game = CreateGame(GAME_INIT, EASY);
	// Ԥ�������а�
	leaderboard = (Leaderboard*)malloc(sizeof(Leaderboard));
	leaderboard->player_count = 0;
}

Game* CreateGame(GameState state, Diffuculty level) {
	Game* game = (Game*)malloc(sizeof(Game));  // ��̬����
	if (game == NULL) return;

	game->time = 0; // ��ʼ��ʱ��Ϊ0
	game->state = state;  // �Խ����ⲿ�����״̬,һ���� GAME_INIT
	game->difficult = level;  // �����Ѷȵȼ�,��������ᴫ�� EASY
	board_config = board_configs[game->difficult];  // �����Ѷȵȼ���ö��ֵ,��ȡ��Ӧ������

	// ����������������ȥ��̬������Ϸ��
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

	return game;  // ��̬�����ɹ��󷵻�gameָ��
}

void InitGame() {
	// ����ÿ�����̶��Ǽ����Ϸ״̬��,���������ϸ�ĶԹ��̿���
	if (game->state != GAME_INIT) return;
	// �µ���Ϸʱ��ʼ������
	game->time = 0;
	player->score = 0;
	player->error_flag = 0;
	player->right_flag = 0;
	// ��ʼ���кš��кš���Ԫ��
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

	// ����
	int mine_count = board_config.mine_count;  // ��ȡ��ǰ����ָ����������
	int mine_row = 0, mine_col = 0;  // ��ʼ�����׵���������
	srand((unsigned int)time(NULL));
	while (mine_count > 0) {  // ������Ϊ0,�Ͳ�������
		mine_row = rand() % rows + 1;  // ��ȡ 1 ~ rows ��Χ��ֵ(ǧ��Ҫ����,���ǵ���Ϸ����ʲô����!!!)
		mine_col = rand() % cols + 1;  // ��ȡ 1 ~ cols ��Χ��ֵ
		if (game->board[mine_row][mine_col].is_mine) continue;  // ���λ���Ѿ�������,������
		game->board[mine_row][mine_col].is_mine = true;  // ��û�в��ù��ĵط�������
		game->board[mine_row][mine_col].value = cell_unexplored;  // ��ʾδ̽��,Ҳ����������
		//game->board[mine_row][mine_col].value = cell_mine;  // ���Դ���,�⿪ע�ͺ����ܿ������õ���
		mine_count--;  // ����һ�ŵ����Ǿ��Լ�һ
	}

	// ͳ����
	for (int row = 1; row < real_rows; ++row) {
		for (int col = 1; col < real_cols; ++col) {
			game->board[row][col].adjacent_mines = GetMineNearCell(row, col);  // TODO ����Ҫʵ��GetMineNearCell����
		}
	}

	game->state = GAME_RUNNING;  // ��ʼ���ú�,������Ϸ״̬��������,�ȴ�����
}

int GetMineNearCell(int row, int col) {
	int mine_count = 0, new_row = 0, new_col = 0;
	int row_offsets[] = { -1, -1, -1, 0, 0, 1, 1, 1 };
	int col_offsets[] = { -1, 0, 1, -1, 1, -1, 0, 1 };
	for (int i = 0; i < 8; ++i) {
		new_row = row + row_offsets[i];
		new_col = col + col_offsets[i];
		if (new_row < 1 || new_row > board_config.rows || new_col < 1 || new_col > board_config.cols)
			continue;  // ��Χ��8�������г��ֲ��ڷ�Χ��,�����Ϸ����������
		if (game->board[new_row][new_col].is_mine) {
			mine_count++;  // �����Χ������,���ñ���+1����ͳ��
		}

	}
	return mine_count;
}

int HandleInput(char operate, int row, int col) {
	// �ú������յĲ����ֱ��� ������ �к� �к�
	if (row < 1 || row > board_config.rows || col < 1 || col > board_config.cols) {
		DisplayErrorMsg("������кź��кŲ�������Ч��Χ�ڣ�");
		return -1;  // У���кź��к��Ƿ���Ч ����-1��ʾ�����У��ⲿ��⵽����Ҫ����������
	}

	switch (operate)
	{
	case 'e':
		if (game->state == GAME_RUNNING) {
			game->state = GAME_ENTER;  // �޸���Ϸ״̬
			UpdateGameState(row, col);  // ������Ϸ������Ϣ
			if (CheckGameOver()) {  // �ж���Ϸ�Ƿ����
				EndGame();  // �����Ϸ����ʱ��ʲô,��С�ڲ�����
			}
			else {
				game->state = GAME_RUNNING;  // ��Ϸû�н������ָ���Ϸ״̬
			}
		}
		break;
	case 'f': {
		if (game->state == GAME_RUNNING) {
			game->state = GAME_ENTER;
			if (!game->board[row][col].is_revealed) {
				game->board[row][col].is_flagged = !game->board[row][col].is_flagged;
				// ����Ǳ��
				if (game->board[row][col].is_flagged) {
					game->board[row][col].value = cell_flagged;
					if (game->board[row][col].is_mine)
						player->right_flag += 1;
					else
						player->error_flag += 1;

				}
				else {  // �����ȡ�����
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
	case 'q':  // �˳�ʱ��Ȼ��Ҫ�����꣨�е㲻����������������
		game->state = GAME_INIT;  // �˳���Ϸ����ζ��״̬�ָ�������ʼ��
		break;
	default:
		break;
	}
	return game->state;  // ����Ϸ״̬�׳�ȥ������״̬����
}

void UpdateGameState(int row, int col) {
	game->state = GAME_UPDATE;
	Cell* cell = &(game->board[row][col]);
	// ̽������ ����̽��
	if (cell->is_revealed) {
		DisplayErrorMsg("���Ѿ�̽����������򿩣�");
		return;
	}
	// ��ǹ��� ����̽��
	if (cell->is_flagged) {
		DisplayErrorMsg("���Ѿ������������򣬲���̽��Ŷ��");
		return;
	}
	// ���������ˡ���ʵ��Ӧ�÷��֣������˺���û�����ٹ���
	// ��������������о�������ͨ����Ϸ״̬ȥ�������ģ�
	if (cell->is_mine) {
		game->state = GAME_LOSE;  // ���׺��޸���Ϸ״̬
		cell->is_revealed = true;  // �޸�Ϊ��̽��
	}
	else
	{
		// �·�����û���׵����
		// ��ǰ���Ӹ�������
		if (cell->adjacent_mines != 0) {
			cell->value = 48 + cell->adjacent_mines;
			cell->is_revealed = true;
		}
		else  // ��ǰ���Ӹ���û���ף��ǻ�����Ҷ��������Ե�����8������������������
		{
			cell->value = cell_empty;  // ��ǰ���Ӹ���û�ף��� cell_empty
			cell->is_revealed = true;  // ��ǰ���Ӹ�Ϊ��̽��
			int new_row = 0, new_col = 0;  // �������ӵ����кų�ʼ��
			int row_offsets[] = { -1, -1, -1, 0, 0, 1, 1, 1 };
			int col_offsets[] = { -1, 0, 1, -1, 1, -1, 0, 1 };
			for (int i = 0; i < 8; ++i) {
				// ���ܵ���ƫ��1λ��Ȼ�󰤸���״̬ 
				new_row = row + row_offsets[i];
				new_col = col + col_offsets[i];
				Cell new_cell = game->board[new_row][new_col];
				// ��ǰ����߽��Լ���̽�����ѱ�ǵ����
				if (new_row < 1 || new_row > board_config.rows || new_col < 1 || new_col > board_config.cols) continue;
				if (new_cell.is_revealed || new_cell.is_flagged) continue;
				// �ݹ�ĸ�����Χ����
				UpdateGameState(new_row, new_col);
			}
		}
	}
}

bool CheckGameOver() {
	// �ж���Ϸʧ��
	if (game->state == GAME_LOSE) {
		DisplayPlayer();
		printf("���׿�����Ϸʧ�ܣ�����\n");
		return true;
	}
	if (game->state != GAME_UPDATE) return;
	// ����Ƿ�ʤ��
	for (int row = 1; row <= board_config.rows; ++row) {
		for (int col = 1; col <= board_config.cols; ++col) {
			if (!game->board[row][col].is_mine && !game->board[row][col].is_revealed)
				return false;   // ֻҪ��һ������Ԫ��û�б�̽����������ûʤ�����
		}
	}
	// ʤ�������
	game->state = GAME_WIN;
	DisplayPlayer();
	printf("������Ŷ���˼Һ�ϲ��~\n");
	return true;
}

void ShowAllMines() {
	// ���ã���Ϸ����ʱ���� EndGame�����л������
	for (int row = 1; row <= board_config.rows; ++row) {
		for (int col = 1; col <= board_config.cols; ++col) {
			if (game->board[row][col].is_mine)
				game->board[row][col].value = cell_mine;
			// ��Ϸ����ʱ������Ϸ�������׵�valueȫ����Ϊ��
		}
	}
}

void StartGame() {
	// ���ã���main.c�б����� Ҳ���ǿ�ʼ��Ϸ��ʱ��
	char operate = 'e';  // ��ʼ��������
	int row = 0, col = 0, game_state = 0;  // ��ʼ�����кš���Ϸ״̬
	InitGame();  // ��ʼ����Ϸ���õ������׵���Ϸ��
	start_time = time(NULL);
	while (true)
	{
		// ��֤����ǰ �ܿ�������
		DisplayGameState();
		printf("��������e ̽��\tf ���\tq ��ֹ\t ��ʽ[������ �к� �к�]\n");
		printf("������");
		// �������벢����Ӧ�����������
		scanf(" %c %d %d", &operate, &row, &col);
		game_state = HandleInput(operate, row, col);
		if (game_state == 0) break;  // ��Ϸ״̬�ص� GAME_INIT ���˳���Ϸ��
		if (game_state == -1) continue;  // ��������в��Ϸ�����
	}
}

void EndGame() {
	end_time = time(NULL);
	game->time += (int)(end_time - start_time);
	// ���ã�ʤ������ʧ�ܺ���� HandleInput�����е�����
	// ��ʾ��Ϸ���������Ϣ
	ShowAllMines();
	DisplayGameState();

	// TODO �Ʒֲ��ܽ�ɼ�
	CalFinalScore();  // �����������÷�
	DisplayGameOver();  // �������������Ϸ�����ɼ�

	// TODO �Ʒֺ�������а�
	AddPlayerToLeaderboard(player);
	SaveLeaderboard();

	// ״̬�ָ�������ʼ��
	// ���³�ʼ״̬��Ϊ�˷�ֹ������Ϸ����ֵĻ�����Ϸ�����Ļ���
	game->state = GAME_INIT;
	InitGame();  // �����Ϸ�岢�ָ�����ʼ
	game->state = GAME_INIT;
}

void ModifyDifficulty(int choice) {
	game = CreateGame(GAME_INIT, (Diffuculty)(choice - 1));
}

void ModifyPlayerName(char name[50]) {
	if (strcmp(player->name, name) != 0) {
		// ����ǳƺ�֮ǰ�Ĳ�һ�������³�ʼ���û��������Ϣ
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
		// �޸Ľṹ���е��ַ��������ʹ�� strcpy ����
		strcpy(player->gender, "��ʿ");
		break;
	}
	case 2: {
		strcpy(player->gender, "Ůʿ");
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
		SortLeaderboard(); // ������󣬽��е�������
	}
	else {
		// �ȵ�������Ȼ������жϺ��ƶ�
		SortLeaderboard();
		int last_index = MAX_PLAYERS - 1;
		if (leaderboard->players[last_index].score >= player->score)
			return;  // �ϰ��ʸ���϶�
		for (last_index; last_index >= 0; last_index--) {
			if (leaderboard->players[last_index].score < player->score) {
				continue;
			}
			else {
				int get_index = last_index + 1;
				MovePlayerToEnd(player, get_index);
				return; // ������ǰ����Ҹ�
			}
		}
		// �����������㣬�������԰�
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
	if (!load_state) {  // ���سɹ���״̬
		InitGame();
	}
	else {
		// ������سɹ�����״̬����Ϊ��Ϊ GAME_RUNNING�����ǲ��ܶ������
		game->state = GAME_RUNNING;
	}
	while (true)
	{
		DisplayGameState();
		printf("��������e ̽��\tf ���\tq ��ֹ\t ��ʽ[������ �к� �к�]\n");
		printf("������");
		scanf(" %c %d %d", &operate, &row, &col);
		game_state = HandleInput(operate, row, col);
		if (game_state == 0) {
			SaveGame();  // ������Ϸ
			break;
		}
		if (game_state == -1) continue;
	}
}