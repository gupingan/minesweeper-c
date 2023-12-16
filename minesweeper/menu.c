#include "game.h"
#include "menu.h"

void ShowMenu() {
	printf("------------菜  单------------\n");
	printf("   1 开始游戏\t2 继续游戏  \n");
	printf("   3 设置用户\t4 选择难度   \n");
	printf("   5 保存游戏\t6 预览排行  \n");
	printf("    你可以输入 0 退出程序  \n");
	printf("------------------------------\n");
}

int HandleMenuChoice() {
	int choice = 0;
	printf(">>> ");
	scanf("%d", &choice);
	if (choice <= 6 && choice >= 0) return choice;
	else return -1;
}

void ShowLevelMenu() {
	printf("-----难度等级-----\n");
	printf("   1 非常轻松   \n");
	printf("   2 有点难度   \n");
	printf("   3 上点强度   \n");
	printf("-----------------\n");
}

void HandleLevelChoice() {
	int choice = 0;
	printf("[选择难度]>>> ");
	scanf("%d", &choice);
	// TODO 利用好int类型的变量choice 去设置难度
	ModifyDifficulty(choice);
}

void ShowPlayerMenu() {
	DisplayPlayerInfo();
	printf("1 修改昵称\t2 修改性别\t0 退出\n");
}

void ShowGenderMenu() {
	printf("-----性别选择-----\n");
	printf("   1 成为男士   \n");
	printf("   2 成为女士   \n");
	printf("   3 我都不要   \n");
	printf("-----------------\n");
}

void HandlePlayerChoice() {
	int choice = 0;
	printf("[设置用户]>>> ");
	scanf("%d", &choice);
	switch (choice)
	{
	case 1: {
		char name[50] = "";
		printf("请设置用户昵称：");
		scanf("%s", name);
		ModifyPlayerName(name);  // 稍后在 game.c 中实现
		break;
	}
	case 2: {
		ShowGenderMenu();
		int gender_choice = 0;
		printf("请选择序号设置性别：");
		scanf(" %d", &gender_choice);
		ModifyPlayerGender(gender_choice);  // 稍后在 game.c 中实现
		break;
	}
	default: {
		break;
	}
	}
}