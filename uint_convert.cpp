/*
#include <stdio.h>
#include <stdint.h>
*/

uint32_t u8_to_u32(const uint8_t* bytes) {
  uint32_t u32 = (bytes[0] << 24) + (bytes[1] << 16) + (bytes[2] << 8) + bytes[3];
  return u32;
}//u8_to_u32

void u32_to_u8(const uint32_t u32, uint8_t* u8) {
  u8[0] = (u32 & 0xff000000) >> 24;
  u8[1] = (u32 & 0x00ff0000) >> 16;
  u8[2] = (u32 & 0x0000ff00) >> 8;
  u8[3] = u32 & 0x000000ff;
}//u32_to_u8
/*
int main() {
  uint8_t  test_1_u8[4] = {0x12, 0x34, 0x56, 0x78};
  uint32_t test_1_u32   = u8_to_u32(test_1_u8);
  printf("test_1_u32 == 0x12345678 -> %d\n", test_1_u32 == 0x12345678);
  
  uint32_t test_2_u32   = 0x87654321;
  uint8_t  test_2_u8[4];
  u32_to_u8(test_2_u32, test_2_u8);
  printf("0x87654321 can be rewritten as\n0x87, 0x65, 0x43, 0x21\n");
  printf("0x87 == test_2_u8[0] -> %d\n", 0x87 == test_2_u8[0]);
  printf("0x65 == test_2_u8[1] -> %d\n", 0x65 == test_2_u8[1]);
  printf("0x43 == test_2_u8[2] -> %d\n", 0x43 == test_2_u8[2]);
  printf("0x21 == test_2_u8[3] -> %d\n", 0x21 == test_2_u8[3]);
  
  return 0;
}*/
