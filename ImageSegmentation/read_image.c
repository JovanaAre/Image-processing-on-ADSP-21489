#include <stdio.h>
#include <stdlib.h>
#include <math.h>

#define DATA_OFFSET_OFFSET 0x000A
#define WIDTH_OFFSET 0x0012
#define HEIGHT_OFFSET 0x0016
#define BITS_PER_PIXEL_OFFSET 0x001C
#define HEADER_SIZE 14
#define INFO_HEADER_SIZE 40
#define NO_COMPRESION 0
#define MAX_NUMBER_OF_COLORS 0
#define ALL_COLORS_REQUIRED 0

typedef unsigned int int32;
typedef short int16;
typedef unsigned char byte;

byte *pixels;

//***Inputs*****
//fileName: The name of the file to open 
//***Outputs****
//pixels: A pointer to a byte array. This will contain the pixel data
//width: An int pointer to store the width of the image in pixels
//height: An int pointer to store the height of the image in pixels
//bytesPerPixel: An int pointer to store the number of bytes per pixel that are used in the image
void read_image(const char *fileName,byte **pixels, int32 *width, int32 *height, int32 *bytesPerPixel)
{
        //Open the file for reading in binary mode
        FILE *imageFile = fopen(fileName, "rb");
        //Read data offset
        int32 dataOffset;
        fseek(imageFile, DATA_OFFSET_OFFSET, SEEK_SET);
        fread(&dataOffset, 4, 1, imageFile);
        //Read width
        fseek(imageFile, WIDTH_OFFSET, SEEK_SET);
        fread(width, 4, 1, imageFile);
        //Read height
        fseek(imageFile, HEIGHT_OFFSET, SEEK_SET);
        fread(height, 4, 1, imageFile);
        //Read bits per pixel
        int16 bitsPerPixel;
        fseek(imageFile, BITS_PER_PIXEL_OFFSET, SEEK_SET);
        fread(&bitsPerPixel, 2, 1, imageFile);
        //Allocate a pixel array
        *bytesPerPixel = ((int32)bitsPerPixel) / 8;

        //Rows are stored bottom-up
        //Each row is padded to be a multiple of 4 bytes. 
        //We calculate the padded row size in bytes
        int paddedRowSize = (int)(4 * ceil((float)(*width) / 4.0f))*(*bytesPerPixel);
        //We are not interested in the padded bytes, so we allocate memory just for
        //the pixel data
        int unpaddedRowSize = (*width)*(*bytesPerPixel);
        //Total size of the pixel data in bytes
        int totalSize = unpaddedRowSize*(*height);
        *pixels = (byte*)malloc(totalSize);
        //Read the pixel data Row by Row.
        //Data is padded and stored bottom-up
        int i = 0;
        //point to the last row of our pixel array (unpadded)
        byte *currentRowPointer = *pixels+((*height-1)*unpaddedRowSize);
        for (i = 0; i < *height; i++)
        {
                //put file cursor in the next row from top to bottom
	        fseek(imageFile, dataOffset+(i*paddedRowSize), SEEK_SET);
	        //read only unpaddedRowSize bytes (we can ignore the padding bytes)
	        fread(currentRowPointer, 1, unpaddedRowSize, imageFile);
	        //point to the next row (from bottom to top)
	        currentRowPointer -= unpaddedRowSize;
        }

        fclose(imageFile);
}

int main()
{
	int32 width;
    int32 height;
    int32 bytesPerPixel;
	
	int i;
	
	FILE *fp1;
	FILE *fp2;
	
	// Read first image
	read_image("test_img1.bmp", &pixels, &width, &height,&bytesPerPixel);
	
	
    if((fp1=fopen("test_img1.h", "w"))==NULL) {
 	   printf("Cannot open file.\n");
	   return 1;
 	}
	
	// Place the first image in the first header file
	fprintf(fp1, "#pragma section(\"seg_hp2\")\n");
	fprintf(fp1, "unsigned char pix[] = {");

	for (int i = 0; i < width*height*bytesPerPixel; i++) {
		fprintf(fp1, "%u", pixels[i]);
	    if (i < width*height*bytesPerPixel - 1) {
	        fprintf(fp1, ", ");
	    }
		else
	       fprintf(fp1, "};\n");
    }  
	
	// Read second image
	read_image("test_img2.bmp", &pixels, &width, &height,&bytesPerPixel);
	
	
    if((fp2=fopen("test_img2.h", "w"))==NULL) {
 	   printf("Cannot open file.\n");
	   return 1;
 	}
	
	// Place the second image in the second header file
	fprintf(fp2, "#pragma section(\"seg_hp2\")\n");
	fprintf(fp2, "unsigned char pix[] = {");

	for (int i = 0; i < width*height*bytesPerPixel; i++) {
		fprintf(fp2, "%u", pixels[i]);
	    if (i < width*height*bytesPerPixel - 1) {
	        fprintf(fp2, ", ");
	    }
		else
	       fprintf(fp2, "};\n");
    }

	fclose(fp1);
	fclose(fp2);
	
	return 0;
}