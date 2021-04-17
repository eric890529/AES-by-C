#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <string.h>
#include <conio.h>
#include <time.h>
#include "transFunction.cpp"
/*
//29 c3 50 5f 57 14 20 f6 40 22 99 b3 1a 02 d7 3a b3 e4 6f 11 ba 8d 2b 97 c1 87 69 44 9a 89 e8 68
//CBC cipher 17 f5 3a 43 ab e4 c9 da 09 4a b9 bc 6e c4 56 e6
//Two One Nine Two
uint8_t plainText[4][4] = 
{
	{0x54, 0x4F, 0x4E, 0x20},
	{0x77, 0x6E, 0x69, 0x54},
	{0x6F, 0x65, 0x6E, 0x77},
	{0x20, 0x20, 0x65, 0x6F}
	/*{0x01, 0x89, 0xFE, 0x76},
	{0x23, 0xAB, 0xDC, 0x54},
	{0x45, 0xCD, 0xBA, 0x32},
	{0x67, 0xEF, 0x98, 0x10}*/
//};//plaintext
//0x0f 0x15 0x71 0xc9 0x47 0xd9 0xe8 0x59 0x0c 0xb7 0xad 0xd6 0xaf 0x7f 0x67 0x98
		
//Thats my Kung Fu-128
//0x54 0x68 0x61 0x74 0x73 0x20 0x6d 0x79 0x20 0x4b 0x75 0x6e 0x67 0x20 0x46 0x75

//cipher 29 c3 50 5f 57 14 20 f6 40 22 99 b3 1a 02 d7 3a
//IV 4E 69 6E 65   4F 6E 65 20   54 77 6F 20    4E 69 6E 65 

//Thats my Kung FuThats my-192
//0x54 0x68 0x61 0x74 0x73 0x20 0x6d 0x79 0x20 0x4b 0x75 0x6e 0x67 0x20 0x46 0x75 0x54 0x68 0x61 0x74 0x73 0x20 0x6d 0x79

//Thats my Kung FuThats my Kung Fu-256
//0x54 0x68 0x61 0x74 0x73 0x20 0x6d 0x79 0x20 0x4b 0x75 0x6e 0x67 0x20 0x46 0x75 0x54 0x68 0x61 0x74 0x73 0x20 0x6d 0x79 0x20 0x4b 0x75 0x6e 0x67 0x20 0x46 0x75

/* 
uint8_t key[4][4] = 
{
	/*{0x54, 0x73, 0x20, 0x67},
	{0x68, 0x20, 0x4B, 0x20},
	{0x61, 0x6D, 0x75, 0x46},
	{0x74, 0x79, 0x6E, 0x75}
	{0x0F, 0x47, 0x0C, 0xAF},
	{0x15, 0xD9, 0xB7, 0x7F},
	{0x71, 0xE8, 0xAD, 0x67},
	{0xC9, 0x59, 0xD6, 0x98}
};*/
uint8_t IV[4][4];
uint8_t* inputCipherText;
/*{
	{0x4E, 0x4F, 0x54, 0x4E},
	{0x69, 0x6E, 0x77, 0x69},
	{0x6E, 0x65, 0x6F, 0x6E},
	{0x65, 0x20, 0x20, 0x65}
};*/

int keyBits;

int keyNum;
int round;  //加密回合數 
int Nk;	//key Nk數量
int blocksNum; //明文區塊數
int cipherTextNum; //解密時16位元個數 
int mod;
//char mod[] = "ECB";

int encryption_decryption;//選擇加密或解密模式的號碼 1-加密 2-解密 
int option;//選擇要設定選項號碼 
int inputLength;
int totalCompute = 0;
char inputPlainText[1600];
char inputKey[256];

uint8_t ctrcounter;//新加的ctr計數器 
uint8_t ctriv[4][4];//新加的ctriv
uint8_t ctrcipher[4][4];//新加的ctr加密後的密文 
uint8_t randomiv[4][4];//新加的ctr隨機變數
int plen;//算明文長度; 

