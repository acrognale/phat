/*
 * fat.c - methods for interfacing with the fat16 file system
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "fat.h"

int check_fat16(unsigned char *buf) 
{
  unsigned char *signature;
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
  else 
  {
    printf("No FAT16 file system was detected.\n");
    return 1;
  }
}

/*
 * Based on my testing, this method doesn't work very well :S
 */
uint16 make_word(unsigned char *buf, uint16 offset, uint16 word_length)
{
  int i;
  int p = 0;
  uint16 word;
  for (i = offset+word_length; i > offset; i--)
  {
    if (!word) {
      word = buf[i];
    }
    else {
      word = word << 8 | buf[i];
    }
    p++;
  }
  return word;
}

int find_root_dir(unsigned char *buf) 
{
  uint16 num_fats = buf[NUM_FATS];
  printf("Number of FATs: %i\n", num_fats);
  uint16 fat_size = buf[FAT_SIZE+1] << 8 | buf[FAT_SIZE];
  printf("Number of blocks for one FAT: %u\n",fat_size);
  
  // blocks before root directory
  uint16 bbrd = (fat_size * num_fats) + 4;
  printf("Blocks before root dir: %u\n", bbrd);

  // get total bytes in a block
  // uint16 bbp = make_word(buf, BYTES_PER_BLOCK, 2);
  uint16 bbp = buf[BYTES_PER_BLOCK+1] << 8 | buf[BYTES_PER_BLOCK];
  printf("Bytes per block: %u\n", bbp);

  // find root directory..
  uint16 root_dir = bbrd * bbp;
  printf("Root directory is at: %x\n", root_dir);
  printf("Total blocks: %u\n", make_word(buf, TOTAL_BLOCKS, 2));
}

block* make_blocks(unsigned char *buf, size_t block_length) 
{
  int i;
  int bc = 0; // block counter
  int dc = 0; // data counter
  block *blocks;
  uint16 total_blocks;
  total_blocks = make_word(buf,TOTAL_BLOCKS,2);
  blocks = (block*)malloc(sizeof(block)*total_blocks);
  blocks->data = (char*)malloc(sizeof(uint16)*block_length);
  for (i = 0; i < total_blocks * block_length; i++) 
  {
    // check if a block has been iterated over yet
    if ((i % block_length) == 0 && i > 0)
    {
      blocks[bc].num = 1;
      bc++;
      dc = 0;
      printf("Block number %i created. \n", bc);
    }
    blocks[bc].data[dc] = buf[i];
    dc++;
  }

  printf("%u",blocks[0].data[0]);
  return blocks;
}
