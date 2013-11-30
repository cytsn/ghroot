//lifeGameTools.h
#include <windows.h>

#pragma once
#include "lifeGameMain.h"

int CellDraw(HDC, int);
int CellFill(HDC, HBRUSH [2], int [BSIZE][BSIZE], int [BSIZE][BSIZE]);
int CellClicked(int, int, int [BSIZE][BSIZE]);
int CellCalculation(int [BSIZE][BSIZE], int [BSIZE][BSIZE]);
int StillLifeTub(int, int, int [BSIZE][BSIZE]);