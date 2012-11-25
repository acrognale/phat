/*
 * main.c - generic description
 */

#include <stdio.h>
#include <stdlib.h>
#include "fat.h"

int main(int argc, char *argv[]) 
{
	if (argc <= 1) {
    printf("Usage: %s <fat file system> \nex, %s test_file_system.img\n", argv[0],argv[0]);
    return 0;
  }

  FILE *fs;
  unsigned char *buffer;
  long lSize;
  size_t result_size;
  int i;
 

  // load the filesystem
  fs = fopen(argv[1],"r");
  if (fs != NULL) {
    fseek(fs, 0, SEEK_END);
    lSize = ftell(fs);
    rewind(fs);

    buffer = (unsigned char*)malloc(sizeof(unsigned char)*lSize);
    if (buffer == NULL) {
      printf("Buffer is null.");
      return -1;
    }

    result_size = fread(buffer,1,lSize,fs);
    if (result_size != lSize) {
      printf("Buffer size does not match file");
      return -1;
    }

    check_fat16(buffer);
    find_root_dir(buffer); 

  } else {
    printf("Could not open filesystem.");
  }

  fclose(fs);
  free(buffer);
  return 0;
}
