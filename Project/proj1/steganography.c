/************************************************************************
**
** NAME:        steganography.c
**
** DESCRIPTION: CS61C Fall 2020 Project 1
**
** AUTHOR:      Dan Garcia  -  University of California at Berkeley
**              Copyright (C) Dan Garcia, 2020. All rights reserved.
**				Justin Yokota - Starter Code
**				Liu ZETONG	
**
** DATE:        2025-03-21
**
**************************************************************************/

#include <stdio.h>
#include <stdlib.h>
#include <inttypes.h>
#include "imageloader.h"

//Determines what color the cell at the given row/col should be. This should not affect Image, and should allocate space for a new Color.
Color *evaluateOnePixel(Image *image, int row, int col)
{
	Color* hideColor = (Color*)malloc(sizeof(Color));
	if(hideColor == NULL)
	{
		printf("malloc ERROR !!!\n");
		exit(-1);
	}

	hideColor->R = hideColor->G = hideColor->B = (image->image[row][col].B & 1)? 255 : 0;
	return hideColor;
}

//Given an image, creates a new image extracting the LSB of the B channel.
Image *steganography(Image *image)
{
	Image* newimage = (Image*)malloc(sizeof(Image));
	newimage->rows = image->rows;
	newimage->cols = image->cols;

	Color** allPixels = (Color**)malloc(newimage->rows * sizeof(Color*));
	for(uint32_t i = 0; i < newimage->rows; i ++)
	{
		allPixels[i] = (Color*)malloc(newimage->cols * sizeof(Color));
	}

	for(uint32_t row = 0; row < newimage->rows; row ++)
	{
		for(uint32_t col = 0; col < newimage->cols; col ++)
		{
			Color* handle = evaluateOnePixel(image, row, col);
			allPixels[row][col] = *handle;
			free(handle);	// 及时销毁，感觉这里值传递即可，传来指针的话就要解引用完就销毁。
		}
	}

	newimage->image = allPixels;
	return newimage;
}

/*
Loads a file of ppm P3 format from a file, and prints to stdout (e.g. with printf) a new image, 
where each pixel is black if the LSB of the B channel is 0, 
and white if the LSB of the B channel is 1.

argc stores the number of arguments.
argv stores a list of arguments. Here is the expected input:
argv[0] will store the name of the program (this happens automatically).
argv[1] should contain a filename, containing a file of ppm P3 format (not necessarily with .ppm file extension).
If the input is not correct, a malloc fails, or any other error occurs, you should exit with code -1.
Otherwise, you should return from main with code 0.
Make sure to free all memory before returning!
*/
int main(int argc, char **argv)
{
	Image *before, *after;
	char* filename = argv[1];

	before = readData(filename);
	after = steganography(before);
	writeData(after);
	
	freeImage(before);
	freeImage(after);
	
	return 0;
}
