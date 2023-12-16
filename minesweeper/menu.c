#include "game.h"
#include "menu.h"

void ShowMenu() {
	printf("------------��  ��------------\n");
	printf("   1 ��ʼ��Ϸ\t2 ������Ϸ  \n");
	printf("   3 �����û�\t4 ѡ���Ѷ�   \n");
	printf("   5 ������Ϸ\t6 Ԥ������  \n");
	printf("    ��������� 0 �˳�����  \n");
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
	printf("-----�Ѷȵȼ�-----\n");
	printf("   1 �ǳ�����   \n");
	printf("   2 �е��Ѷ�   \n");
	printf("   3 �ϵ�ǿ��   \n");
	printf("-----------------\n");
}

void HandleLevelChoice() {
	int choice = 0;
	printf("[ѡ���Ѷ�]>>> ");
	scanf("%d", &choice);
	// TODO ���ú�int���͵ı���choice ȥ�����Ѷ�
	ModifyDifficulty(choice);
}

void ShowPlayerMenu() {
	DisplayPlayerInfo();
	printf("1 �޸��ǳ�\t2 �޸��Ա�\t0 �˳�\n");
}

void ShowGenderMenu() {
	printf("-----�Ա�ѡ��-----\n");
	printf("   1 ��Ϊ��ʿ   \n");
	printf("   2 ��ΪŮʿ   \n");
	printf("   3 �Ҷ���Ҫ   \n");
	printf("-----------------\n");
}

void HandlePlayerChoice() {
	int choice = 0;
	printf("[�����û�]>>> ");
	scanf("%d", &choice);
	switch (choice)
	{
	case 1: {
		char name[50] = "";
		printf("�������û��ǳƣ�");
		scanf("%s", name);
		ModifyPlayerName(name);  // �Ժ��� game.c ��ʵ��
		break;
	}
	case 2: {
		ShowGenderMenu();
		int gender_choice = 0;
		printf("��ѡ����������Ա�");
		scanf(" %d", &gender_choice);
		ModifyPlayerGender(gender_choice);  // �Ժ��� game.c ��ʵ��
		break;
	}
	default: {
		break;
	}
	}
}