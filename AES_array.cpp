#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <string.h>
#include <conio.h>
#include <time.h>
#include "transFunction.cpp"
uint8_t IV[4][4];
uint8_t* inputCipherText;
int keyBits;	//key���� 128/192/256 

int keyNum;	//�`�@key���ƶq 
int round;  //�[�K�^�X�� 
int Nk;	//key Nk�ƶq
int blocksNum; //����϶���
int cipherTextNum; //�ѱK��16�줸�Ӽ� 
int mod;	//�]�w�[�K�Ҧ�ECB CBC...... 

int encryption_decryption;	//��ܥ[�K�θѱK�Ҧ������X 1-�[�K 2-�ѱK 
int option;	//��ܭn�]�w�ﶵ���X 
int inputLength;	//��J���媺�r����� 
char inputPlainText[1600];	//�s���J������ 
char inputKey[256];	//�s���J��key 

uint8_t ctrcounter;//�s�[��ctr�p�ƾ� 
uint8_t ctriv[4][4];//�s�[��ctriv
uint8_t ctrcipher[4][4];//�s�[��ctr�[�K�᪺�K�� 
uint8_t randomiv[4][4];//�s�[��ctr�H���ܼ�
int plen;//��������; 

void randomctr() {	//�p�G�n�]ctr���H���ܼƪ��ܥγo�� 
	srand( time(NULL) );

	for(int i=0; i<2; i++)
		for(int j=0; j<4; j++)
			randomiv[i][j]=rand()%257;

	for(int i=2; i<4; i++)
		for(int j=0; j<4; j++)
			randomiv[i][j]=0X00;
	return;
}//randomctr

void printctrp(uint8_t text[][4]) {	//�L�Xctr����(�x�}) 
	for(int i=0; i<4; i++) {
        for(int j=0; j<4; j++)
            printf("%c ", text[i][j]);
        printf("\n");
    }//for
    printf("\n\n");
    return;
}//printctr����

void printctrpline(uint8_t text[][4]) {	//�L�Xctr����(�@��) 
	for(int i=0; i<4; i++) {
        for(int j=0; j<4; j++)
            printf("%c ", text[j][i]);
    }//for
    printf("\n\n");
    return;
}//printctrpline

void printctrl(uint8_t text[][4]) {	//�L�X��n�ƶq������
	int count=1;
	for(int i=0; i<4; i++) {
        for(int j=0; j<4; j++){
        	count++;
        	if(count>plen||text[j][i]==0) break;
        	printf("%02X ", text[j][i]);
            
		}
	}//for
    plen-=16;
    printf("\n\n");
    return;
}//printctrl

void printState(uint8_t text[][4]) {	//�L�X�x�}
	for(int i=0; i<4; i++) {
        for(int j=0; j<4; j++)
            printf("%02X ", text[i][j]);
        printf("\n");
    }//for
    printf("\n\n");
    return;
}//printState

void printState_line(uint8_t text[][4]) {	//�L�X�@�� 
	for(int i=0; i<4; i++) {
        for(int j=0; j<4; j++)
            printf("%02X ", text[j][i]);
    }//for
    printf("\n");
    return;
}//printState

void de_printState(uint8_t text[][4]) {	//�Ω�ECB��CBC���̫�@��block�L�X��n������ƶq 
	
	int shortageNum = text[3][3];
	uint8_t buffer[4][4] = {0x00};
		//printState(text);
	for(int i=3; i>=0; i--)
		for(int j=3; j>=0; j--){
			if(shortageNum <= 0)
				buffer[j][i] = text[j][i];
			shortageNum--;
		}//for
	//printState(buffer);
	
	matrixTranspose(buffer);
	int printNum = 16 - text[3][3];
	for(int i=0; i<4; i++) {
        for(int j=0; j<4; j++) {
        	if(printNum <= 0) break;
        	printf("%02X ", buffer[i][j]);
        	printNum--;
		}//for
        if(printNum <= 0) break;
    }//for
	//printState_line(buffer);
	return;
}//printState

void printMod() {	//�b�ﶵ������ܥX�{�b��ܪ��Ҧ� 
	switch(mod) {
		case 1:
			printf("ECB\n\n");
			break;
		case 2:
			printf("CBC\n\n");
			break;
		case 3:
			printf("CFB\n\n");
			break;
		case 4:
			printf("OFB\n\n");
			break;
		case 5:
			printf("CTR\n\n");
			break;
	}//switch
	return;
}//printMod

void IV_shift(){	//�Ω�CFB�첾IV 
	//printState(IV);
	for(int i=0; i<4; i++)
		for(int j=0; j<4; j++)
			if(j+1 == 4)
				IV[j][i]=IV[0][i+1];	
			else
				IV[j][i]=IV[j+1][i];
			
	//printf("After shift");
	//printState(IV);
	return;
}//IV_shift

void setKeyBits() {	//���key������ 
	int select;
	printf("�п�ܪ��_����(128/192/256)�G\n");
	printf("1. AES-128\n");
	printf("2. AES-192\n");
	printf("3. AES-256\n");
	scanf(" %d", &select);
	fflush(stdin);
	switch(select) {
		case 1:
			keyBits = 128;
			break;
		case 2:
			keyBits = 192;
			break;
		case 3:
			keyBits = 256;
			break;
		default:
			keyBits = 128;
			printf("\n\n�ﶵ���~�A�]���w�]��AES-128\n\n");
			printf("\n\n\n�Ы����N���~��\n");
			getch();
			fflush(stdin);
			system("cls");
			return;
	}//switch
	printf("\n\n\n�]�w���\\n");
	system("cls");
	return;
}//setKeyBits

