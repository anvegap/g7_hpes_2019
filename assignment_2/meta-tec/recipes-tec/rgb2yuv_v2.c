#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <ctype.h>
#include <time.h>
#include <string.h>

void rgbToYuv(unsigned char* RGB_buffer , unsigned char* YUV_buffer, int size)
{
    unsigned char Y_buffer[(size/3)];
    unsigned char U_buffer[(size/3)];
    unsigned char V_buffer[(size/3)];
      
    int k = 0;
    unsigned char rVal;
    unsigned char gVal;
    unsigned char bVal;
            
	for(int j = 0; j <= size-3; j=j+3)
	{  
        if(j==0)
        {
            for(int h=0; h<3; h++)
                printf("INICIO FOR: %i\n",RGB_buffer[h]);
        }
        
        //floating
		//YUV_buffer[j]   = 0.299  * RGB_buffer[j+0] + 0.587 * RGB_buffer[j+1] + 0.114 * RGB_buffer[j+2];	    //0.299 * rValue + 0.587 * gValue + 0.114 * bValue;
		//YUV_buffer[j+1] = -0.169 * RGB_buffer[j+0] - 0.331 * RGB_buffer[j+1] + 0.5   * RGB_buffer[j+2] + 128; //-0.147 * rValue - 0.289 * gValue + 0.436 * bValue; 
		//YUV_buffer[j+2] = 0.5    * RGB_buffer[j+0] - 0.419 * RGB_buffer[j+1] - 0.081 * RGB_buffer[j+2] + 128;  //0.615 * rValue + 0.515 * gValue - 0.100 * bValue;
        
        YUV_buffer[j]   = (unsigned char)(0.299  * RGB_buffer[j+0] + 0.587 * RGB_buffer[j+1] + 0.114 * RGB_buffer[j+2]);	    //0.299 * rValue + 0.587 * gValue + 0.114 * bValue;
		YUV_buffer[j+1] = (unsigned char)(-0.1473 * RGB_buffer[j+0] - 0.28886 * RGB_buffer[j+1] + 0.436   * RGB_buffer[j+2] + 128); //-0.147 * rValue - 0.289 * gValue + 0.436 * bValue; 
		YUV_buffer[j+2] = (unsigned char)(0.615 * RGB_buffer[j+0] - 0.51499 * RGB_buffer[j+1] - 0.10001 * RGB_buffer[j+2] + 128);  //0.615 * rValue + 0.515 * gValue - 0.100 * bValue;
        
        if(j==0)
        {
            for(int h=0; h<3; h++)
                printf("DESPUES CONVERSION: %i\n",RGB_buffer[h]);
        }
        
        //fixed
        //YUV_buffer[j+0]   = ((66 * RGB_buffer[j+0] + 129 * RGB_buffer[j+1] + 25 * RGB_buffer[j+2] + 128) >> 8) + 16;  //0.299 * rValue + 0.587 * gValue + 0.114 * bValue;
		//YUV_buffer[j+1] = ((-38 * RGB_buffer[j+0] - 74 * RGB_buffer[j+1] + 112 * RGB_buffer[j+2] + 128) >> 8) + 128;  //-0.147 * rValue - 0.289 * gValue + 0.436 * bValue; 
		//YUV_buffer[j+2] = ((112 * RGB_buffer[j+0] - 94 * RGB_buffer[j+1] - 18 * RGB_buffer[j+2] + 128) >> 8) + 128;   //0.615 * rValue + 0.515 * gValue - 0.100 * bValue;
        
        //printf("\nR: %i, G: %i, B: %i -------- Y: %i, U: %i, V: %i", RGB_buffer[j], RGB_buffer[j+1], RGB_buffer[j+2], Y_buffer[k], U_buffer[k], V_buffer[k]);
        //printf("\nR: %i, G: %i, B: %i -------- Y: %i, U: %i, V: %i", RGB_buffer[j], RGB_buffer[j+1], RGB_buffer[j+2], YUV_buffer[j], YUV_buffer[j+1], YUV_buffer[j+2]);
        
        Y_buffer[k] = YUV_buffer[j];
        U_buffer[k] = YUV_buffer[j+1];
        V_buffer[k] = YUV_buffer[j+2];
        
        //printf("\n------------------------------------------valor de K: %i", k);
        //fflush(stdout);
        if(j==0)
        {
            printf("R: %i, G: %i, B: %i -------- ", RGB_buffer[j], RGB_buffer[j+1], RGB_buffer[j+2]);
            printf("Y: %i, U: %i, V: %i\n", Y_buffer[k], U_buffer[k], V_buffer[k]);
        }
     
        k++;
	}
	
	unsigned char* out = malloc(size*sizeof(unsigned char));
    
	memcpy(out, Y_buffer, (size/3)*sizeof(unsigned char));
    memcpy(out + (size/3), U_buffer, (size/3)*sizeof(unsigned char));
    memcpy(out + (2*size/3), V_buffer, (size/3)*sizeof(unsigned char));
    
    YUV_buffer = out;
    //printf("Valor de k: %d", k);
}

