/************************************************************************
**
** NAME:        gameoflife.c
**
** DESCRIPTION: CS61C Fall 2020 Project 1
**
** AUTHOR:      Justin Yokota - Starter Code
**			Liu ZETONG	
**
**
** DATE:        2025-03-22
**
**************************************************************************/

#include <stdio.h>
#include <stdlib.h>
#include <inttypes.h>
#include "imageloader.h"

//Determines what color the cell at the given row/col should be. This function allocates space for a new Color.
//Note that you will need to read the eight neighbors of the cell in question. The grid "wraps", so we treat the top row as adjacent to the bottom row
//and the left column as adjacent to the right column.
Color *evaluateOneCell(Image *image, int row, int col, uint32_t rule)
{
	// 基本信息初始化
	uint32_t rows = image->rows;
	uint32_t cols = image->cols;
	uint8_t lifeFlagR, lifeFlagG, lifeFlagB, 
	       	nextLifeFlagR, nextLifeFlagG, nextLifeFlagB, 
		numAliveR, numAliveG, numAliveB;

	// 申请颜色内存
	Color* cellColor = (Color*)malloc(sizeof(Color));
	if(cellColor == NULL)
	{
		printf("malloc ERROR !!!\n");
		exit(-1);
	}
	cellColor->R = cellColor->G = cellColor->B = 0;

	// 获取当前生命，如果活着，白色，255；否则死了，黑色，0
	// lifeFlag = image->image[row][col].R==255 ? 1 : 0;
	// 进阶问题，24位颜色，要分开判断
	lifeFlagR = image->image[row][col].R;
	lifeFlagG = image->image[row][col].G;
	lifeFlagB = image->image[row][col].B;

	// 获取活邻居数量
	for(uint8_t bit = 0; bit < 8; bit ++)
	{
		numAliveR = numAliveG = numAliveB = 0;

		for(uint8_t i =0; i < 3; i ++)
		{
			for(uint8_t j = 0; j < 3; j ++)
			{
				if(i == 1 && j == 1)
				{
					continue;
				}
			
				if((image->image[(row-1+i+rows)%rows][(col-1+j+cols)%cols].R) & (1 << bit))
				{
					numAliveR ++;
				}	
				if((image->image[(row-1+i+rows)%rows][(col-1+j+cols)%cols].G) & (1 << bit))
				{
					numAliveG ++;
				}
				if((image->image[(row-1+i+rows)%rows][(col-1+j+cols)%cols].B) & (1 << bit))
				{
					numAliveB ++;
				}
			}
		}

		nextLifeFlagR = ((rule >> (((lifeFlagR >> bit) & 1)*9)) >> numAliveR) & 1;
		nextLifeFlagG = ((rule >> (((lifeFlagG >> bit) & 1)*9)) >> numAliveG) & 1;
		nextLifeFlagB = ((rule >> (((lifeFlagB >> bit) & 1)*9)) >> numAliveB) & 1;
		
		cellColor->R |= (nextLifeFlagR << bit);
		cellColor->G |= (nextLifeFlagG << bit);
		cellColor->B |= (nextLifeFlagB << bit);
	}

	return cellColor;
}

//The main body of Life; given an image and a rule, computes one iteration of the Game of Life.
//You should be able to copy most of this from steganography.c
Image *life(Image *image, uint32_t rule)
{
	Image* newLife = (Image*)malloc(sizeof(Image));
	newLife->rows = image->rows;
	newLife->cols = image->cols;

	Color** allCells = (Color**)malloc(newLife->rows * sizeof(Color*));
	for(uint32_t i = 0; i < newLife->rows; i ++)
	{
		allCells[i] = (Color*)malloc(newLife->cols * sizeof(Color));
	}

	for(uint32_t row = 0; row < newLife->rows; row ++)
	{
		for(uint32_t col = 0; col < newLife->cols; col ++)
		{
			Color* handle = evaluateOneCell(image, row, col, rule);
			allCells[row][col] = *handle;
			free(handle);	// 及时销毁，感觉这里值传递即可，传来指针的话就要解引用完就销毁。
		}
	}

	newLife->image = allCells;
	return newLife;

}

/*
Loads a .ppm from a file, computes the next iteration of the game of life, then prints to stdout the new image.

argc stores the number of arguments.
argv stores a list of arguments. Here is the expected input:
argv[0] will store the name of the program (this happens automatically).
argv[1] should contain a filename, containing a .ppm.
argv[2] should contain a hexadecimal number (such as 0x1808). Note that this will be a string.
You may find the function strtol useful for this conversion.
If the input is not correct, a malloc fails, or any other error occurs, you should exit with code -1.
Otherwise, you should return from main with code 0.
Make sure to free all memory before returning!

You may find it useful to copy the code from steganography.c, to start.
*/
int main(int argc, char **argv)
{
	if(argc != 3)
	{
		printf("usage: ./gameOfLife filename rule\nfilename is an ASCII PPM file (type P3) with maximum value 255.\nrule is a hex number beginning with 0x; Life is 0x1808.\n");
		exit(-1);
	}
	Image *nowFrame, *nextFrame;
	char* filename = argv[1], *endptr;
	uint32_t rule = strtol(argv[2], &endptr, 0);
	if(*endptr != '\0')
	{
		printf("rule ERROR !!!\n");
		exit(-1);
	}

	nowFrame = readData(filename);
	nextFrame = life(nowFrame, rule);
	writeData(nextFrame);

	freeImage(nowFrame);
	freeImage(nextFrame);

	return 0;

}