void setPlainText() {	//��J����(�@��^��) 
	printf("�п�J���[�K��r(�^�� �̪�1600�r)�G\n");
	fgets(inputPlainText,  sizeof(inputPlainText), stdin);
	system("cls");
	return;
}//setPlainText

void setCipherText() {	//�]�w�K��(16�i��) 
	printf("�п�J���ѱK16�i��ӼơG\n");
	scanf(" %d", &cipherTextNum);
	fflush(stdin);
	inputCipherText = (uint8_t*)calloc(cipherTextNum, sizeof(uint8_t));
	printf("�п�J���ѱK16�i��(%d�� ex:1A FF...)�G\n", cipherTextNum);
	
	for(int i=0; i<cipherTextNum; i++)
		scanf(" %02X", &inputCipherText[i]);
			
	fflush(stdin);
	system("cls");
	return;
}//setPlainText

void setKey() {	//�]�wkey(�@��^��i�t�Ů�)  
	printf("�п�J���_�G\n");
	fgets(inputKey,  sizeof(inputKey), stdin);
	system("cls");
	return;
}//setKey

void setMod() {	//��ܼҦ� 
	printf("�п�ܥ[�K�Ҧ�(ECB/CBC/CFB/OFB/CTR)�G\n");
	printf("1. ECB\n");
	printf("2. CBC\n");
	printf("3. CFB\n");
	printf("4. OFB\n");
	printf("5. CTR\n");
	scanf(" %d", &mod);
	fflush(stdin);
	if(mod==1 || mod==2 || mod==3 || mod==4 || mod==5) {
		printf("\n\n\n�]�w���\\n");
	} else {
		mod = 1;
		printf("\n\n�Ҧ��]�w���~�A���]��ECB\n\n");
		printf("\n\n\n�Ы����N���~��\n");
		getch();
		fflush(stdin);
	}//if-else
	/*if(strcmp(mod, "ECB")==0 || strcmp(mod, "CBC")==0 || strcmp(mod, "CFB")==0 || strcmp(mod, "OFB")==0 || strcmp(mod, "CTR")==0) {
		printf("\n\n\n�]�w���\\n");
	} else {
		strcpy(mod, "ECB");
		printf("�Ҧ��]�w���~\n\n");
		printf("\n\n\n�Ы����N���~��\n");
		getch();
		fflush(stdin);
	}//if-else*/
	system("cls");
	return;
}//setMod 

void setIV() {	//�]�w��l�V�qIV 
	printf("�п�JIV(16��16�i��EX:1A FF...)�G\n");
	for(int i=0; i<4; i++)
		for(int j=0; j<4; j++) 
			scanf(" %02X", &IV[j][i]);		
	fflush(stdin);
	system("cls");
	return;
}//setIV

void setting() {	//��l�]�w������ 
	while(true) {
		printf("�[�ѱK��ܡG\n\n");
		printf("1. �[�K\n\n");
		printf("2. �ѱK\n\n");
		printf("�п�J�ﶵ�G"); 
		scanf(" %d", &encryption_decryption);
		system("cls");
		switch(encryption_decryption) {
			case 1:
				while(true) {
					printf("(�[�K)��J�������X�i��]�w�G\n");
					printf("padding�Ҧ��GPKCS7\n\n");
					printf("1. �[�K�Ҧ�(ECB/CBC/CFB/OFB/CTR)�G");
					printMod();					
					printf("2. ���_����(128/192/256 Bits)�G%d\n\n", keyBits);
					printf("3. ���_(�W�L%d Bits���������|�Q�ϥ�)�G\n%s\n\n", keyBits, inputKey);
					printf("4. ���[�K��r�G\n%s\n\n", inputPlainText);
					printf("5. ��l�V�q(IV)�G\n");
					for(int i=0; i<4; i++)
						for(int j=0; j<4; j++) 
							printf("%02X ", IV[j][i]);
					printf("\n\n");			
					printf("9. �}�l�[�K\n\n");
					printf("0. �W�@�B\n\n");
					//printf("0. ����\n\n");
					printf("�п�J�ﶵ�G"); 
					scanf(" %d", &option);
					fflush(stdin);
					system("cls");
					if(option == 9 || option == 0) break;
					switch(option) {
						case 1:
							setMod();
							break;
						case 2:
							setKeyBits(); 
							break;
						case 3:
							setKey();
							break;
						case 4:
							setPlainText();
							break;
						case 5:
							setIV();
						break;
					}//switch
				}//while
				break;
			case 2:
				while(true) {
					printf("(�ѱK)��J�������X�i��]�w�G\n");
					printf("padding�Ҧ��GPKCS7\n\n");
					printf("1. �[�K�Ҧ�(ECB/CBC/CFB/OFB/CTR)�G");
					printMod();
					printf("2. ���_����(128/192/256 Bits)�G%d\n\n", keyBits);
					printf("3. ���_(�W�L%d Bits���������|�Q�ϥ�)�G\n%s\n\n", keyBits, inputKey);
					printf("4. ���ѱK16�i��ӼơG%d\n\n", cipherTextNum);
					for(int i=0; i<cipherTextNum; i++)
						printf("%02X ", inputCipherText[i]);
					printf("\n\n");
					printf("5. ��l�V�q(IV)�G\n");
					for(int i=0; i<4; i++)
						for(int j=0; j<4; j++) 
							printf("%02X ", IV[j][i]);
					printf("\n\n");	
					printf("9. �}�l�ѱK\n\n");
					printf("0. �W�@�B\n\n");
					//printf("0. ����\n\n");
					printf("�п�J�ﶵ�G"); 
					scanf(" %d", &option);
					fflush(stdin);
					system("cls");
					if(option == 9 || option == 0) break;
					switch(option) {
						case 1:
							setMod();
							break;
						case 2:
							setKeyBits(); 
							break;
						case 3:
							setKey();
							break;
						case 4:
							setCipherText();
							break;
						case 5:
							setIV();
						break;
					}//switch
				}//while
				break;
		}//switch
		//if(option == 9) break;
		if(option == 9) return;
	}//while
	return;
}//setting

