#pragma once
#define _CRT_SECURE_NO_WARNINGS
// ���˵� scanf �� msvc �²���ȫ������
#include <stdbool.h>
#include <stdio.h>
#include <string.h>

// ��ʾ���˵�
void ShowMenu();

// �������˵�ѡ��
int HandleMenuChoice();

// ��ʾ�Ѷȵȼ�
void ShowLevelMenu();

// �����Ѷȵȼ�
void HandleLevelChoice();

// ������ҵĲ˵�
void ShowPlayerMenu();

// ��������Ա�Ĳ˵�
void ShowGenderMenu();

// ����������ҵ�ѡ��
void HandlePlayerChoice();
