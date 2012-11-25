/*
 * fat.c - methods for interfacing with the fat16 file system
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "fat.h"

// A few offsets
#define FAT_16_SIGNATURE 0x36
#define NUM_FATS 0x10
#define FAT_SIZE_1 0x16
#define FAT_SIZE_2 0x17
#define BYTES_PER_BLOCK 0x0b

/*
 * Checks for the fat16 label in the boot block
 */
int check_fat16(char *buf) 
{
  char *signature;
  uint16 i;
  uint16 p;
  p = 0;
  for (i = FAT_16_SIGNATURE; i < FAT_16_SIGNATURE + 5; i++)
  {
    signature[p] = buf[i];
    p++;
  }
  if (strcmp(signature,"FAT16") == 0) 
  {
    printf("FAT16 file system detected!\n");
    return 0;
  }
}

uint16 ctouint16(char *buf, size_t length) 
{
  int i;
  uint16 *result;
  result = (uint16*)malloc(sizeof(char)*length);
  for (i = 0; i < length; i++)
  {
    result[i] = (uint16)buf[i];
  }
  return *result;
}

uint16 add_byte_array(char *buf, uint16 offset, uint16 length) 
{
  uint16 i;
  uint16 p = 0;
  uint16 *arr = (uint16*)malloc(sizeof(uint16)*length);
    
  for (i = buf[offset]; i < buf[offset+length]; i++)
  {
    arr[p] = buf[i];
    p++;
  }
  uint16 res = 0;
  for (i = 0; i < length; i++)
  {
    res = res + arr[i];
    printf("%u\n", res);
  }
  return res;
}

int find_root_dir(char *buf) 
{
  uint16 num_fats = buf[NUM_FATS];
  printf("Number of FATs: %i\n", num_fats);
  uint16 size1 = buf[FAT_SIZE_1];
  uint16 size2 = buf[FAT_SIZE_2];
  uint16 fat_size = size1 + size2;
  printf("Number of blocks for one FAT: %u\n",fat_size);
  
  //blocks before root directory
  uint16 bbrd = fat_size * num_fats + 1;
  printf("Blocks before root dir: %u\n", bbrd);
  //get total bytes in a block
  uint16 bbp = buf[BYTES_PER_BLOCK];
  printf("Bytes per block: %u\n", bbp);
  bbp = add_byte_array(buf, BYTES_PER_BLOCK, 2);
  printf("Bytes per block: %u\n", bbp);
}
