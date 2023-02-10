/*****************************************************************************
 * ImageProcessing.c
 *****************************************************************************/

#include <sys/platform.h>
#include <def21489.h>
#include <sru21489.h>
#include <SYSREG.h>
#include "adi_initialize.h"
#include "ImageProcessing.h"

#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <stdint.h>
#include <cycle_count.h>
#include <builtins.h>

#pragma optimize_for_speed

#define BYTESPERPIXEL 3

// For compiling
//#define IMAGE_1
#define IMAGE_2
#define EIGHT_DIR_EDGE
//#define QUICK_EDGE

#ifdef IMAGE_1
#include "test_img1.h"
#define WIDTH 200
#define HEIGHT 200
#endif

#ifdef IMAGE_2
#include "test_img2.h"
#define WIDTH 100
#define HEIGHT 100
#endif

#pragma section("seg_hp2")
byte pixels_2d[WIDTH][HEIGHT];
#pragma section("seg_hp2")
byte edge_detection_pixels[WIDTH][HEIGHT];
#pragma section("seg_hp2")
byte edge_detection_pixels_tmp[WIDTH][HEIGHT];
#pragma section("seg_hp2")
byte pixels_1d[WIDTH*HEIGHT*BYTESPERPIXEL];
#pragma section("seg_hp2")
byte pixels[WIDTH*HEIGHT*BYTESPERPIXEL];


cycle_t start_count;
cycle_t final_count;


void delay_cycles(unsigned int delayCount)
{
	/* delayCount = 1 => 38ns delay */
	while(delayCount--);
}

void init_SRU(void)
{
	//** LED01**//
	SRU(HIGH,DPI_PBEN06_I);
	SRU(FLAG4_O,DPI_PB06_I);
	//** LED02**//
	SRU(HIGH,DPI_PBEN13_I);
	SRU(FLAG5_O,DPI_PB13_I);
	//** LED03**//
	SRU(HIGH,DPI_PBEN14_I);
	SRU(FLAG6_O,DPI_PB14_I);
	//** LED04**//
	SRU(HIGH,DAI_PBEN03_I);
	SRU(HIGH,DAI_PB03_I);
	//** LED05**//
	SRU(HIGH,DAI_PBEN04_I);
	SRU(HIGH,DAI_PB04_I);
	//** LED06**//
	SRU(HIGH,DAI_PBEN15_I);
	SRU(HIGH,DAI_PB15_I);
	//** LED07**//
	SRU(HIGH,DAI_PBEN16_I);
	SRU(HIGH,DAI_PB16_I);
	//** LED08**//
	SRU(HIGH,DAI_PBEN17_I);
	SRU(HIGH,DAI_PB17_I);
	//Setting flag pins as outputs
	sysreg_bit_set(sysreg_FLAGS, (FLG4O|FLG5O|FLG6O) );
	//Setting HIGH to flag pins
	sysreg_bit_set(sysreg_FLAGS, (FLG4|FLG5|FLG6) );
}