void initialize(int encryption_decryption) {	//��l�Ƥ@�Ǽƾ� EX:�n���X�^�X�B�`�@�n�X��Key 
	Nk = keyBits / 32;	//128-4  192-6  256-8
	round = Nk + 6;	//128-10  192-12  256-14
	keyNum = (4 * round) + 4;	//128-44  192-52  256-60
	
	if(encryption_decryption == 1) {
		//strlen()���J����r�h 1�A�ҥH��ڤj�p�n -1 
		inputLength = strlen(inputPlainText) - 1;
		if(mod == 1 || mod == 2) {	// ECB || CBC
			blocksNum = (inputLength/16) + 1;
		} else {
			//�����X��plainText_blocks
			if(inputLength < 16) blocksNum = 1;
			else if((inputLength % 16) == 0) blocksNum = inputLength/16;
			else blocksNum = (inputLength/16) + 1;
		}//if-esle
		
	} else if(encryption_decryption == 2) {
		if(cipherTextNum < 16) blocksNum = 1;
		else if((cipherTextNum % 16) == 0) blocksNum = cipherTextNum/16;
		else blocksNum = (cipherTextNum/16) + 1;
	}//if-esle-if
	return;
}//initialize

void encryption(uint8_t plainText[][4], uint32_t* keys) {	//AES�[�K 
	//printf("%d %d %d\n", Nk, round, keyNum);
	/*
	for(int i=0; i<4; i++) {
        for(int j=0; j<4; j++)
            printf("%02X ", key[i][j]);
        printf("\n");
    }
    printf("\n\n");*/
    /*
    for(int i=0; i<keyNum; i++)
        printf("%02X\n", keys[i]);
	*/
	//���N�ǹL�Ӫ���ƽƻs�istateText�קK�v�T��ƾ� 
	uint8_t stateText[4][4];
	for(int i=0; i<4; i++)
		memcpy(stateText[i], plainText[i], sizeof(plainText[i]));
		
	//printState(stateText);
	matrixTranspose(stateText);
	//printState(stateText);
	//addRoundKey
	int keyCount = 0;
	for(int i=0; i<4; i++) {
		u32_to_u8(addRoundKey_32(stateText[i], keys[keyCount]), stateText[i]);
        keyCount++;
    }//for
	//printState(stateText);
    
    //four transform
	for(int i=0; i<round; i++) {
		//subBytes
		for(int j=0; j<4; j++)
			subBytes_32(stateText[j]);
		//printState(stateText);
        
        matrixTranspose(stateText);
        //printState(stateText);
        
        //shiftRow
	    shiftRow(stateText);
		//printState(stateText);
        
        matrixTranspose(stateText);
        //mixColumn �̫�@�^�X����
		if(i != (round-1)){
			for(int j=0; j<4; j++)
	        	mixColumn_32(stateText[j]);
	        //printState(stateText);
        }//if
        
        
        //addRoundKey
	    for(int j=0; j<4; j++) {		
            u32_to_u8(addRoundKey_32(stateText[j], keys[keyCount]), stateText[j]);
            keyCount++;
        }//for
        
		//printState(stateText);
	}//for
	matrixTranspose(stateText);
	//printState(stateText);
	
	//�N���G�s�^�h 
	for(int i=0; i<4; i++)
		memcpy(plainText[i], stateText[i], sizeof(stateText[i]));
	return;
}//encryption

