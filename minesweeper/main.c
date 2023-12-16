#include "menu.h"

int main() {
	int choice = 0;
	bool save_state = false;
	Preload();
	LoadLeaderboard();
	if (!LoadGame()) InitGame();
	do
	{
		ShowMenu();
		choice = HandleMenuChoice();
		if (choice == 0) break;
		if (choice == -1) continue;
		switch (choice)
		{
		case 1: {
			StartGame();
			break;
		}
		case 2: {
			ContinueGame();
			break;
		}
		case 3: {
			ShowPlayerMenu();
			HandlePlayerChoice();
			break;
		}
		case 4: {
			ShowLevelMenu();
			HandleLevelChoice();
			break;
		}
		case 5: {
			save_state = SaveGame();
			if (save_state) printf("保存成功\n");
			else printf("保存失败\n");
			break;
		}
		case 6: {
			DisplayLeaderboard();
			break;
		}
		default:
			break;
		}
	} while (true);

    return 0;
}