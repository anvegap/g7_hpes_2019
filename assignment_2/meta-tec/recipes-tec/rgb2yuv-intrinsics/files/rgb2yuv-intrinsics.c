#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <ctype.h>
#include <time.h>
#include <string.h>

//NEON optimized libraries and Intrinsics
//#include <omxSP.h>
#include <arm_neon.h>

void ColorSpaceConvertIntrinsics(uint8_t * __restrict Y, uint8_t * __restrict U, uint8_t * __restrict V, uint8_t * __restrict Src, int n)
{
    int i;
    
    uint8x8_t rY = vdup_n_u8(66);
    uint8x8_t gY = vdup_n_u8(129);
    uint8x8_t bY = vdup_n_u8(25);
    
    uint8x8_t rU = vdup_n_u8(38);
    uint8x8_t gU = vdup_n_u8(74);
    uint8x8_t bU = vdup_n_u8(112);
    
    uint8x8_t rV = vdup_n_u8(112);
    uint8x8_t gV = vdup_n_u8(94);
    uint8x8_t bV = vdup_n_u8(18);
    
    uint8x8_t offset16 = vdup_n_u8(16);
    uint16x8_t offset128 = vdupq_n_u16(128);
    
    n/=8;
    
    for(i=0; i<n; i++)
    {
        uint16x8_t tempY; 
        uint16x8_t tempU; 
        uint16x8_t tempV;
        
        uint8x8_t resultY;
        uint16x8_t resultU;
        uint16x8_t resultV;
        
        uint8x8x3_t RGB = vld3_u8(Src);
        
        //YUV[0] = ((66 * R + 129 * G + 25 * B + 128) >> 8) + 16;
        //Y Conversion
        tempY = vmull_u8(RGB.val[0], rY);		     // 66 * R
        tempY = vmlal_u8(tempY, RGB.val[1], gY);	 // + 129 * G
        tempY = vmlal_u8(tempY, RGB.val[2], bY);     // + 25 * B
        resultY = vshrn_n_u16(tempY, 8);		     // >> 8
        resultY = vadd_u8(resultY, offset16);		 // + 16

        //YUV[1] = ((-38 * R - 74 * G + 112 * B + 128) >> 8) + 128;
        //U Conversion
        tempU = vmull_u8(RGB.val[2], bU);	          // 112 * B
        tempU = vmlsq_u16(tempU, vmovl_u8(RGB.val[0]), vmovl_u8(rU));     // - 38 * R       u16x8, u16x8, u16x8
        tempU = vmlsq_u16(tempU, vmovl_u8(RGB.val[1]), vmovl_u8(gU));     // - 74 * G
        tempU = vaddq_u16(tempU, offset128);                              // + 128 
        resultU = vmovl_u8(vshrn_n_u16(tempU, 8));                        // >> 8
        resultU = vaddq_u16(resultU, offset128);                          // + 128
        
        //YUV[2] = ((112 * R - 94 * G - 18 * B + 128) >> 8) + 128;
        //V Conversion
        tempV = vmull_u8(RGB.val[0], rV);                                 // 112 * R
        tempV = vmlsq_u16(tempV, vmovl_u8(RGB.val[1]), vmovl_u8(gV));     // - 94 * G
        tempV = vmlsq_u16(tempV, vmovl_u8(RGB.val[2]), vmovl_u8(bV));     // - 18 * B
        tempV = vaddq_u16(tempV, offset128);                              // + 128
        resultV = vmovl_u8(vshrn_n_u16(tempV, 8));                        // >> 8
        resultV = vaddq_u16(resultV, offset128);                          // + 128
        
        vst1_u8(Y, resultY);
        vst1_u8(U, vmovn_u16(resultU));
        vst1_u8(V, vmovn_u16(resultV));
        
        Src += 8*3;
        Y += 8;
        U += 8;
        V += 8;
    }
}