void decryption(uint8_t cipherText[][4], uint32_t* keys) {	//AES�ѱK 
	//Thats my Kung Fu-128
	//0x54 0x68 0x61 0x74 0x73 0x20 0x6d 0x79 0x20 0x4b 0x75 0x6e 0x67 0x20 0x46 0x75
	
	//Thats my Kung FuThats my-192
	//0x54 0x68 0x61 0x74 0x73 0x20 0x6d 0x79 0x20 0x4b 0x75 0x6e 0x67 0x20 0x46 0x75 0x54 0x68 0x61 0x74 0x73 0x20 0x6d 0x79
	
	//Thats my Kung FuThats my Kung Fu-256
	//0x54 0x68 0x61 0x74 0x73 0x20 0x6d 0x79 0x20 0x4b 0x75 0x6e 0x67 0x20 0x46 0x75 0x54 0x68 0x61 0x74 0x73 0x20 0x6d 0x79 0x20 0x4b 0x75 0x6e 0x67 0x20 0x46 0x75
	
	uint8_t stateText[4][4];
	//���N�ǹL�Ӫ���ƽƻs�istateText�קK�v�T��ƾ� 
	for(int i=0; i<4; i++)
		memcpy(stateText[i], cipherText[i], sizeof(cipherText[i]));
	matrixTranspose(stateText);
	
	//addRoundKey
	int keyCount = keyNum-1;
	for(int i=3; i>=0; i--) {
	    
		u32_to_u8(addRoundKey_32(stateText[i], keys[keyCount]), stateText[i]);
        keyCount--;
    }//for
    
	//printState(stateText);
    
    //four transform
	for(int i=0; i<round; i++) {
		matrixTranspose(stateText);
        //Inv_shiftRow
	    Inv_shiftRow(stateText);
		//printState(stateText);
		
		
        //Inv_subBytes
		/*for(int j=0; j<4; j++)
			for(int k=0; k<4; k++)
				stateText[j][k] = Inv_subBytes(stateText[j][k]);*/
		for(int j=0; j<4; j++)
			Inv_subBytes_32(stateText[j]);
		//printState(stateText);
		
		matrixTranspose(stateText);
		//printState(stateText);
        //addRoundKey
	    for(int j=3; j>=0; j--) {			
	        
		    u32_to_u8(addRoundKey_32(stateText[j], keys[keyCount]), stateText[j]);
		    //printf("%02X\n", keys[keyCount]);
            keyCount--;
            
        }//for
        //printState(stateText);
        
        
        //Inv_mixColumn �̫�@�^�X����
		if(i != (round-1)){
			for(int j=0; j<4; j++)
	        	Inv_mixColumn_32(stateText[j]);
	        //printf("mix\n");	
	        //printState(stateText);
        }//if
		
	}//for
	matrixTranspose(stateText);
	
	//�N���G�s�^�h 
	for(int i=0; i<4; i++)
		memcpy(cipherText[i], stateText[i], sizeof(stateText[i]));
	return;
}//decryption

void ECB(uint8_t cipherText_blocks[][4][4], uint8_t plainText_blocks[][4][4], uint32_t* keys) {	//ECB�[�K 
	
	for(int i=0; i<blocksNum; i++) {
		for(int j=0; j<4; j++)
			memcpy(cipherText_blocks[i][j], plainText_blocks[i][j], sizeof(plainText_blocks[i][j]));
		encryption(cipherText_blocks[i], keys);
		
		//printf("�K��%d�G\n", i);
		//printState(cipherText_blocks[i]);
		printState_line(cipherText_blocks[i]);
	}//for
	return;
}//ECB

void de_ECB(uint8_t cipherText_blocks[][4][4], uint8_t plainText_blocks[][4][4], uint32_t* keys) { //ECB�ѱK 
	for(int i=0; i<blocksNum; i++) {
		//printState(cipherText_blocks[i]);
		for(int j=0; j<4; j++)
			memcpy(plainText_blocks[i][j], cipherText_blocks[i][j], sizeof(cipherText_blocks[i][j]));
		decryption(plainText_blocks[i], keys);
		//printState(plainText_blocks[i]);
		//printf("����%d�G\n", i);
		if((i+1)!=blocksNum) {
			//printState(plainText_blocks[i]);
			printState_line(plainText_blocks[i]);
		} else {
			de_printState(plainText_blocks[i]);
		}//if-else
	}//for
	return;
}//de_ECB

void CBC(uint8_t cipherText_blocks[][4][4], uint8_t plainText_blocks[][4][4], uint32_t* keys) {	//CBC�[�K 
	for(int i=0; i<blocksNum; i++) {
		if(i == 0) {
			for(int j=0; j<4; j++)
				for(int k=0; k<4; k++)
					cipherText_blocks[i][j][k] = plainText_blocks[i][j][k] ^ IV[j][k];
		} else {
		    //printState(cipherText_blocks[i-1]);
			for(int j=0; j<4; j++)
				for(int k=0; k<4; k++)
					cipherText_blocks[i][j][k] = plainText_blocks[i][j][k] ^ cipherText_blocks[i-1][j][k];
		}//if-else
		//printState(plainText_blocks[i]);
		encryption(cipherText_blocks[i], keys);
		//printf("�K��%d�G\n", i);
		//printState(cipherText_blocks[i]);
		printState_line(cipherText_blocks[i]);
	}//for
	return;
}//CBC

void de_CBC(uint8_t cipherText_blocks[][4][4], uint8_t plainText_blocks[][4][4], uint32_t* keys) {	//CBC�ѱK 
	for(int i=0; i<blocksNum; i++) {
		for(int j=0; j<4; j++)
			memcpy(plainText_blocks[i][j], cipherText_blocks[i][j], sizeof(cipherText_blocks[i][j]));
		decryption(plainText_blocks[i], keys);
		if(i == 0) {
			for(int j=0; j<4; j++)
				for(int k=0; k<4; k++)
					plainText_blocks[i][j][k] = plainText_blocks[i][j][k] ^ IV[j][k];
		} else {
			for(int j=0; j<4; j++)
				for(int k=0; k<4; k++)
					plainText_blocks[i][j][k] = plainText_blocks[i][j][k] ^ cipherText_blocks[i-1][j][k];
		}//if-esle
		
		//printf("����%d�G\n", i);
		if((i+1)!=blocksNum) {
			//printState(plainText_blocks[i]);
			printState_line(plainText_blocks[i]);
		} else {
			de_printState(plainText_blocks[i]);
		}//if-else
	}//for
	return;
}//de_CBC

