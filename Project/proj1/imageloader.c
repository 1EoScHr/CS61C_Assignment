/************************************************************************
**
** NAME:        imageloader.c
**
** DESCRIPTION: CS61C Fall 2020 Project 1
**
** AUTHOR:      Dan Garcia  -  University of California at Berkeley
**              Copyright (C) Dan Garcia, 2020. All rights reserved.
**              Justin Yokota - Starter Code
**			Liu ZETONG	
**
**
** DATE:        2025-03-21
**
**************************************************************************/

#include <stdio.h>
#include <stdlib.h>
#include <inttypes.h>
#include <string.h>
#include "imageloader.h"

//Opens a .ppm P3 image file, and constructs an Image object. 
//You may find the function fscanf useful.
//Make sure that you close the file with fclose before returning.
Image *readData(char *filename) 
{
	// 初始化
	Image *image = (Image*)malloc(sizeof(Image));
	if(image == NULL)
	{
		printf("malloc ERROR !!!\n");
		exit(-1);
	}	
	char ppmType[3];	
	uint8_t ppmRange;
	
	// 打开ppm文件
	FILE *ppm = fopen(filename, "r");
	if(ppm == NULL)
	{
	    	printf("open the ppm file FAIL !!!");
		exit(-1);
	}

	// 依次处理ppm格式、图片长宽、颜色范围
	fscanf(ppm, "%s", ppmType);
	fscanf(ppm, "%u %u", &image->cols, &image->rows);	// uint32_t 用 %u 输入
	fscanf(ppm, "%hhu", &ppmRange);				// uint8_t 用 %hhu 输入
							
	// 根据图片长宽初始化每个像素
	// Color** allPixels = (Color**)malloc(image->rows*image->cols * sizeof(Color))
	// 上面这个写法实际上申请的是一个一维数组，不能通过二维数组的方式访问
	//
	// 想要用二维数组方式，就要逐维度定义：第一维是Color**，有rows个Color*；第二维是对于每一个Color*，有cols个Color
	Color** allPixels = (Color**)malloc(image->rows * sizeof(Color*));
	if(allPixels == NULL)
	{
		printf("malloc ERROR !!!\n");
		exit(-1);
	}
	for(uint32_t i = 0; i < image->rows; i ++)
	{
		allPixels[i] = (Color*)malloc(image->cols * sizeof(Color));
		if(allPixels[i] == NULL)
		{
			printf("malloc ERROR !!!\n");
			exit(-1);
		}
	}

	Color nowPixel;

	// 遍历整个ppm文件
	for(uint32_t row = 0; row < image->rows; row ++)
	{
		for(uint32_t col = 0; col < image->cols; col ++)
		{
			fscanf(ppm, "%hhu %hhu %hhu", &nowPixel.R, &nowPixel.G, &nowPixel.B);
			allPixels[row][col] = nowPixel;
		}
	}
	
	// 遍历结果写进image
	image->image = allPixels;

	// 关闭ppm文件
	fclose(ppm);

	return image;
}

//Given an image, prints to stdout (e.g. with printf) a .ppm P3 file with the image's data.
void writeData(Image *image)
{
	// 格式前缀
	printf("P3\n");
	printf("%u %u\n", image->cols, image->rows);
	printf("255\n");

	for(uint32_t row = 0; row < image->rows; row ++)
	{
		for(uint32_t col = 0; col < image->cols; col ++)
		{
			Color nowPixel = image->image[row][col] ;
			printf("%3hhu %3hhu %3hhu", nowPixel.R, nowPixel.G, nowPixel.B);	// 宽度3，对齐

			if(col != image->cols - 1)
			{
				printf("   ");
			}
		}

		printf("\n");
	}
}

//Frees an image
void freeImage(Image *image)
{
	// 先释放所有像素，否则会变野指针
	// 逐维度释放
	Color *nowPixel;
	for(uint32_t row = 0; row < image->rows; row ++)
	{
		nowPixel = image->image[row];
		free(nowPixel);
	}	

	free(image->image);

	// 再释放image
	free(image);	
}
