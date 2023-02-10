#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include "ImageProcessing/Debug/edge_detected_pixels.h"
#include "ImageProcessing/Debug/colored_pixels.h"

#define DATA_OFFSET_OFFSET 0x000A
#define WIDTH_OFFSET 0x0012
#define HEIGHT_OFFSET 0x0016
#define BITS_PER_PIXEL_OFFSET 0x001C
#define HEADER_SIZE 14
#define INFO_HEADER_SIZE 40
#define NO_COMPRESION 0
#define MAX_NUMBER_OF_COLORS 0
#define ALL_COLORS_REQUIRED 0

#define BYTESPERPIXEL 3

typedef unsigned int int32;
typedef short int16;
typedef unsigned char byte;

byte pixels_1d[WIDTH*HEIGHT*BYTESPERPIXEL];


//***Inputs*****
//fileName: The name of the file to save 
//pixels: Pointer to the pixel data array
//width: The width of the image in pixels
//height: The height of the image in pixels
//bytesPerPixel: The number of bytes per pixel that are used in the image
void write_image(const char *fileName, byte *pixels, int32 width, int32 height,int32 bytesPerPixel)
{
        //Open file in binary mode
        FILE *outputFile = fopen(fileName, "wb");
        //*****HEADER************//
        //write signature
        const char *BM = "BM";
        fwrite(&BM[0], 1, 1, outputFile);
        fwrite(&BM[1], 1, 1, outputFile);
        //Write file size considering padded bytes
        int paddedRowSize = (int)(4 * ceil((float)width/4.0f))*bytesPerPixel;
        int32 fileSize = paddedRowSize*height + HEADER_SIZE + INFO_HEADER_SIZE;
        fwrite(&fileSize, 4, 1, outputFile);
        //Write reserved
        int32 reserved = 0x0000;
        fwrite(&reserved, 4, 1, outputFile);
        //Write data offset
        int32 dataOffset = HEADER_SIZE+INFO_HEADER_SIZE;
        fwrite(&dataOffset, 4, 1, outputFile);

        //*******INFO*HEADER******//
        //Write size
        int32 infoHeaderSize = INFO_HEADER_SIZE;
        fwrite(&infoHeaderSize, 4, 1, outputFile);
        //Write width and height
        fwrite(&width, 4, 1, outputFile);
        fwrite(&height, 4, 1, outputFile);
        //Write planes
        int16 planes = 1; //always 1
        fwrite(&planes, 2, 1, outputFile);
        //write bits per pixel
        int16 bitsPerPixel = bytesPerPixel * 8;
        fwrite(&bitsPerPixel, 2, 1, outputFile);
        //write compression
        int32 compression = NO_COMPRESION;
        fwrite(&compression, 4, 1, outputFile);
        //write image size (in bytes)
        int32 imageSize = width*height*bytesPerPixel;
        fwrite(&imageSize, 4, 1, outputFile);
        //write resolution (in pixels per meter)
        int32 resolutionX = 11811; //300 dpi
        int32 resolutionY = 11811; //300 dpi
        fwrite(&resolutionX, 4, 1, outputFile);
        fwrite(&resolutionY, 4, 1, outputFile);
        //write colors used 
        int32 colorsUsed = MAX_NUMBER_OF_COLORS;
        fwrite(&colorsUsed, 4, 1, outputFile);
        //Write important colors
        int32 importantColors = ALL_COLORS_REQUIRED;
        fwrite(&importantColors, 4, 1, outputFile);
        //write data
        int i = 0;
        int unpaddedRowSize = width*bytesPerPixel;
        for ( i = 0; i < height; i++)
        {
                //start writing from the beginning of last row in the pixel array
                int pixelOffset = ((height - i) - 1)*unpaddedRowSize;
                fwrite(&pixels[pixelOffset], 1, paddedRowSize, outputFile);	
        }
        fclose(outputFile);
}

void convert_2d_into_1d(void)
{
    int i,j;
    // Convert 2D matrix of pixels into 1D array
	for(int i = 0; i < HEIGHT; i++) {
        for (int j = 0; j < WIDTH; j++) {
           int index = i*WIDTH*BYTESPERPIXEL + j*BYTESPERPIXEL;
           pixels_1d[index] = edge_detection_pixels[i][j];
           pixels_1d[index+1] = edge_detection_pixels[i][j];
           pixels_1d[index+2] = edge_detection_pixels[i][j];
        }
    }
}

int main()
{
    convert_2d_into_1d();
    write_image("edge_detected_img.bmp", pixels_1d, WIDTH, HEIGHT, BYTESPERPIXEL);
    write_image("colored_img.bmp", colored_pixels, WIDTH, HEIGHT, BYTESPERPIXEL);

    return 0;
}