void CFB_8(uint8_t cipherText_blocks[][4][4], uint8_t plainText_blocks[][4][4], uint32_t* keys) {	//CFB-8�[�K 
	int s=8;//select 8 bits
	int block=0;	
	int y;
	int x;
	uint8_t tempPlain[4][4];
	uint8_t tempCipher[4][4];
	uint8_t reg[4][4];
			
	int inputLength = strlen(inputPlainText) - 1;
	int times=(inputLength*8)/s;
	for(int i=0,  y=0,  x=0; i<times; i++) {
		uint8_t temp[4][4];
		for(int j=0; j<4; j++)
			memcpy(temp[j], IV[j], sizeof(temp[j]));
		encryption(temp, keys);
		uint8_t left = temp[0][0];
		tempCipher[y][x] = left ^ plainText_blocks[block][y][x];
		//printf("IV=\n");
		//printState(IV);
		IV_shift();
		IV[3][3]=tempCipher[y][x];
		/*printf("after IV=\n");
		printState(IV);*/
	/*	printf("tempCipher=\n");
		printState(tempCipher);*/
		y++;
		if(y==4) {
			y=0;
			x++;
		}//if
		if(((i+1)%16) == 0) {
			for(int j=0; j<4; j++)
				for(int k=0; k<4; k++)
					cipherText_blocks[block][j][k] = tempCipher[j][k];
			x=0;
			y=0;
			block++;
		} else {
			for(int j=0; j<4; j++)
				for(int k=0; k<4; k++)
					cipherText_blocks[block][j][k] = tempCipher[j][k];
		}//if-else
		
	}//for
	//printf("block=%d\ncipher=\n",block);
	/*for(int j=0;j<=block;j++) {
		printState(cipherText_blocks[j]);
    	printState_line(cipherText_blocks[j]);
	}//for*/
	int index3=0;
	int index1=0,index2=0;
	for(int j=0;j<times;j++) {
		index1=j%4;
		if((j%4)==0 && j!=0)index2++;
		if((j%16)==0 && j!=0) {
			index3++;
			index2=0;
			index1=0;	
		}//if
		printf("%02X ", cipherText_blocks[index3][index1][index2]);
	}//for
	return;
}//CFB_8

void de_CFB_8(uint8_t cipherText_blocks[][4][4], uint8_t plainText_blocks[][4][4], uint32_t* keys) {	//CFB-8�[�K 
	int s=8;//select 8 bits
	int block=0;	
	int y;
	int x;
	uint8_t tempPlain[4][4];
	uint8_t tempCipher[4][4];
	uint8_t** reg;
	//63 36 b2 10 83 23 b5 7e e1 0d f3 c4 72 87 5a cd
	//63 36 b2 10 83 23 b5 7e e1 0d f3 c4 72 87 5a cd 65
	
	int inputLength = cipherTextNum ;
	int times=(inputLength*8)/s;
	for(int i=0,  y=0,  x=0; i<times; i++) {
		uint8_t temp[4][4];
		for(int j=0; j<4; j++)
			memcpy(temp[j], IV[j], sizeof(temp[j]));
		encryption(temp, keys);
		uint8_t left=temp[0][0];
		tempPlain[y][x] = left ^ cipherText_blocks[block][y][x];
		IV_shift();
		IV[3][3]=cipherText_blocks[block][y][x];
	/*	printf("IV=\n");
		printState(IV);
		printf("tempCipher=\n");
		printState(tempCipher);*/
		y++;
		if(y == 4) {
			y=0;
			x++;
		}//if
		
		if(((i+1)%16)==0) {
			for(int j=0; j<4; j++)
				for(int k=0; k<4; k++)
					plainText_blocks[block][j][k] = tempPlain[j][k];
			x=0;
			y=0;
			block++;
		} else {
			for(int j=0; j<4; j++)
				for(int k=0; k<4; k++)
					plainText_blocks[block][j][k] = tempPlain[j][k];
		}//if-else
	}//for
	/*printf("block=%d\nplain=\n",block);
    for(int j=0;j<=block;j++){
	    printState(plainText_blocks[j]);
	    printState_line(plainText_blocks[j]);
    }//for*/
    //printf("block=%d\nplain=\n",block);
	int index1 = 0;
	int index2 = 0;
	int index3 = 0;
    for(int j=0; j<times; j++) {
		index1=j%4;
		if((j%4)==0 && j!=0) index2++;
		if((j%16)==0 && j!=0){
			index3++;
			index2=0;
			index1=0;	
		}//if
		printf("%02X ", plainText_blocks[index3][index1][index2]);
	}//for
	return;
}//de_CFB_8