void rgb2yuv(char *input_image, char *output_image)
{
	FILE * input_fp;
	FILE * output_fp;
	int character;
	int charCounter=0;
	
	input_fp = fopen(input_image, "r");
	if (input_fp == NULL)
	{
		printf("ERROR: Couldn't open file %s\n\n", input_image);
		exit(1);
	}
	else
	{       
		while((character = fgetc(input_fp))!= EOF)
		{
			charCounter++;
		}

		unsigned char RGB_buffer[charCounter];
        
        rewind(input_fp);
        charCounter = 0;
        
        while((character = fgetc(input_fp))!= EOF)
        {
            RGB_buffer[charCounter] = (unsigned char)character;
            charCounter++;
        }
          
        unsigned char YUV_buffer[2*charCounter];
    
        unsigned char *YUV;
        unsigned char Y_buffer[(charCounter/3)];
        unsigned char U_buffer[(charCounter/3)];
        unsigned char V_buffer[(charCounter/3)];
        
        uint8_t * pSrc = RGB_buffer;
        uint8_t * pY =  Y_buffer;
        uint8_t * pU =  U_buffer;
        uint8_t * pV =  V_buffer;
        
        ColorSpaceConvertIntrinsics(pY, pU, pV, pSrc, charCounter/3);
    
        memcpy(YUV_buffer, Y_buffer, (charCounter/3)*sizeof(unsigned char));
        memcpy(YUV_buffer + (charCounter/3), U_buffer, (charCounter/3)*sizeof(unsigned char));
        memcpy(YUV_buffer + (2*charCounter/3), V_buffer, (charCounter/3)*sizeof(unsigned char));
        
        output_fp = fopen(output_image, "w");
        
		if (output_fp == NULL)
		{
			printf("ERROR: Couldn't create file %s\n\n", output_image);
			exit(1);
		}

		fwrite(YUV_buffer, charCounter, 1, output_fp);
	}

	fclose(input_fp);
	fclose(output_fp);
}

int main(int argc, char **argv )
{
	char *ivalue = NULL;
	char *ovalue = NULL;
	extern char *optarg;
	int iflag = 0;
    int oflag = 0;
    int aflag = 0;
    int hflag = 0;
	int c;

	while((c = getopt(argc, argv, "ahi:o:")) != -1)
	{
		switch(c)
		{
			case 'i':
				iflag = 1;
				ivalue = optarg;// optarg stores the value after -i parameter
				break;
			case 'o':
				oflag = 1;
				ovalue = optarg;// optarg stores the value after -o parameter
				break;
			case 'a':
				aflag = 1;
				break;
			case 'h':
				hflag = 1;
				break;
			case '?':
				if(optopt=='o')
				{	fprintf(stderr, "Option -%c requires argument for output file \n", optopt);
					oflag=0;}
				else if(optopt=='i')
				{	fprintf(stderr, "Option -%c requires argument for input file \n", optopt);
					iflag=0;}
				else if (isprint(optopt))
					fprintf(stderr, "unknown option `-%c \n", optopt);
				else
					fprintf(stderr, "unknown character `\\x%x'.\n", optopt);
				return 1;
			default:
				printf("unknown case \n");
				break;
		}
	}

	if(hflag)
        printf(" Usage:\n ./rgb2yuv -i <RGB file name> -o <YUV file name>\n");
	else
    {
        if(aflag)
            printf(" Authors:\n Jose Pablo Vernava \n Albert Hernandez \n Natalia Rodriguez \n Anthony Vega\n");
        else
        {
            if(iflag)
            {
                if(oflag)
                {
                    clock_t beginExecution;
                    clock_t endExecution;
                    
                    double timeOfExecution;
                    beginExecution = clock();
                    
                    rgb2yuv(ivalue, ovalue);
                    
                    endExecution = clock();
                    
                    timeOfExecution = (double)(endExecution - beginExecution)/CLOCKS_PER_SEC;
                    printf("\n\nrgb2yuv execution time is: %f Seconds\n\n", timeOfExecution);
                }
                else
                {
                    printf("Missing Paramenter \n");
                    printf(" Usage:\n ./rgb2yuv -i <RGB file name> -o <YUV file name>\n");
                }
                
            }
            else
            {
                printf("Missing Paramenter \n");
                printf(" Usage:\n ./rgb2yuv -i <RGB file name> -o <YUV file name>\n");
            }
        }
    }
    
	return 0;
}