//void read_image(const char *fileName,byte **pixels, int32 *width, int32 *height, int32 *bytesPerPixel)
//{
//        //Open the file for reading in binary mode
//        FILE *imageFile = fopen(fileName, "rb");
//        //Read data offset
//        int32 dataOffset;
//        fseek(imageFile, DATA_OFFSET_OFFSET, SEEK_SET);
//        fread(&dataOffset, 4, 1, imageFile);
//        //Read width
//        fseek(imageFile, WIDTH_OFFSET, SEEK_SET);
//        fread(width, 4, 1, imageFile);
//        //Read height
//        fseek(imageFile, HEIGHT_OFFSET, SEEK_SET);
//        fread(height, 4, 1, imageFile);
//        //Read bits per pixel
//        int16 bitsPerPixel;
//        fseek(imageFile, BITS_PER_PIXEL_OFFSET, SEEK_SET);
//        fread(&bitsPerPixel, 2, 1, imageFile);
//        //Allocate a pixel array
//        *bytesPerPixel = ((int32)bitsPerPixel) / 8;
//
//        //Rows are stored bottom-up
//        //Each row is padded to be a multiple of 4 bytes.
//        //We calculate the padded row size in bytes
//        int paddedRowSize = (int)(4 * ceil((float)(*width) / 4.0f))*(*bytesPerPixel);
//        //We are not interested in the padded bytes, so we allocate memory just for
//        //the pixel data
//        int unpaddedRowSize = (*width)*(*bytesPerPixel);
//        //Total size of the pixel data in bytes
//        int totalSize = unpaddedRowSize*(*height);
//        *pixels = (byte*)malloc(totalSize);
//        //Read the pixel data Row by Row.
//        //Data is padded and stored bottom-up
//        int i = 0;
//        //point to the last row of our pixel array (unpadded)
//        byte *currentRowPointer = *pixels+((*height-1)*unpaddedRowSize);
//        for (i = 0; i < *height; i++)
//        {
//          //put file cursor in the next row from top to bottom
//	        fseek(imageFile, dataOffset+(i*paddedRowSize), SEEK_SET);
//	        //read only unpaddedRowSize bytes (we can ignore the padding bytes)
//	        fread(currentRowPointer, 1, unpaddedRowSize, imageFile);
//	        //point to the next row (from bottom to top)
//	        currentRowPointer -= unpaddedRowSize;
//        }
//
//        fclose(imageFile);
//}

//void write_image(const char *fileName, byte *pixels, int32 width, int32 height,int32 bytesPerPixel)
//{
//        //Open file in binary mode
//        FILE *outputFile = fopen(fileName, "wb");
//        //*****HEADER************//
//        //write signature
//        const char *BM = "BM";
//        fwrite(&BM[0], 1, 1, outputFile);
//        fwrite(&BM[1], 1, 1, outputFile);
//        //Write file size considering padded bytes
//        int paddedRowSize = (int)(4 * ceil((float)width/4.0f))*bytesPerPixel;
//        int32 fileSize = paddedRowSize*height + HEADER_SIZE + INFO_HEADER_SIZE;
//        fwrite(&fileSize, 4, 1, outputFile);
//        //Write reserved
//        int32 reserved = 0x0000;
//        fwrite(&reserved, 4, 1, outputFile);
//        //Write data offset
//        int32 dataOffset = HEADER_SIZE+INFO_HEADER_SIZE;
//        fwrite(&dataOffset, 4, 1, outputFile);
//
//        //*******INFO*HEADER******//
//        //Write size
//        int32 infoHeaderSize = INFO_HEADER_SIZE;
//        fwrite(&infoHeaderSize, 4, 1, outputFile);
//        //Write width and height
//        fwrite(&width, 4, 1, outputFile);
//        fwrite(&height, 4, 1, outputFile);
//        //Write planes
//        int16 planes = 1; //always 1
//        fwrite(&planes, 2, 1, outputFile);
//        //write bits per pixel
//        int16 bitsPerPixel = bytesPerPixel * 8;
//        fwrite(&bitsPerPixel, 2, 1, outputFile);
//        //write compression
//        int32 compression = NO_COMPRESION;
//        fwrite(&compression, 4, 1, outputFile);
//        //write image size (in bytes)
//        int32 imageSize = width*height*bytesPerPixel;
//        fwrite(&imageSize, 4, 1, outputFile);
//        //write resolution (in pixels per meter)
//        int32 resolutionX = 11811; //300 dpi
//        int32 resolutionY = 11811; //300 dpi
//        fwrite(&resolutionX, 4, 1, outputFile);
//        fwrite(&resolutionY, 4, 1, outputFile);
//        //write colors used
//        int32 colorsUsed = MAX_NUMBER_OF_COLORS;
//        fwrite(&colorsUsed, 4, 1, outputFile);
//        //Write important colors
//        int32 importantColors = ALL_COLORS_REQUIRED;
//        fwrite(&importantColors, 4, 1, outputFile);
//        //write data
//        int i = 0;
//        int unpaddedRowSize = width*bytesPerPixel;
//        for ( i = 0; i < height; i++)
//        {
//           //start writing from the beginning of last row in the pixel array
//           int pixelOffset = ((height - i) - 1)*unpaddedRowSize;
//           fwrite(&pixels[pixelOffset], 1, paddedRowSize, outputFile);
//        }
//        fclose(outputFile);
//}

