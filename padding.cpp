/*#include<stdio.h>
#include<stdlib.h>
#include<stdint.h>*/

void PKCS7_padding(uint8_t text[][4], int shortageNum) {

	int buffer = shortageNum;
	text[3][3] = shortageNum;
	for(int i=3; i>=0; i--) {
		for(int j=3; j>=0; j--) {
			text[j][i] = shortageNum;
			buffer--;
			if(buffer == 0) break;
		}//for
		if(buffer == 0) break;
	}//for
	/*for(int i=0; i<4; i++) {
		for(int j=0; j<4; j++)
			printf("%x ", text[i][j]);
		printf("\n");
	}*/
					
}//padding

/*
int main(void){
	uint8_t text[10]={0x10,0x20,0x30,0x40};	
	uint8_t plainText[4][4] = 
	{
 		{0x01, 0x89, 0xFE, },
 		{0x23, 0xAB, 0xDC, },
 		{0x45, 0xCD,  },
 		{0x23, 0xAB,  }
	};//plaintext
	for(int i=0;i<4;i++){
		for(int j=0;j<4;j++){
			printf("%x ",plainText[i][j]);
		}
		printf("\n");
	}
	printf("\n----------------------\n");
	int count=0;
	int check=1;
	int index_i=0;
	int index_j=0;
	while(count!=16){
		index_i=count%4;
		index_j=count/4;
		if(plainText[index_i][index_j]==NULL&&check){
			check=0;
			plainText[index_i][index_j]=1;
		}else if(plainText[index_i][index_j]==NULL){
			plainText[index_i][index_j]=0;
		}
		count++;
	}
	for(int i=0;i<4;i++){
		for(int j=0;j<4;j++){
			printf("%x ",plainText[i][j]);
		}
		printf("\n");
	}
}*/