void CFB(uint8_t cipherText_blocks[][4][4], uint8_t plainText_blocks[][4][4], uint32_t* keys) {	//CFB�[�K 
	int block=0;	
	int y;
	int x;
	uint8_t tempPlain[4][4];
	uint8_t tempCipher[4][4];
	uint8_t reg[4][4];
	//128 bits
	for(int i=0; i<blocksNum; i++) {
		if(i==0){
			uint8_t temp[4][4];
			for(int j=0; j<4; j++)
				memcpy(temp[j], IV[j], sizeof(temp[j]));
			encryption(temp, keys);
			for(int j=0; j<4; j++)
				for(int k=0; k<4; k++)
					cipherText_blocks[i][j][k] = plainText_blocks[i][j][k] ^ temp[j][k];
		} else {
			uint8_t temp[4][4];
			for(int j=0; j<4; j++)
				memcpy(temp[j], reg[j], sizeof(temp[j]));
			encryption(reg, keys);
			for(int j=0; j<4; j++)
				for(int k=0; k<4; k++)
					cipherText_blocks[i][j][k] = plainText_blocks[i][j][k] ^ temp[j][k];
		}//if-else
		for(int j=0; j<4; j++)
			memcpy(reg[j], cipherText_blocks[i][j], sizeof(cipherText_blocks[i][j]));
		printf("�K��%d�G\n", i);
		printState(cipherText_blocks[i]);
		printState_line(cipherText_blocks[i]);
	}//for
	return;
}//CFB

void de_CFB(uint8_t cipherText_blocks[][4][4], uint8_t plainText_blocks[][4][4], uint32_t* keys) {	//CFB�ѱK 
	int block=0;	
	int y;
	int x;
	uint8_t tempPlain[4][4];
	uint8_t tempCipher[4][4];
	uint8_t reg[4][4];
	//63 36 b2 10 83 23 b5 7e e1 0d f3 c4 72 87 5a cd
	//63 36 b2 10 83 23 b5 7e e1 0d f3 c4 72 87 5a cd 65
	
	//128 bits
	for(int i=0; i<blocksNum; i++) {
		if(i == 0) {
			uint8_t temp[4][4];
			for(int j=0; j<4; j++)
				memcpy(temp[j], IV[j], sizeof(temp[j]));
			encryption(temp, keys);
			for(int j=0; j<4; j++)
				for(int k=0; k<4; k++)
					plainText_blocks[i][j][k] = cipherText_blocks[i][j][k] ^ temp[j][k];
		} else {
			uint8_t temp[4][4];
			for(int j=0; j<4; j++)
				memcpy(temp[j], reg[j], sizeof(temp[j]));
			encryption(temp, keys);
		//	printf("reg%d�G\n", i);
		//	printState(reg);
		//	printState_line(reg);
		//	printf("temp%d�G\n", i);
		//	printState(temp);
		//	printState_line(temp);
			for(int j=0; j<4; j++)
				for(int k=0; k<4; k++)
					plainText_blocks[i][j][k] = cipherText_blocks[i][j][k] ^ temp[j][k];
		}//if-else
		
		for(int j=0; j<4; j++)
			memcpy(reg[j], cipherText_blocks[i][j], sizeof(cipherText_blocks[i][j]));
		
		printf("����%d�G\n", i);
		if((i+1)!=blocksNum) {
			printState(plainText_blocks[i]);
			printState_line(plainText_blocks[i]);
		} else {
			de_printState(plainText_blocks[i]);
		}//if-else
	}//for
	return;
}//de_CFB

void OFB(uint8_t cipherText_blocks[][4][4], uint8_t plainText_blocks[][4][4], uint32_t* keys) {	//OFB�[�K
	uint8_t output[4][4];
	
	for(int i=0; i<blocksNum; i++) {
		if(i == 0) {
			for(int j=0; j<4; j++)
				memcpy(output[j], IV[j], sizeof(IV[j]));
			encryption(output, keys);
		} else {
			encryption(output, keys);
		}//if-else
		for(int j=0; j<4; j++)
			for(int k=0; k<4; k++)
				cipherText_blocks[i][j][k] = plainText_blocks[i][j][k] ^ output[j][k];
		printf("�K��%d�G\n", i);
		printState(cipherText_blocks[i]);
		printState_line(cipherText_blocks[i]);
	}//for
	return;
}//OFB

void de_OFB(uint8_t cipherText_blocks[][4][4], uint8_t plainText_blocks[][4][4], uint32_t* keys) {	//OFB�ѱK 
	uint8_t output[4][4];
	for(int i=0; i<blocksNum; i++) {
		if(i == 0) {
			for(int j=0; j<4; j++)
				memcpy(output[j], IV[j], sizeof(IV[j]));
			encryption(output, keys);
		} else {
			encryption(output, keys);
		}//if-else
		for(int j=0; j<4; j++)
			for(int k=0; k<4; k++)
				plainText_blocks[i][j][k] = cipherText_blocks[i][j][k] ^ output[j][k];
		printf("����%d�G\n", i);
		if((i+1)!=blocksNum) {
			printState(plainText_blocks[i]);
			printState_line(plainText_blocks[i]);
		} else {
			de_printState(plainText_blocks[i]);
		}//if-else
	}//for
	return;
}//de_OFB