void setup_masks(int detect_type, int mask_0[3][3], int mask_1[3][3], int mask_2[3][3], int mask_3[3][3], int mask_4[3][3], int mask_5[3][3], int mask_6[3][3], int mask_7[3][3])
{
    int i, j;
    if(detect_type == 1){
        for(i=0; i<3; i++){
            for(j=0; j<3; j++){
                mask_0[i][j] = kirsch_mask_0[i][j];
                mask_1[i][j] = kirsch_mask_1[i][j];
                mask_2[i][j] = kirsch_mask_2[i][j];
                mask_3[i][j] = kirsch_mask_3[i][j];
                mask_4[i][j] = kirsch_mask_4[i][j];
                mask_5[i][j] = kirsch_mask_5[i][j];
                mask_6[i][j] = kirsch_mask_6[i][j];
                mask_7[i][j] = kirsch_mask_7[i][j];
            }
        }
    } /* ends if detect_type == KIRSCH */

    if(detect_type == 2){
        for(i=0; i<3; i++){
            for(j=0; j<3; j++){
                mask_0[i][j] = prewitt_mask_0[i][j];
                mask_1[i][j] = prewitt_mask_1[i][j];
                mask_2[i][j] = prewitt_mask_2[i][j];
                mask_3[i][j] = prewitt_mask_3[i][j];
                mask_4[i][j] = prewitt_mask_4[i][j];
                mask_5[i][j] = prewitt_mask_5[i][j];
                mask_6[i][j] = prewitt_mask_6[i][j];
                mask_7[i][j] = prewitt_mask_7[i][j];
            }
        }
    } /* ends if detect_type == PREWITT */

    if(detect_type == 3){
        for(i=0; i<3; i++){
            for(j=0; j<3; j++){
                mask_0[i][j] = sobel_mask_0[i][j];
                mask_1[i][j] = sobel_mask_1[i][j];
                mask_2[i][j] = sobel_mask_2[i][j];
                mask_3[i][j] = sobel_mask_3[i][j];
                mask_4[i][j] = sobel_mask_4[i][j];
                mask_5[i][j] = sobel_mask_5[i][j];
                mask_6[i][j] = sobel_mask_6[i][j];
                mask_7[i][j] = sobel_mask_7[i][j];
                //printf("%d ", mask_0[i][j]);
            }
        }
    } /* ends if detect_type == SOBEL */
} /* ends setup_masks */

