/*
01 10
02 20
03 30
12 21
13 31
23 32
*/

/*
#include <stdio.h>
#include <stdint.h>
#include <string.h>
*/
void matrixTranspose(uint8_t matrix[][4]) {
	int temp;
	for(int i=0; i<3; i++) {
		for(int j=(i+1); j<4; j++) {
			temp = matrix[i][j];
			matrix[i][j] = matrix[j][i];
			matrix[j][i] = temp;
		}//for
	}//for
}//matrixTranspose

/*
int main() {
	uint8_t matrix[4][4] = 
    {
	    /*{0x00, 0x04, 0x08, 0x0C},
	    {0x01, 0x05, 0x09, 0x0D},
	    {0x02, 0x06, 0x0A, 0x0E},
	    {0x03, 0x07, 0x0B, 0x0F}
	    {0x0F, 0x47, 0x0C, 0xAF},
		{0x15, 0xD9, 0xB7, 0x7F},
		{0x71, 0xE8, 0xAD, 0x67},
		{0xC9, 0x59, 0xD6, 0x98}*/
		/*{0x54, 0x73, 0x20, 0x67},
		{0x68, 0x20, 0x4B, 0x20},
		{0x61, 0x6D, 0x75, 0x46},
		{0x74, 0x79, 0x6E, 0x75}*/
		//key 54 68 61 74   73 20 6d 79   20 4b 75 6e   67 20 46 75
		//key 54 68 61 74   73 20 6d 79   20 4b 75 6e   67 20 46 75   54 68 61 74   73 20 6d 79
		//key 54 68 61 74   73 20 6d 79   20 4b 75 6e   67 20 46 75   54 68 61 74   73 20 6d 79   20 4b 75 6e   67 20 46 75
	/*	{0x54, 0x4F, 0x4E, 0x20},
		{0x77, 0x6E, 0x69, 0x54},
		{0x6F, 0x65, 0x6E, 0x77},
		{0x20, 0x20, 0x65, 0x6F}*/
		//54 77 6f 20   4f 6e 65 20   4e 69 6e 65   20 54 77 6f
		//0x54 0x77 0x6f 0x20 0x4f 0x6e 0x65 0x20 0x4e 0x69 0x6e 0x65 0x20 0x54 0x77 0x6f
		//0x54 0x77 0x6f 0x20 0x4f 0x6e 0x65 0x20 0x4e 0x69 0x6e 0x65 0x20 0x54 0x77 0x6f
		
		//29 c3 50 5f 57 14 20 f6 40 22 99 b3 1a 02 d7 3a
		
		//4e 69 6e 65   4f 6e 65 20   54 77 6f 20   4e 69 6e 65
   /* };//plaintext
    
    for(int i=0; i<4; i++) {
	    for(int j=0; j<4; j++)
	        printf("0x%x ", matrix[i][j]);
	    printf("\n");
    }//for
    printf("\n");
    
	matrixTranspose(matrix);
	for(int i=0; i<4; i++) {
	    for(int j=0; j<4; j++)
	        printf("%x ", matrix[i][j]);
	    printf("\n");
    }//for
    printf("\n");
    
	matrixTranspose(matrix);
	for(int i=0; i<4; i++) {
	    for(int j=0; j<4; j++)
	        printf("0x%x ", matrix[i][j]);
	    printf("\n");
    }//for
}//main
*/