void OFB_8(uint8_t cipherText_blocks[][4][4], uint8_t plainText_blocks[][4][4], uint32_t* keys) {	//OFB-8�[�K 
	uint8_t output[4][4];
	int count = 0;
	for(int i=0; i<blocksNum; i++) {
		for(int j=0; j<4; j++) {
			for(int k=0; k<4; k++) {
				for(int l=0; l<4; l++)
					memcpy(output[l], IV[l], sizeof(IV[l]));
				encryption(output, keys);
				for(int m=0; m<4; m++) {
					for(int n=0; n<4; n++) {
						if(n < 3) {
							IV[n][m] =  IV[n+1][m];
						} else if(n==3 && m<3) {
							IV[n][m] =  IV[0][m+1];
						} else if(n==3 && m==3) {
							IV[n][m] =  output[0][0];
						}//if-else
					}//for
				}//for
				cipherText_blocks[i][k][j] = plainText_blocks[i][k][j] ^ output[0][0];
				count++;
				if(count >= inputLength) break;
			}//for
			if(count >= inputLength) break;
		}//for
		if(count >= inputLength) break;
	}//for
	count = 0;
	for(int i=0; i<blocksNum; i++) {
		//printf("�K��%d�G\n", i);
		for(int j=0; j<4; j++){
			for(int k=0; k<4; k++){
				printf("%02X ",cipherText_blocks[i][k][j]);
				count++;
				if(count >= inputLength) break;
			}//for
			if(count >= inputLength) break;
		}//for
		if(count >= inputLength) break;
		printf("\n");
	}//for
	return;
}//OFB_8

void de_OFB_8(uint8_t cipherText_blocks[][4][4], uint8_t plainText_blocks[][4][4], uint32_t* keys) {	//OFB-8�ѱK 
	uint8_t output[4][4];
	int count = 0;
	for(int i=0; i<blocksNum; i++) {
		for(int j=0; j<4; j++){
			for(int k=0; k<4; k++) {
				for(int l=0; l<4; l++)
					memcpy(output[l], IV[l], sizeof(IV[l]));
				encryption(output, keys);
				
				for(int m=0; m<4; m++) {
					for(int n=0; n<4; n++) {
						if(n<3) {
							IV[n][m] =  IV[n+1][m];
						} else if(n==3 && m<3) {
							IV[n][m] =  IV[0][m+1];
						} else if(n==3 && m==3) {
							IV[n][m] =  output[0][0];
						}//if-else-if
					}//for
				}//for
				plainText_blocks[i][k][j] = cipherText_blocks[i][k][j] ^ output[0][0];
				count++;
				if(count >= cipherTextNum) break;
			}//for
			if(count >= cipherTextNum) break;
		}//for
		if(count >= cipherTextNum) break;
	}//for
	count = 0;
	for(int i=0; i<blocksNum; i++) {
		//printf("����%d�G\n", i);
		for(int j=0; j<4; j++) {
			for(int k=0; k<4; k++) {
				printf("%02X ",plainText_blocks[i][k][j]);
				count++;
				if(count >= cipherTextNum) break;
			}//for
			if(count >= cipherTextNum) break;
		}//for
		if(count >= cipherTextNum) break;
		printf("\n");
	}//for
	return;
}//de_OFB_8

void CTR(uint8_t cipherText_blocks[][4][4], uint8_t plainText_blocks[][4][4], uint32_t* keys) {	//CTR�[�K 
	//randomiv[3][3]+=1;
	plen=strlen(inputPlainText);
	uint8_t	ctri[4][4]= 
	{
		 /*{0x54, 0x4F, 0x4E, 0x20},
		 {0x77, 0x6E, 0x69, 0x54},
		 {0x00, 0x00, 0x00, 0x00},
		 {0x00, 0x00, 0x00, 0x00}	*/
		 {0x61, 0x61, 0x61, 0x61},
		 {0x61, 0x61, 0x61, 0x61},
		 {0x61, 0x61, 0x61, 0x61},
		 {0x61, 0x61, 0x61, 0x61}
	};
		//54 77 00 00 4F 6E 00 00 4E 69 00 00 20 54 00 00
	for(int i=0;i<4;i++)
		for(int j=0;j<4;j++)
			ctriv[i][j]=ctri[i][j];

	//ctrcipher=encryption(randomiv);�H��nonce
	for(int j=0; j<4; j++)
		memcpy(ctrcipher[j], ctriv[j], sizeof(ctriv[j]));
	encryption(ctrcipher, keys);
	printf("�K��G\n");
	for(int i=0; i<blocksNum; i++) {
		for(int j=0;j<4;j++)
			for(int k=0;k<4;k++)
				cipherText_blocks[i][j][k]=ctrcipher[j][k]^plainText_blocks[i][j][k];

		
		//printState(cipherText_blocks[i]);
		//printState_line(cipherText_blocks[i]);
		/*printf("\n��i�Ӫ�����:%s %d\n",inputPlainText,strlen(inputPlainText));
		for(int i=0;i<4;i++){
			for(int j=0;j<4;j++)
			printf("%c",plainText_blocks[0][j][i]);
		}*/
		//printf("�I�_�᪺�K��:");
		printctrl(cipherText_blocks[i]);
		
		ctrcounter++;//�p�ƾ� 
	}//for
	return;
}//CTR

