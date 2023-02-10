/*****************************************************************************
 * ImageProcessing.h
 *****************************************************************************/

#ifndef __IMAGEPROCESSING_H__
#define __IMAGEPROCESSING_H__

/* Add your custom header content here */

//#define DATA_OFFSET_OFFSET 0x000A
//#define WIDTH_OFFSET 0x0012
//#define HEIGHT_OFFSET 0x0016
//#define BITS_PER_PIXEL_OFFSET 0x001C
//#define HEADER_SIZE 14
//#define INFO_HEADER_SIZE 40
//#define NO_COMPRESION 0
//#define MAX_NUMBER_OF_COLORS 0
//#define ALL_COLORS_REQUIRED 0


#define KIRSCH 1
#define PREWITT 2
#define SOBEL 3

#define TRESHOLD 30

#define WHITE 255
#define BLACK 0

typedef unsigned int int32;
typedef short int16;
typedef unsigned char byte;

/***************************
*
* Directions for the masks
* 3 2 1
* 4 x 0
* 5 6 7
*
****************************/

/* masks for kirsch operator */
int kirsch_mask_0[3][3] = {{ 5, 5, 5},
						   {-3, 0, -3},
                           {-3, -3, -3} };

int kirsch_mask_1[3][3] = {{-3, 5, 5},
                           {-3, 0, 5},
                           {-3, -3, -3} };

int kirsch_mask_2[3][3] = {{-3, -3, 5},
                           {-3, 0, 5},
                           {-3, -3, 5} };

int kirsch_mask_3[3][3] = {{-3, -3, -3},
                           {-3, 0, 5},
                           {-3, 5, 5} };

int kirsch_mask_4[3][3] = {{-3, -3, -3},
                           {-3, 0, -3},
                           { 5, 5, 5} };

int kirsch_mask_5[3][3] = {{-3, -3, -3},
                           { 5, 0, -3},
                           { 5, 5, -3} };

int kirsch_mask_6[3][3] = {{ 5, -3, -3},
                           { 5, 0, -3},
                           { 5, -3, -3} };

int kirsch_mask_7[3][3] = {{ 5, 5, -3},
                           { 5, 0, -3},
                           {-3, -3, -3} };

/* masks for prewitt operator */
int prewitt_mask_0[3][3] = {{ 1, 1, 1},
                            { 1, -2, 1},
                            {-1, -1, -1} };

int prewitt_mask_1[3][3] = {{ 1, 1, 1},
                            { 1, -2, -1},
                            { 1, -1, -1} };

int prewitt_mask_2[3][3] = {{ 1, 1, -1},
                           { 1, -2, -1},
                           { 1, 1, -1} };

int prewitt_mask_3[3][3] = {{ 1, -1, -1},
                            { 1, -2, -1},
                            { 1, 1, 1} };

int prewitt_mask_4[3][3] = {{-1, -1, -1},
                            { 1, -2, 1},
                            { 1, 1, 1} };

int prewitt_mask_5[3][3] = {{-1, -1, 1},
                            {-1, -2, 1},
                            { 1, 1, 1} };

int prewitt_mask_6[3][3] = {{-1, 1, 1},
                            {-1, -2, 1},
                            {-1, 1, 1} };

int prewitt_mask_7[3][3] = {{ 1, 1, 1},
                            {-1, -2, 1},
                            {-1, -1, 1} };

/* masks for sobel operator */
int sobel_mask_0[3][3] = {{ 1, 2, 1},
                          { 0, 0, 0},
                          {-1, -2, -1} };

int sobel_mask_1[3][3] = {{ 2, 1, 0},
                          { 1, 0, -1},
                          { 0, -1, -2} };

int sobel_mask_2[3][3] = {{ 1, 0, -1},
                          { 2, 0, -2},
                          { 1, 0, -1} };

int sobel_mask_3[3][3] = {{ 0, -1, -2},
                          { 1, 0, -1},
                          { 2, 1, 0} };

int sobel_mask_4[3][3] = {{-1, -2, -1},
                          { 0, 0, 0},
                          { 1, 2, 1} };

int sobel_mask_5[3][3] = {{-2, -1, 0},
                          {-1, 0, 1},
                          { 0, 1, 2} };

int sobel_mask_6[3][3] = {{-1, 0, 1},
                          {-2, 0, 2},
                          {-1, 0, 1} };

int sobel_mask_7[3][3] = {{ 0, 1, 2},
                          {-1, 0, 1},
                          {-2, -1, 0} };

/* mask for quick operator */
int quick_mask[3][3] = {{-1, 0, -1},
                        { 0, 4, 0},
                        {-1, 0, -1} };


/***********************************************
*
* FUNCTION DECLARATIONS
*
***********************************************/

/*
 * @brief This function reads BMP image.
 *
 * @param[in] 	fileName 	  - The name of the file to open
 * @param[in]	pixels	      - A pointer to a byte array that will contain the pixel data
 * @param[in]	width	      - An int pointer to store the width of the image in pixels
 * @param[in]	height	      - An int pointer to store the height of the image in pixels
 * @param[in]	bytesPerPixel - An int pointer to store the number of bytes per pixel that are used in the image
 *
 * @output
 * @returns 	              - None
 */
