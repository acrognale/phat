/*
 * fat.c - methods for interfacing with the fat16 file system
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "fat.h"

/*
 * Checks for the fat16 label in the boot block
 */

struct block {
};

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

uint16 make_word(char *buf, uint16 offset, uint16 word_length)
{
  int i;
  int p = 0;
  uint16 word = 0;
  for (i = offset+word_length; i > offset; i--)
  {
    if (word == 0) {
      word = buf[i];
    }
    else {
      word = word << 8 | buf[i];
    }
    p++;
  }
  return word;
}

int find_root_dir(char *buf) 
{
  uint16 num_fats = buf[NUM_FATS];
  printf("Number of FATs: %i\n", num_fats);
  uint16 fat_size = make_word(buf, FAT_SIZE, 2);
  printf("Number of blocks for one FAT: %u\n",fat_size);
  
  //blocks before root directory
  uint16 bbrd = fat_size * num_fats + 1;
  printf("Blocks before root dir: %u\n", bbrd);
  //get total bytes in a block
  uint16 bbp = make_word(buf, BYTES_PER_BLOCK, 2);
  printf("Bytes per block: %u\n", bbp);
  //find root directory..
  uint16 root_dir = bbrd * bbp;
  printf("Root directory is at: %x\n", root_dir);
  printf("Total blocks: %u\n", make_word(buf, 0x20, 4));
}