void de_CTR(uint8_t cipherText_blocks[][4][4], uint8_t plainText_blocks[][4][4], uint32_t* keys) {	//CTR�ѱK 
	plen=strlen(inputPlainText);
	
	uint8_t	ctri[4][4] =
	{
		 /*{0x54, 0x4F, 0x4E, 0x20},
		 {0x77, 0x6E, 0x69, 0x54},
		 {0x00, 0x00, 0x00, 0x00},
		 {0x00, 0x00, 0x00, 0x00}*/ 
		 {0x61, 0x61, 0x61, 0x61},
		 {0x61, 0x61, 0x61, 0x61},
		 {0x61, 0x61, 0x61, 0x61},
		 {0x61, 0x61, 0x61, 0x61}
	};
	
	for(int i=0;i<4;i++)
		for(int j=0;j<4;j++)
			ctriv[i][j]=ctri[i][j];
	for(int j=0; j<4; j++)
		memcpy(ctrcipher[j], ctriv[j], sizeof(ctriv[j]));
	encryption(ctrcipher, keys);

	printf("����G\n");
	for(int i=0; i<blocksNum; i++) {
		for(int j=0;j<4;j++)
			for(int k=0;k<4;k++)
				plainText_blocks[i][j][k]=ctrcipher[j][k]^cipherText_blocks[i][j][k];
		printctrl(plainText_blocks[i]);
		ctrcounter++;//�p�ƾ� 
	}//for
	return;
}//de_CTR

int main() {
	uint8_t** key;	
	key = (uint8_t**)calloc(4, sizeof(uint8_t*)); //key[4][]
	mod = 1;
	keyBits = 128;
	strcpy(inputPlainText, "Two One Nine Two\n");
	//strcpy(inputPlainText, "Two One Nine Tw\n");
	//strcpy(inputPlainText, "Two One Nine TwoTwo One Ni\n");
	while(true) {
		strcpy(inputKey, "Thats my Kung FuThats my Kung Fu\n");
		for(int i=0; i<4; i++)
			for(int j=0; j<4; j++)
				IV[j][i] = 0x61;
	
		setting();	//�]�m�[�ѱK�Ҧ��BAES-128 || AES-192 || AES-256�Bkey�B����BIV
		initialize(encryption_decryption);	//��l��key�B����K��϶��j�p���}�C�j�p�Bpadding
		
		for(int i=0; i<4; i++)
			key[i] = (uint8_t*)calloc(Nk, sizeof(uint8_t));
		uint32_t keys[keyNum];
		uint8_t plainText_blocks[blocksNum][4][4];
		uint8_t cipherText_blocks[blocksNum][4][4];
		int count = 0;
		for(int i=0; i<Nk; i++)
			for(int j=0; j<4; j++){
				key[j][i] = inputKey[count];
				count++;
			}//for
		keyExpansion(key, keys, Nk, keyNum);
		
		if(encryption_decryption == 1) {
			count = 0;
			for(int i=0; i<blocksNum; i++)
				for(int j=0; j<4; j++) {
					for(int k=0; k<4; k++) {
						plainText_blocks[i][k][j] = inputPlainText[count];
						//printf("%02X ", plainText_blocks[i][k][j]);
						count++;
					}//for
					//printf("\n");
				}//for
			if(mod == 1 || mod == 2) {	// ECB || CBC
				int shortageNum = 16 - (inputLength % 16);
				PKCS7_padding(plainText_blocks[blocksNum-1], shortageNum);
			}//if
		} else if(encryption_decryption == 2) {
			count = 0;
			for(int i=0; i<blocksNum; i++)
				for(int j=0; j<4; j++) {
					for(int k=0; k<4; k++) {
						cipherText_blocks[i][k][j] = inputCipherText[count];
						//printf("%02X ", cipherText_blocks[i][k][j]);
						count++;
					}//for
					//printf("\n");
				}//for
		}//if-else-if
		
		//if(option == 0) break;
		//system("cls");
		if(encryption_decryption == 1) {
			if(mod == 1) {	//ECB
				ECB(cipherText_blocks, plainText_blocks, keys);
			} else if(mod == 2) {	//CBC
				CBC(cipherText_blocks, plainText_blocks, keys);
			} else if(mod == 3) {	//CFB-8
				CFB_8(cipherText_blocks, plainText_blocks, keys);
			} else if(mod == 4) {	//OFB-8
				OFB_8(cipherText_blocks, plainText_blocks, keys);
			} else if(mod == 5) {	//CTR
				CTR(cipherText_blocks, plainText_blocks, keys);
			}//if-else-if
		} else {
			if(mod == 1) {	//ECB
				de_ECB(cipherText_blocks, plainText_blocks, keys);
			} else if(mod == 2) {	//CBC
				de_CBC(cipherText_blocks, plainText_blocks, keys);
			} else if(mod == 3) {	//CFB-8
				de_CFB_8(cipherText_blocks, plainText_blocks, keys);
			} else if(mod == 4) {	//OFB-8
				de_OFB_8(cipherText_blocks, plainText_blocks, keys);
			} else if(mod == 5) {	//CTR
				de_CTR(cipherText_blocks, plainText_blocks, keys);
			}//if-else-if
		}//if-else
		
		//printState();
		printf("\n\n\n�Ы����N���~��\n");
		getch();
		fflush(stdin);
		system("cls");
	}//while
	free(key);
}//main
