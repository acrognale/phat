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
uint16 read_int(unsigned char *buf, uint16 offset, uint16 length)
{
  if (length == 1) return (uint16) buf[offset];
  if (length == 2) return (uint16) buf[offset+1] << 8 | buf[offset];
  if (length == 3) return (uint16) buf[offset+2] << 16 | buf[offset+1] << 8 | buf[offset];
  if (length == 4) return (uint16) buf[offset+3] << 24 | buf[offset+2] << 16 | buf[offset+1] << 8 | buf[offset];
}

int find_root_dir(unsigned char *buf, block* blocks) 
{
  uint16 num_fats = read_int(buf, NUM_FATS, 1);
  printf("Number of FATs: %i\n", num_fats);
  uint16 fat_size = read_int(buf, FAT_SIZE, 2);
  printf("Number of blocks for one FAT: %u\n",fat_size);
  
  // blocks before root directory
  uint16 bbrd = (fat_size * num_fats) + 4;
  printf("Blocks before root dir: %u\n", bbrd);

  // get total bytes in a block
  uint16 bbp = read_int(buf, BYTES_PER_BLOCK, 2);
  printf("Bytes per block: %u\n", bbp);

  // find root directory..
  uint16 root_dir = bbrd * bbp;
  printf("Root directory is at: %x\n", root_dir);
  printf("Total blocks: %u\n", read_int(buf, TOTAL_BLOCKS, 2));
}

block* make_blocks(unsigned char *buf, size_t block_length) 
{
  int i;
  int bc = 0; // block counter
  int dc = 0; // data counter
  block *blocks;
  uint16 total_blocks;
  total_blocks = read_int(buf,TOTAL_BLOCKS,2);
  blocks = malloc(sizeof(block)*block_length*total_blocks);
  blocks->data = malloc(sizeof(unsigned char)*block_length*block_length);
  for (i = 0; i < total_blocks * block_length; i++) 
  {
    if (&blocks[bc] == NULL)
    {
      printf("blocks[bc] is null!\n");
      return NULL;
    }
    else if (&blocks[bc].data[dc] == NULL)
    {
      printf("blocks[bc].data[dc] is null!\n");
      return NULL;
    }
    else if (&buf[i] == NULL)
    {
      printf("buf[i] is null! \n");
      return NULL;
    }
    // check if a block has been iterated over yet
    if ((i % block_length) == 0 && i > 0)
    {
      // blocks[bc].num = 1;
      blocks[bc].data[dc] = buf[i]; // copy last byte over
      bc++;
      blocks[bc].data = malloc(sizeof(unsigned char)*block_length);
      dc = 0;
    } 
    else 
    { 
      blocks[bc].data[dc] = buf[i];
      dc++;
    }
  }
  return blocks;
}