void perform_convolution(int detect_type, int rows, int cols, int high)
{
    int a, b, i, is_present, j, sum;
    int mask_0[3][3], mask_1[3][3], mask_2[3][3], mask_3[3][3], mask_4[3][3], mask_5[3][3], mask_6[3][3], mask_7[3][3];
    byte max, min;

    setup_masks(detect_type, mask_0, mask_1, mask_2, mask_3, mask_4, mask_5, mask_6, mask_7);

    min = BLACK;
    max = WHITE;

    /* clear output image array */
    for(i=0; i<rows; i++)
        for(j=0; j<cols; j++)
            edge_detection_pixels[i][j] = 0;

	//#pragma SIMD_for
	#pragma no_vectorization
    for(i=1; i<rows-1; i++){
        for(j=1; j<cols-1; j++){

            /* Convolve for all 8 directions */

            /* 0 direction */
            sum = 0;
            for(a=-1; a<2; a++){
                for(b=-1; b<2; b++){
                sum = sum + pixels_2d[i+a][j+b] * mask_0[a+1][b+1];
                }
            }

            if(sum > max) sum = max;
            if(sum < 0) sum = 0;

            if(sum > edge_detection_pixels[i][j])
                edge_detection_pixels[i][j] = sum;

            /* 1 direction */
            sum = 0;
            for(a=-1; a<2; a++){
                for(b=-1; b<2; b++){
                    sum = sum + pixels_2d[i+a][j+b] * mask_1[a+1][b+1];
                }
            }
            //printf("%d ",sum);
            if(sum > max) sum = max;
            if(sum < 0) sum = 0;

            if(sum > edge_detection_pixels[i][j])
                edge_detection_pixels[i][j] = sum;

            /* 2 direction */
            sum = 0;
            for(a=-1; a<2; a++){
                for(b=-1; b<2; b++){
                    sum = sum + pixels_2d[i+a][j+b] * mask_2[a+1][b+1];
                }
            }
            if(sum > max) sum = max;
            if(sum < 0) sum = 0;

            if(sum > edge_detection_pixels[i][j])
                edge_detection_pixels[i][j] = sum;

            /* 3 direction */
            sum = 0;
            for(a=-1; a<2; a++){
                for(b=-1; b<2; b++){
                    sum = sum + pixels_2d[i+a][j+b] * mask_3[a+1][b+1];
                }
            }
            if(sum > max) sum = max;
            if(sum < 0) sum = 0;

            if(sum > edge_detection_pixels[i][j])
                edge_detection_pixels[i][j] = sum;

            /* 4 direction */
            sum = 0;
            for(a=-1; a<2; a++){
                for(b=-1; b<2; b++){
                    sum = sum + pixels_2d[i+a][j+b] * mask_4[a+1][b+1];
                }
            }
            if(sum > max) sum = max;
            if(sum < 0) sum = 0;

            if(sum > edge_detection_pixels[i][j])
                edge_detection_pixels[i][j] = sum;

            /* 5 direction */
            sum = 0;
            for(a=-1; a<2; a++){
                for(b=-1; b<2; b++){
                    sum = sum + pixels_2d[i+a][j+b] * mask_5[a+1][b+1];
                }
            }
            if(sum > max) sum = max;
            if(sum < 0) sum = 0;

            if(sum > edge_detection_pixels[i][j])
                edge_detection_pixels[i][j] = sum;

            // /* 6 direction */
            sum = 0;
            for(a=-1; a<2; a++){
                for(b=-1; b<2; b++){
                    sum = sum + pixels_2d[i+a][j+b] * mask_6[a+1][b+1];
                }
            }
            if(sum > max) sum = max;
            if(sum < 0) sum = 0;

            if(sum > edge_detection_pixels[i][j])
                edge_detection_pixels[i][j] = sum;

            /* 7 direction */
            sum = 0;
            for(a=-1; a<2; a++){
                for(b=-1; b<2; b++){
                    sum = sum + pixels_2d[i+a][j+b] * mask_7[a+1][b+1];
                }
            }
            if(sum > max) sum = max;
            if(sum < 0) sum = 0;

            if(sum > edge_detection_pixels[i][j])
                edge_detection_pixels[i][j] = sum;
        } /* ends loop over j */
    } /* ends loop over i */

/* threshold the output image */
        for(i=0; i<rows; i++){
            for(j=0; j<cols; j++){
                if(edge_detection_pixels[i][j] > high){
                    edge_detection_pixels[i][j] = BLACK;
                }
                else{
                    edge_detection_pixels[i][j] = WHITE;
                }
            }
        }
} /* ends perform_convolution */