// void read_image(const char *fileName,byte **pixels, int32 *width, int32 *height, int32 *bytesPerPixel);


/*
 * @brief This function writes BMP image.
 *
 * @param[in] 	fileName 	   - The name of the file to save
 * @param[in]	pixels	       - Pointer to the pixel data array
 * @param[in]	width	       - The width of the image in pixels
 * @param[in]	height	       - The height of the image in pixels
 * @param[in]	bytesPerPixel  - The number of bytes per pixel that are used in the image
 *
 * @output
 * @returns 	               - None
 */
// void write_image(const char *fileName, byte *pixels, int32 width, int32 height,int32 bytesPerPixel);

/*
 * @brief This function copies the mask values into the mask arrays mask_0 through mask_7
 * 		  depending on the detect_type parameter.
 *
 * @param[in] 	detect_type 	- One of three different operator types (KIRSCH, PREWITT, SOBEL)
 * @param[in]	mask_0[3][3]	- Mask array for direction 0
 * @param[in]	mask_1[3][3]	- Mask array for direction 1
 * @param[in]	mask_2[3][3]	- Mask array for direction 2
 * @param[in]	mask_3[3][3]	- Mask array for direction 3
 * @param[in]	mask_4[3][3]	- Mask array for direction 4
 * @param[in]	mask_5[3][3]	- Mask array for direction 5
 * @param[in]	mask_6[3][3]	- Mask array for direction 6
 * @param[in]	mask_7[3][3]	- Mask array for direction 7
 *
 * @output
 * @returns 	                - None
 */
void setup_masks(int detect_type, int mask_0[3][3], int mask_1[3][3], int mask_2[3][3], int mask_3[3][3], int mask_4[3][3], int mask_5[3][3], int mask_6[3][3], int mask_7[3][3]);

/*
 * @brief * This function performs convolution between the input
 * 			image pixels_2d and 8 3x3 masks. The result is placed in the output image edge_detection_pixels.
 *
 * @param[in] 	detect_type  - One of three different operator types (KIRSCH, PREWITT, SOBEL)
 * @param[in]	rows	     - Number of rows of the image
 * @param[in]	cols	     - Number of columns of the image
 * @param[in]	high	     - Upper value of the treshold
 *
 * @output
 * @returns 	             - None
 */
void perform_convolution(int detect_type, int rows, int cols, int high);

/*
 * @brief * This function changes the values of all outer edge pixels in image edge_detection_pixels
 * 			from white to black ​and keeps the old value of all other pixels.
 *
 * @param[in] 	 - None
 *
 * @output
 * @returns 	 - None
 */
void fix_outer_edges(void);

/*
 * @brief * This function detects edges in an area of image and sends the result to another image.
 *          It reads the input image, calls a perform_convolution function
 *          and fix_outer_edges function.
 *
 * @param[in] 	detect_type  - One of three different operator types (KIRSCH, PREWITT, SOBEL)
 * @param[in]	rows	     - Number of rows of the image
 * @param[in]	cols	     - Number of columns of the image
 * @param[in]	high	     - Upper value of the treshold
 *
 * @output
 * @returns 	 - None
 */
void detect_edges(int detect_type, int high, int rows, int cols);

/*
 * @brief This function finds edges by using a single 3x3 quick_mask.
 *
 * @param[in]	high	    - Upper value of the treshold
 * @param[in]	rows	    - Number of rows of the image
 * @param[in]	cols	    - Number of columns of the image
 *
 * @output
 * @returns 	            - None
 */
void quick_edge(int high, int rows, int cols);

/*
 * @brief This function converts a color image to a grayscale image.
 *
 * @param[in] 	pixels	    - Array of color image pixels
 * @param[in]	width	    - Width of the image
 * @param[in]	hight	    - Height of the image
 *
 * @output
 * @returns 	            - None
 */
void grayscale(byte *pixels, int width, int height);

/*
 * @brief This function converts a grayscale image to a color image.
 *
 * @param[in] 	pixels	    - Array of grayscale image pixels
 * @param[in]	width	    - Width of the image
 * @param[in]	hight	    - Height of the image
 *
 * @output
 * @returns 	            - None
 */
void reverse_grayscale(byte *pixels, int width, int height);

/*
 * @brief This function encodes a segmented image obtained by edge detection algorithm.
 *
 * @param[in] 	 - None
 *
 * @output
 * @returns 	 - None
 */
void encode_image(void);

/*
 * @brief This function converts a one-dimensional array of pixels into a two-dimensional one.
 *
 * @param[in] 	 - None
 *
 * @output
 * @returns 	 - None
 */
void convert_1d_into_2d(void);

/*
 * @brief This function converts a two-dimensional array of pixels into a one-dimensional array.
 *
 * @param[in] 	 - None
 *
 * @output
 * @returns 	 - None
 */
void convert_2d_into_1d(void);

#endif /* __IMAGEPROCESSING_H__ */