void randomctr(){//如果要設ctr的隨機變數的話用這個 
	srand( time(NULL) );

	for(int i=0;i<2;i++){
		for(int j=0;j<4;j++){
			randomiv[i][j]=rand()%257;
		}
	}
	for(int i=2;i<4;i++){
		for(int j=0;j<4;j++){
			randomiv[i][j]=0X00;
		}
	}
}

void printctrp(uint8_t text[][4]) {
	for(int i=0; i<4; i++) {
        for(int j=0; j<4; j++)
            printf("%c ", text[i][j]);
        printf("\n");
    }//for
    printf("\n\n");
}//printctr明文

void printctrpline(uint8_t text[][4]) {
	for(int i=0; i<4; i++) {
        for(int j=0; j<4; j++)
            printf("%c ", text[j][i]);
    }//for
    printf("\n\n");
}//printctrpline

void printctrl(uint8_t text[][4]) {
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
}//printctrl

void printState(uint8_t text[][4]) {
	for(int i=0; i<4; i++) {
        for(int j=0; j<4; j++)
            printf("%02X ", text[i][j]);
        printf("\n");
    }//for
    printf("\n\n");
    return;
}//printState

void printState_line(uint8_t text[][4]) {
	for(int i=0; i<4; i++) {
        for(int j=0; j<4; j++)
            printf("%02X ", text[j][i]);
    }//for
    printf("\n");
    return;
}//printState