void fix_outer_edges(void)
{
	int i,j;
	for (i = 0; i < HEIGHT; i++) {
	   for (j = 0; j < WIDTH; j++) {
	      edge_detection_pixels_tmp[i][j]= edge_detection_pixels[i][j];
	    }
	}

	for (i = 0; i < HEIGHT; i++) {
	   for (j = 0; j < WIDTH; j++) {
	      if( i==0 || i==(HEIGHT-1) || j==0 || j==(WIDTH-1))
	         edge_detection_pixels[i][j]= BLACK;
	      else
	         edge_detection_pixels[i][j]= edge_detection_pixels_tmp[i][j];
	     }
	}
}

void detect_edges(int detect_type, int high, int rows, int cols)
{
    perform_convolution(detect_type, rows, cols, high);
    fix_outer_edges();

} /* ends detect_edges */

void quick_edge(int high, int rows, int cols)
{
    int a, b, i, j, k, length, max, sum, width;

    max = WHITE;

	//#pragma SIMD_for
	#pragma no_vectorization
    /* Do convolution over image array */
    for(i=1; i<rows-1; i++){
        for(j=1; j<cols-1; j++){
            sum = 0;
            for(a=-1; a<2; a++){
                for(b=-1; b<2; b++){
                    sum = sum + pixels_2d[i+a][j+b] *quick_mask[a+1][b+1];
                }
            }
            //printf("%d ",sum);
            if(sum < 0) sum = 0;
            if(sum > max) sum = max;
            if(sum > edge_detection_pixels[i][j])
                edge_detection_pixels[i][j] = sum;
               // edge_detection_pixels[i][j] = sum;
        } /* ends loop over j */
    } /* ends loop over i */

    /* threshold the output image */
        for(i=0; i<rows; i++){
            for(j=0; j<cols; j++){
                if(edge_detection_pixels[i][j] > high){
                    edge_detection_pixels[i][j] = BLACK;
                }
                else{
                    edge_detection_pixels[i][j] = WHITE;
                }
            }
        }
    fix_outer_edges();
} /* ends quick_edge */

void grayscale(byte *pixels, int width, int height) {

    int j=0;
	#pragma no_vectorization
    for (int i = 0; i < width * height; i++) {
        int gray = (pixels[i * 3]*0.3 + pixels[i * 3 + 1]*0.59 + pixels[i * 3 + 2]*0.11);
        pixels[i * 3] = gray;
        pixels[i * 3 + 1] = gray;
        pixels[i * 3 + 2] = gray;
    }
}

void reverse_grayscale(byte *pixels, int width, int height) {

	//#pragma SIMD_for
	//#pragma no_vectorization
    for (int i = 0; i < width * height; i++) {
    	int gray = pixels[i * 3];
    	int red = gray / 0.3;
    	int green = gray / 0.59;
    	int blue = gray / 0.11;

    	if(red<255)
    		pixels[i * 3] = red;
    	else
    		pixels[i * 3] = red%256;

    	if(green<255)
    		pixels[i * 3 + 1] =  green;
    	else
    		pixels[i * 3 + 1] =  green%256;

    	if(blue<255)
    		pixels[i * 3 + 2] = blue;
    	else
    		pixels[i * 3 + 2] = blue%256;
   }
}

void encode_image(void)
{
    byte colors = 40;

	//#pragma SIMD_for
	#pragma no_vectorization
    for (int i = 0; i < HEIGHT; i++) {
       for (int j = 0; j < WIDTH; j++) {
          if(edge_detection_pixels[i][j] == BLACK)
              edge_detection_pixels[i][j] = BLACK;
          else if(edge_detection_pixels[i][j]==WHITE && edge_detection_pixels[i-1][j]==BLACK && edge_detection_pixels[i][j-1]==BLACK && (edge_detection_pixels[i-1][j+1]!=BLACK))
              edge_detection_pixels[i][j]=edge_detection_pixels[i-1][j+1];
          else if(edge_detection_pixels[i][j]==WHITE && edge_detection_pixels[i-1][j]==BLACK && edge_detection_pixels[i][j-1]==BLACK)
          {
              edge_detection_pixels[i][j] = colors;
              colors += 40;
          }
          else if(edge_detection_pixels[i][j]==WHITE && (edge_detection_pixels[i-1][j]!=BLACK))
               edge_detection_pixels[i][j]=edge_detection_pixels[i-1][j];
          else if(edge_detection_pixels[i][j]==WHITE && (edge_detection_pixels[i][j-1]!=BLACK))
               edge_detection_pixels[i][j]=edge_detection_pixels[i][j-1];
          //else if(edge_detection_pixels[i][j]==255 && edge_detection_pixels[i-1][j]==0
          //&& edge_detection_pixels[i][j-1]==0 && (edge_detection_pixels[i-1][j+1]!=0))
               //edge_detection_pixels[i][j]=edge_detection_pixels[i-1][j+1];
       }
    }
}