void printChar(char *fileToPrint)
{
	FILE * filename;
	unsigned char character;

	filename = fopen(fileToPrint, "r");
	if (filename == NULL)
	{
		printf("Error reading file\n\n");
		exit(1);
	}

	while((character = fgetc(filename))!= EOF)
	{
		printf("%c", character);
	}
	fclose(filename);
}

void rgb2yub(char *input_image, char *output_image)
{
	FILE * input_fp;
	FILE * output_fp;
	char character;
	double timeOfExecution = 0;
	int charCounter=0;
	int pixelCounter=0;
	
	printf("Archivo RGB a convertir: %s \n", input_image);
	clock_t beginExecution = clock(); 		// **Reubicar esta linea para medir bien el tiempo
	
	input_fp = fopen(input_image, "r");
	if (input_fp == NULL)
	{
		printf("Error en apertura de archivo\n\n");
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
            if(charCounter <3)
                printf("%i\n",(unsigned char)character);
            charCounter++;
        }
            
        unsigned char YUV_buffer[pixelCounter];
		
        rgbToYuv(RGB_buffer, YUV_buffer, charCounter);

        output_fp = fopen(output_image, "w");
		if (output_fp == NULL)
		{
			printf("Error en crear archivo de salida\n\n");
			exit(1);
		}

		fputs(YUV_buffer, output_fp);
	}


	printf("\n\n\nFinished convertion \n");
	fclose(input_fp);
	fclose(output_fp);


	clock_t endExecution = clock();
	timeOfExecution = (double)(endExecution - beginExecution)/CLOCKS_PER_SEC;
	printf("\n\nrgb2yub execution time is: %f Seconds\n\n", timeOfExecution);
	
}

int main(int argc, char **argv )
{
	char *ivalue = "initial_input";
	char *ovalue = "initial_output";
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
				{	fprintf(stderr, "Option -%c requires argument \n", optopt);
					oflag=0;}
				else if(optopt=='i')
				{	fprintf(stderr, "Option -%c requires argument \n", optopt);
					iflag=0;}
				else if (isprint(optopt))
					fprintf(stderr, "unknown option `-%c \n", optopt);
				else
					fprintf(stderr, "unknown Character `\\x%x'.\n", optopt);
				return 1;
			default:
				printf("unknown case \n");
				break;
		}
	}

	if(hflag)
	{
	printf(" Usage:\n ./rgb2yub -i <RGB file name> -o <YUV file name>\n");
	}
	if(iflag * oflag)
	{
		rgb2yub(ivalue, ovalue);
	}
	else
	{
		printf("Missing Paramenter \n");
		printf(" Usage:\n ./rgb2yub -i <RGB file name> -o <YUV file name>\n");
	}
	if(aflag)
	{
	printf(" Autors:\n Jose Pablo Vernava \n Albert Hernandez \n Natalia Rodriguez \n Anthony Vega\n");
	}
	return 0;
}
