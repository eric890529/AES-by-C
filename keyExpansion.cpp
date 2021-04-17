/*#include <stdio.h>
#include <stdint.h>

uint8_t input[4][4] = 
{
	{0x0F, 0x47, 0x0C, 0xAF},
	{0x15, 0xD9, 0xB7, 0x7F},
	{0x71, 0xE8, 0xAD, 0x67},
	{0xC9, 0x59, 0xD6, 0x98}
};//input
*/

uint32_t g(uint32_t word, uint32_t rcon) {
	uint8_t bytes[4];
	u32_to_u8(word, bytes);
	
	//shift
	uint8_t buffer;
	buffer = bytes[0];
	for(int i=0; i<3; i++)
		bytes[i] = bytes[i+1];
	bytes[3] = buffer;
	
	//subBytes
	for(int i=0; i<4; i++)
		bytes[i] = subBytes(bytes[i]);
	
	return rcon^u8_to_u32(bytes);
}//g

uint32_t rcon[14] = {0x01000000, 0x02000000, 0x04000000, 0x08000000, 0x10000000, 0x20000000, 0x40000000, 
					 0x80000000, 0x1B000000, 0x36000000, 0x6C000000, 0xD8000000, 0xAB000000, 0x4D000000};
					 
void keyExpansion(uint8_t* text[4], uint32_t* key, int Nk, int keyNum) {
	//uint32_t key[keyNum];
	uint8_t bytes[4];
	int keyCount = 0;
	//第1~Nk個key 
	for(int i=0; i<Nk; i++) {
		bytes[0] = text[0][i];
		bytes[1] = text[1][i];
		bytes[2] = text[2][i];
		bytes[3] = text[3][i];
		key[keyCount] = u8_to_u32(bytes);
		keyCount++;
	}//for
	
	//第Nk~keyNum個key 
	int rconCount = 0;
	for(int i=0; i<(keyNum-Nk); i++) {
		if(keyCount%Nk == 0) {
			key[keyCount] = key[keyCount-Nk] ^ g(key[keyCount-1], rcon[rconCount]);
			rconCount++;
		} else if(keyCount%Nk == 4 && Nk > 6){
			uint8_t bytes[4];
			u32_to_u8(key[keyCount-1], bytes);
			//subBytes
			for(int i=0; i<4; i++)
				bytes[i] = subBytes(bytes[i]);
			key[keyCount] = key[keyCount-Nk] ^ u8_to_u32(bytes);
		} else {
			key[keyCount] = key[keyCount-Nk] ^ key[keyCount-1];
		}//if-else-if
		keyCount++;
	}//for

	
	/*printf("%d\n", rconCount);
	for(int i=0; i<44; i++)
		printf("%d:%x\n", i, key[i]);*/
}//keyExpansion

/*
int main() {
	int keyNum = 44;
	uint32_t key[keyNum];
	keyExpansion(input, key, keyNum);
	for(int i=0; i<44; i++) {
		printf("%x\n", key[i]);
	}
	
}*/