void de_printState(uint8_t text[][4]) {
	
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

void printMod() {
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

void IV_shift(){
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

void setKeyBits() {
	int select;
	printf("請選擇金鑰長度(128/192/256)：\n");
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
			printf("\n\n選項錯誤，設為預設值AES-128\n\n");
			printf("\n\n\n請按任意鍵繼續\n");
			getch();
			fflush(stdin);
			system("cls");
			return;
	}//switch
	printf("\n\n\n設定成功\n");
	system("cls");
	return;
}//setKeyBits

void setPlainText() {
	printf("請輸入欲加密文字(英文 最長1600字)：\n");
	fgets(inputPlainText,  sizeof(inputPlainText), stdin);
	system("cls");
	return;
}//setPlainText

void setCipherText() {
	printf("請輸入欲解密16進位個數：\n");
	scanf(" %d", &cipherTextNum);
	fflush(stdin);
	inputCipherText = (uint8_t*)calloc(cipherTextNum, sizeof(uint8_t));
	printf("請輸入欲解密16進位(%d個 ex:1A FF...)：\n", cipherTextNum);
	
	for(int i=0; i<cipherTextNum; i++)
		scanf(" %02X", &inputCipherText[i]);
			
	fflush(stdin);
	system("cls");
	return;
}//setPlainText

void setKey() {
	printf("請輸入金鑰：\n");
	fgets(inputKey,  sizeof(inputKey), stdin);
	system("cls");
	return;
}//setKey

void setMod() {
	printf("請選擇加密模式(ECB/CBC/CFB/OFB/CTR)：\n");
	printf("1. ECB\n");
	printf("2. CBC\n");
	printf("3. CFB\n");
	printf("4. OFB\n");
	printf("5. CTR\n");
	scanf(" %d", &mod);
	fflush(stdin);
	if(mod==1 || mod==2 || mod==3 || mod==4 || mod==5) {
		printf("\n\n\n設定成功\n");
	} else {
		mod = 1;
		printf("\n\n模式設定錯誤，重設為ECB\n\n");
		printf("\n\n\n請按任意鍵繼續\n");
		getch();
		fflush(stdin);
	}//if-else
	/*if(strcmp(mod, "ECB")==0 || strcmp(mod, "CBC")==0 || strcmp(mod, "CFB")==0 || strcmp(mod, "OFB")==0 || strcmp(mod, "CTR")==0) {
		printf("\n\n\n設定成功\n");
	} else {
		strcpy(mod, "ECB");
		printf("模式設定錯誤\n\n");
		printf("\n\n\n請按任意鍵繼續\n");
		getch();
		fflush(stdin);
	}//if-else*/
	system("cls");
	return;
}//setMod 

void setIV() {
	printf("請輸入IV(16個16進位EX:1A FF...)：\n");
	for(int i=0; i<4; i++)
		for(int j=0; j<4; j++) 
			scanf(" %02X", &IV[j][i]);		
	fflush(stdin);
	system("cls");
	return;
}//setIV

void setting() {
	while(true) {
		printf("加解密選擇：\n\n");
		printf("1. 加密\n\n");
		printf("2. 解密\n\n");
		printf("請輸入選項："); 
		scanf(" %d", &encryption_decryption);
		system("cls");
		switch(encryption_decryption) {
			case 1:
				while(true) {
					printf("(加密)輸入對應號碼進行設定：\n");
					printf("padding模式：PKCS7\n\n");
					printf("1. 加密模式(ECB/CBC/CFB/OFB/CTR)：");
					printMod();					
					printf("2. 金鑰長度(128/192/256 Bits)：%d\n\n", keyBits);
					printf("3. 金鑰(超過%d Bits的部分不會被使用)：\n%s\n\n", keyBits, inputKey);
					printf("4. 欲加密文字：\n%s\n\n", inputPlainText);
					printf("5. 初始向量(IV)：\n");
					for(int i=0; i<4; i++)
						for(int j=0; j<4; j++) 
							printf("%02X ", IV[j][i]);
					printf("\n\n");			
					printf("9. 開始加密\n\n");
					printf("0. 上一步\n\n");
					//printf("0. 結束\n\n");
					printf("請輸入選項："); 
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
					printf("(解密)輸入對應號碼進行設定：\n");
					printf("padding模式：PKCS7\n\n");
					printf("1. 加密模式(ECB/CBC/CFB/OFB/CTR)：");
					printMod();
					printf("2. 金鑰長度(128/192/256 Bits)：%d\n\n", keyBits);
					printf("3. 金鑰(超過%d Bits的部分不會被使用)：\n%s\n\n", keyBits, inputKey);
					printf("4. 欲解密16進位個數：%d\n\n", cipherTextNum);
					for(int i=0; i<cipherTextNum; i++)
						printf("%02X ", inputCipherText[i]);
					printf("\n\n");
					printf("5. 初始向量(IV)：\n");
					for(int i=0; i<4; i++)
						for(int j=0; j<4; j++) 
							printf("%02X ", IV[j][i]);
					printf("\n\n");	
					printf("9. 開始解密\n\n");
					printf("0. 上一步\n\n");
					//printf("0. 結束\n\n");
					printf("請輸入選項："); 
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

void initialize(int encryption_decryption) {
	Nk = keyBits / 32;	//128-4  192-6  256-8
	round = Nk + 6;	//128-10  192-12  256-14
	keyNum = (4 * round) + 4;	//128-44  192-52  256-60
	
	if(encryption_decryption == 1) {
		//strlen()比輸入的文字多 1，所以實際大小要 -1 
		inputLength = strlen(inputPlainText) - 1;
		if(mod == 1 || mod == 2) {	// ECB || CBC
			blocksNum = (inputLength/16) + 1;
		} else {
			//切成幾個plainText_blocks
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

void encryption(uint8_t plainText[][4], uint32_t* keys) {
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
	uint8_t stateText[4][4];
	for(int i=0; i<4; i++)
		memcpy(stateText[i], plainText[i], sizeof(plainText[i]));
	//printState(stateText);
	matrixTranspose(stateText);
	//printState(stateText);
	//addRoundKey
	int keyCount = 0;
	for(int i=0; i<4; i++) {
		
	    /*uint8_t bytes[4];
		u32_to_u8(keys[keyCount], bytes);
	    for(int j=0; j<4; j++) {
			stateText[i][j] = addRoundKey(plainText[i][j], bytes[j]);
			//printf("%02X, %02X\n", plainText[j][i], bytes[j]);
		}//for
		*/
		
		u32_to_u8(addRoundKey_32(stateText[i], keys[keyCount]), stateText[i]);
        keyCount++;
    }//for
	//printState(stateText);
    
    //four transform
	for(int i=0; i<round; i++) {
		//subBytes
		/*for(int j=0; j<4; j++)
			for(int k=0; k<4; k++)
				stateText[j][k] = subBytes(stateText[j][k]);*/
		for(int j=0; j<4; j++)
			subBytes_32(stateText[j]);
		//printState(stateText);
        
        matrixTranspose(stateText);
        //printState(stateText);
        //shiftRow
	    shiftRow(stateText);
		//printState(stateText);
        
        matrixTranspose(stateText);
        //mixColumn 最後一回合不做
		if(i != (round-1)){
			for(int j=0; j<4; j++)
	        	mixColumn_32_Bitch(stateText[j]);
	        //printState(stateText);
        }//if
        
        
        //addRoundKey
	    for(int j=0; j<4; j++) {		
	        /*uint8_t bytes[4];
		    u32_to_u8(keys[keyCount], bytes);
	        for(int k=0; k<4; k++) {
			    stateText[k][j] = addRoundKey(stateText[k][j], bytes[k]);
			    //printf("%02X, %02X\n", plainText[j][i], bytes[j]);
		    }//for
            */
            u32_to_u8(addRoundKey_32(stateText[j], keys[keyCount]), stateText[j]);
            keyCount++;
        }//for
        
		//printState(stateText);
	}//for
	matrixTranspose(stateText);
	//printState(stateText);
	for(int i=0; i<4; i++)
		memcpy(plainText[i], stateText[i], sizeof(stateText[i]));
}//encryption

void decryption(uint8_t cipherText[][4], uint32_t* keys) {
	//Thats my Kung Fu-128
	//0x54 0x68 0x61 0x74 0x73 0x20 0x6d 0x79 0x20 0x4b 0x75 0x6e 0x67 0x20 0x46 0x75
	
	//Thats my Kung FuThats my-192
	//0x54 0x68 0x61 0x74 0x73 0x20 0x6d 0x79 0x20 0x4b 0x75 0x6e 0x67 0x20 0x46 0x75 0x54 0x68 0x61 0x74 0x73 0x20 0x6d 0x79
	
	//Thats my Kung FuThats my Kung Fu-256
	//0x54 0x68 0x61 0x74 0x73 0x20 0x6d 0x79 0x20 0x4b 0x75 0x6e 0x67 0x20 0x46 0x75 0x54 0x68 0x61 0x74 0x73 0x20 0x6d 0x79 0x20 0x4b 0x75 0x6e 0x67 0x20 0x46 0x75
	/*for(int i=0; i<keyNum; i++) {
		printf("%02X\n", keys[i]);
	}*/ 
	uint8_t stateText[4][4];
	//先將傳過來的資料複製進stateText避免影響原數據 
	for(int i=0; i<4; i++)
		memcpy(stateText[i], cipherText[i], sizeof(cipherText[i]));
	matrixTranspose(stateText);
	
	//addRoundKey
	int keyCount = keyNum-1;
	for(int i=3; i>=0; i--) {
	    /*uint8_t bytes[4];
		u32_to_u8(keys[keyCount], bytes);
	    for(int j=0; j<4; j++) {
			stateText[j][i] = addRoundKey(cipherText[j][i], bytes[j]);
			//printf("%02X, %02X\n", cipherText[j][i], bytes[j]);
		}//for*/
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
	        /*uint8_t bytes[4];
		    u32_to_u8(keys[keyCount], bytes);
	        for(int k=0; k<4; k++) {
			    stateText[k][j] = addRoundKey(stateText[k][j], bytes[k]);
			    //printf("%02X, %02X\n", plainText[j][i], bytes[j]);
		    }//for*/ 
		    u32_to_u8(addRoundKey_32(stateText[j], keys[keyCount]), stateText[j]);
		    //printf("%02X\n", keys[keyCount]);
            keyCount--;
            
        }//for
        //printState(stateText);
        
        
        //Inv_mixColumn 最後一回合不做
		if(i != (round-1)){
			for(int j=0; j<4; j++)
	        	Inv_mixColumn_32_Bitch(stateText[j]);
	        //printf("mix\n");	
	        //printState(stateText);
        }//if
		
	}//for
	matrixTranspose(stateText);
	for(int i=0; i<4; i++)
		memcpy(cipherText[i], stateText[i], sizeof(stateText[i]));
}//decryption

void ECB(uint8_t cipherText_blocks[][4][4], uint8_t plainText_blocks[][4][4], uint32_t* keys) {
	
	for(int i=0; i<blocksNum; i++) {
		for(int j=0; j<4; j++)
			memcpy(cipherText_blocks[i][j], plainText_blocks[i][j], sizeof(plainText_blocks[i][j]));
		encryption(cipherText_blocks[i], keys);
		
		//printf("密文%d：\n", i);
		//printState(cipherText_blocks[i]);
		printState_line(cipherText_blocks[i]);
	}//for
	
}//ECB

void de_ECB(uint8_t cipherText_blocks[][4][4], uint8_t plainText_blocks[][4][4], uint32_t* keys) { 
	for(int i=0; i<blocksNum; i++) {
		//printState(cipherText_blocks[i]);
		for(int j=0; j<4; j++)
			memcpy(plainText_blocks[i][j], cipherText_blocks[i][j], sizeof(cipherText_blocks[i][j]));
		decryption(plainText_blocks[i], keys);
		//printState(plainText_blocks[i]);
		//printf("明文%d：\n", i);
		if((i+1)!=blocksNum) {
			//printState(plainText_blocks[i]);
			printState_line(plainText_blocks[i]);
		} else {
			de_printState(plainText_blocks[i]);
		}//if-else
	}//for
}//de_ECB

void CBC(uint8_t cipherText_blocks[][4][4], uint8_t plainText_blocks[][4][4], uint32_t* keys) {
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
		//printf("密文%d：\n", i);
		//printState(cipherText_blocks[i]);
		printState_line(cipherText_blocks[i]);
	}//for
}//CBC

void de_CBC(uint8_t cipherText_blocks[][4][4], uint8_t plainText_blocks[][4][4], uint32_t* keys) {
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
		
		//printf("明文%d：\n", i);
		if((i+1)!=blocksNum) {
			//printState(plainText_blocks[i]);
			printState_line(plainText_blocks[i]);
		} else {
			de_printState(plainText_blocks[i]);
		}//if-else
	}//for
}//de_CBC

void CFB_8(uint8_t cipherText_blocks[][4][4], uint8_t plainText_blocks[][4][4], uint32_t* keys) {
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
}//CFB_8

void de_CFB_8(uint8_t cipherText_blocks[][4][4], uint8_t plainText_blocks[][4][4], uint32_t* keys) {
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
}//de_CFB_8

void CFB(uint8_t cipherText_blocks[][4][4], uint8_t plainText_blocks[][4][4], uint32_t* keys) {
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
		printf("密文%d：\n", i);
		printState(cipherText_blocks[i]);
		printState_line(cipherText_blocks[i]);
	}//for
}//CFB

void de_CFB(uint8_t cipherText_blocks[][4][4], uint8_t plainText_blocks[][4][4], uint32_t* keys) {
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
		//	printf("reg%d：\n", i);
		//	printState(reg);
		//	printState_line(reg);
		//	printf("temp%d：\n", i);
		//	printState(temp);
		//	printState_line(temp);
			for(int j=0; j<4; j++)
				for(int k=0; k<4; k++)
					plainText_blocks[i][j][k] = cipherText_blocks[i][j][k] ^ temp[j][k];
		}//if-else
		
		for(int j=0; j<4; j++)
			memcpy(reg[j], cipherText_blocks[i][j], sizeof(cipherText_blocks[i][j]));
		
		printf("明文%d：\n", i);
		if((i+1)!=blocksNum) {
			printState(plainText_blocks[i]);
			printState_line(plainText_blocks[i]);
		} else {
			de_printState(plainText_blocks[i]);
		}//if-else
	}//for
}//de_CFB

void OFB(uint8_t cipherText_blocks[][4][4], uint8_t plainText_blocks[][4][4], uint32_t* keys) {
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
		printf("密文%d：\n", i);
		printState(cipherText_blocks[i]);
		printState_line(cipherText_blocks[i]);
	}//for
}//OFB

void de_OFB(uint8_t cipherText_blocks[][4][4], uint8_t plainText_blocks[][4][4], uint32_t* keys) {
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
		printf("明文%d：\n", i);
		if((i+1)!=blocksNum) {
			printState(plainText_blocks[i]);
			printState_line(plainText_blocks[i]);
		} else {
			de_printState(plainText_blocks[i]);
		}//if-else
	}//for
}//de_OFB

void OFB_8(uint8_t cipherText_blocks[][4][4], uint8_t plainText_blocks[][4][4], uint32_t* keys) {
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
		//printf("密文%d：\n", i);
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
}//OFB_8

void de_OFB_8(uint8_t cipherText_blocks[][4][4], uint8_t plainText_blocks[][4][4], uint32_t* keys) {
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
		//printf("明文%d：\n", i);
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
}//de_OFB_8

void CTR(uint8_t cipherText_blocks[][4][4], uint8_t plainText_blocks[][4][4], uint32_t* keys) {
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

	//ctrcipher=encryption(randomiv);隨機nonce
	for(int j=0; j<4; j++)
		memcpy(ctrcipher[j], ctriv[j], sizeof(ctriv[j]));
	encryption(ctrcipher, keys);
	printf("密文：\n");
	for(int i=0; i<blocksNum; i++) {
		for(int j=0;j<4;j++)
			for(int k=0;k<4;k++)
				cipherText_blocks[i][j][k]=ctrcipher[j][k]^plainText_blocks[i][j][k];

		
		//printState(cipherText_blocks[i]);
		//printState_line(cipherText_blocks[i]);
		/*printf("\n輸進來的明文:%s %d\n",inputPlainText,strlen(inputPlainText));
		for(int i=0;i<4;i++){
			for(int j=0;j<4;j++)
			printf("%c",plainText_blocks[0][j][i]);
		}*/
		//printf("截斷後的密文:");
		printctrl(cipherText_blocks[i]);
		
		ctrcounter++;//計數器 
	}//for
}//CTR

void de_CTR(uint8_t cipherText_blocks[][4][4], uint8_t plainText_blocks[][4][4], uint32_t* keys) {
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

	printf("明文%d：\n");
	for(int i=0; i<blocksNum; i++) {
		for(int j=0;j<4;j++)
			for(int k=0;k<4;k++)
				plainText_blocks[i][j][k]=ctrcipher[j][k]^cipherText_blocks[i][j][k];
		printctrl(plainText_blocks[i]);
		ctrcounter++;//計數器 
	}//for
}//de_CTR

int main() {//47 26 fa 34 9 96 d0 8 3a 5a b9 8 35 a8
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
	
		setting();	//設置加解密模式、AES-128 || AES-192 || AES-256、key、明文、IV
		initialize(encryption_decryption);	//初始化key、明文密文區塊大小的陣列大小、padding
		
		
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
		//if(totalCompute == 0)
		
		/*else
			reset(keyBits);*/
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
		/*for(int i=0; i<Nk; i++)
			for(int j=0; j<4; j++){
				printf("%c", key[j][i]);
			}//for
		printf("\n");*/
		
	
		
		/*for(int i=0; i<keyNum; i++)
			printf("%02X\n", key[i]);
		*/
		
		printf("\n\n\n請按任意鍵繼續\n");
		getch();
		fflush(stdin);
		system("cls");
	}//while
	
	free(key);
}//main