void convert_1d_into_2d(void)
{
	int i,j;
	// Convert 1D array of pixels into 2D matrix of pixels
	for (i = 0; i < HEIGHT; i++) {
	    for (j = 0; j < WIDTH; j++) {
	       int index = i*WIDTH*BYTESPERPIXEL + j*BYTESPERPIXEL;
	       pixels_2d[i][j]= pixels[index];
	       //printf("%u", pixels_2d[i][j]);
	    }
	       // printf("\n");
	}
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

/**
 * If you want to use command program arguments, then place them in the following string.
 */
char __argv_string[] = "";

int main(int argc, char *argv[])
{
	/**
	 * Initialize managed drivers and/or services that have been added to
	 * the project.
	 * @return zero on success
	 */
	adi_initComponents();

	init_SRU();

	//Turn off LEDs
	sysreg_bit_clr(sysreg_FLAGS, FLG4);
	sysreg_bit_clr(sysreg_FLAGS, FLG6);
	SRU(LOW,DAI_PB03_I);
	SRU(LOW,DAI_PB04_I);
	SRU(LOW,DAI_PB15_I);
	SRU(LOW,DAI_PB16_I);
	SRU(LOW,DAI_PB17_I);

	/* Begin adding your custom code here */

    int i,j;

    FILE *fp1;
    FILE *fp2;

    //read_image("test_img2.bmp", &pixels, &width, &height, &bytesPerPixel);

    delay_cycles(3500000);
    // Turn on LED01
    sysreg_bit_set(sysreg_FLAGS, FLG4);

    START_CYCLE_COUNT(start_count);
	//#pragma SIMD_for
    // Read pixels of input image
    for(i=0;i<WIDTH*HEIGHT*BYTESPERPIXEL;i++)
    {
    	pixels[i]=pix[i];
    	//printf("%u ", pixels[i]);
    }
    STOP_CYCLE_COUNT(final_count, start_count);
    PRINT_CYCLES("READING IMAGE: ", final_count);
    //printf("%d", sizeof(pixels));

    delay_cycles(3500000);
    // Turn on LED02
    sysreg_bit_set(sysreg_FLAGS, FLG5);

    START_CYCLE_COUNT(start_count);
	grayscale(pixels, WIDTH, HEIGHT);
    STOP_CYCLE_COUNT(final_count, start_count);
    PRINT_CYCLES("CONVERSION INTO GREYSCALE: ", final_count);

	//write_image("img2.bmp", pixels, width, height, bytesPerPixel);

	delay_cycles(3500000);
	// Turn on LED03
	sysreg_bit_set(sysreg_FLAGS, FLG6);

	convert_1d_into_2d();

	#ifdef QUICK_EDGE
	START_CYCLE_COUNT(start_count);
	quick_edge(TRESHOLD, HEIGHT, WIDTH);
    STOP_CYCLE_COUNT(final_count, start_count);
    PRINT_CYCLES("EDGE DETECTION: ", final_count);
	#endif

	#ifdef EIGHT_DIR_EDGE
    START_CYCLE_COUNT(start_count);
	detect_edges(KIRSCH, TRESHOLD, HEIGHT, WIDTH);
    STOP_CYCLE_COUNT(final_count, start_count);
    PRINT_CYCLES("EDGE DETECTION: ", final_count);
	#endif

	delay_cycles(3500000);
	// Turn on LED04
	SRU(HIGH,DAI_PB03_I);


	// Write segmented image into output edge_detected_pixels.h file
	fp1 = fopen("edge_detected_pixels.h", "w");

	if (fp1 == NULL)
	{
	   printf("Error opening file!\n");
	   return 1;
	}

	fprintf(fp1, "#define WIDTH %d\n", WIDTH);
	fprintf(fp1, "#define HEIGHT %d\n", HEIGHT);
	fprintf(fp1, "unsigned char edge_detection_pixels[%d][%d] = {\n", WIDTH, HEIGHT);

	START_CYCLE_COUNT(start_count);
	//#pragma SIMD_for
	//#pragma no_vectorization
	for (int i = 0; i < HEIGHT; i++){
		fprintf(fp1, "{ ");
		for(int j=0; j< WIDTH; j++){
			fprintf(fp1, "%u", edge_detection_pixels[i][j]);
			if (expected_true(j < WIDTH - 1)) {
				fprintf(fp1, ", ");
			}
			else if (i == (HEIGHT-1))
				fprintf(fp1, "}");
			else
				fprintf(fp1, "},\n");
	   }
    }
	fprintf(fp1, "};\n");
	STOP_CYCLE_COUNT(final_count, start_count);
	PRINT_CYCLES("WRITING SEGMENTED IMAGE: ", final_count);

	fclose(fp1);

	delay_cycles(3500000);
	// Turn on LED05
	SRU(HIGH,DAI_PB04_I);

	START_CYCLE_COUNT(start_count);
	encode_image();
    STOP_CYCLE_COUNT(final_count, start_count);
    PRINT_CYCLES("IMAGE ENCODING: ", final_count);

	convert_2d_into_1d();

	delay_cycles(3500000);
	// Turn on LED06
	SRU(HIGH,DAI_PB15_I);

	START_CYCLE_COUNT(start_count);
	reverse_grayscale(pixels_1d, WIDTH, HEIGHT);
    STOP_CYCLE_COUNT(final_count, start_count);
    PRINT_CYCLES("IMAGE COLORING: ", final_count);

	delay_cycles(3500000);
	// Turn on LED07
	SRU(HIGH,DAI_PB16_I);

//	for(i=0;i<WIDTH*HEIGHT*BYTESPERPIXEL;i++)
//	{
//	   printf("%u", pixels_1d[i]);
//	}

	// Write colored image into output colored_pixels.h file
	fp2 = fopen("colored_pixels.h", "w");

	if (fp2 == NULL)
	{
	    printf("Error opening file!\n");
	    return 1;
	 }

	 fprintf(fp2, "#define WIDTH %d\n", WIDTH);
	 fprintf(fp2, "#define HEIGHT %d\n", HEIGHT);
	 fprintf(fp2, "unsigned char colored_pixels[] = {");

	 START_CYCLE_COUNT(start_count);
	// #pragma SIMD_for
	//#pragma no_vectorization
	 for (int i = 0; i < WIDTH*HEIGHT*BYTESPERPIXEL; i++)
	 {
	      fprintf(fp2, "%u", pixels_1d[i]);
	      if (expected_true(i < WIDTH*HEIGHT*BYTESPERPIXEL - 1)) {
	          fprintf(fp2, ", ");
	      }
	      else
	         fprintf(fp2, "};\n");
	 }
	 STOP_CYCLE_COUNT(final_count, start_count);
	 PRINT_CYCLES("WRITING COLORED IMAGE: ", final_count);

	 fclose(fp2);

	 delay_cycles(3500000);
	 // Turn on LED08
	 SRU(HIGH,DAI_PB17_I);
	 printf("Done!\n");

	 return 0;
}

