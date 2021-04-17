/*#include <stdio.h>
#include <stdint.h>*/
/*
uint32_t addRoundKey(uint32_t text, uint32_t key) {
    return text^key;
}//addRoundKey
*/
uint8_t addRoundKey(uint8_t text, uint8_t key) {
    return text^key;
}//addRoundKey

uint32_t addRoundKey_32(uint8_t* text, uint32_t key) {
	uint32_t buffer = u8_to_u32(text);
    return buffer ^  key;
}//addRoundKey_32
