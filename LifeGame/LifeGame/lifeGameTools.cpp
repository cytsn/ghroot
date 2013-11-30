//lifeGameTools.cpp
#include "lifeGameTools.h"

int CellDraw(HDC hdc, int pos)
{
	MoveToEx(hdc, pos * CSIZE, 0, NULL);
	LineTo(hdc, pos * CSIZE, BSIZE * CSIZE);
	MoveToEx(hdc, 0, pos * CSIZE, NULL);
	LineTo(hdc, BSIZE * CSIZE, pos * CSIZE);
	
	return 0;
}

int CellFill(HDC hdc, HBRUSH *hBrush ,int celltable[BSIZE][BSIZE] ,int cellsumtable[BSIZE][BSIZE])
{
	int x, y;
	for (x = 0; x < BSIZE; x++) {
		for (y = 0; y < BSIZE; y++) {

			switch (cellsumtable[x][y]){
			case 2:
				SelectObject(hdc, hBrush[2]);
				break;
			case 3:
				SelectObject(hdc, hBrush[3]);
				break;
			default :
				SelectObject(hdc, hBrush[0]);
				break;
			}

			Rectangle(hdc, x * CSIZE, y * CSIZE, ( x + 1 ) * CSIZE, ( y + 1 ) * CSIZE );
		}
	}

	return 0;
}

int CellCalculation(int celltable[BSIZE][BSIZE], int cellsumtable[BSIZE][BSIZE])
{
	int x, y, lifesum=0;
	static int tmptable[BSIZE][BSIZE];

	for( x = 0 ; x < BSIZE ; x++ ){
		for( y = 0 ; y < BSIZE ; y++ ){

			if ( x == 0 || y == 0 || x == BSIZE - 1 || y == BSIZE - 1) {
					tmptable[x][y] = 0;
					cellsumtable[x][y] = lifesum;
					continue;
			}

			lifesum = celltable[x-1][y-1] + celltable[x][y-1] + celltable[x+1][y-1] + celltable[x-1][y] + celltable[x+1][y]
					  + celltable[x-1][y+1] + celltable[x][y+1] + celltable[x+1][y+1];

			if( celltable[x][y] == 0){
				if ( lifesum == 3 ){
					tmptable[x][y] = 1;
					cellsumtable[x][y] = lifesum;
				} else {
					tmptable[x][y] = 0;
					cellsumtable[x][y] = -1;
				}
			} else {
				if ( lifesum == 2 || lifesum == 3 ){
					tmptable[x][y] = 1;
				} else {
					tmptable[x][y] = 0;
				}
				cellsumtable[x][y] = lifesum;
			}

			lifesum = 0;
		}
	}

	for( x = 0 ; x < BSIZE ; x++ ){
		for( y = 0 ; y < BSIZE ; y++ ){
			celltable[x][y] = tmptable[x][y];
		}
	}

	return 0;
}

int CellClicked(int lw, int hw, int celltable[BSIZE][BSIZE])
{
	int x, y;

	x = lw / CSIZE;
	y = hw / CSIZE;
	if (celltable[x][y] == 0)
		celltable[x][y] = 1;

	return 0;
}

int StillLifeTub(int lw, int hw, int celltable[BSIZE][BSIZE])
{
	int x, y;

	x = lw / CSIZE;
	y = hw / CSIZE;

	if (celltable[x][y] == 0)
		celltable[x-1][y] = 1;
		celltable[x][y-1] = 1;
		celltable[x+1][y] = 1;
		celltable[x][y+1] = 1;

	return 0;
}