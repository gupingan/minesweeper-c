#pragma once
#define _CRT_SECURE_NO_WARNINGS
// 过滤掉 scanf 在 msvc 下不安全的问题
#include <stdbool.h>
#include <stdio.h>
#include <string.h>

// 显示主菜单
void ShowMenu();

// 处理主菜单选项
int HandleMenuChoice();

// 显示难度等级
void ShowLevelMenu();

// 设置难度等级
void HandleLevelChoice();

// 设置玩家的菜单
void ShowPlayerMenu();

// 设置玩家性别的菜单
void ShowGenderMenu();

// 处理设置玩家的选项
void HandlePlayerChoice();
