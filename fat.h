/* fat.h -
 * Header file that contains various definitions about the FAT file system
 */

typedef unsigned short int uint16;
typedef struct {
  uint16 num;
  uint16 length;
  unsigned char *data;
} block;

typedef struct {
  file * files;
} directory;

typedef struct {
  unsigned long long filename;
  int file_extension;
  int file_attributes;
} file;


int check_fat16(unsigned char *buf);
int find_root_dir(unsigned char *buf, block* blocks);
uint16 read_int(unsigned char *buf, uint16 offset, uint16 word_length);
uint16 ctouint16(unsigned char *buf, size_t length);
block *make_blocks(unsigned char *buf, size_t block_length);
uint16 find_offset(block b);


/*
 * Methods for interfacing with files on the file system
 */
int get_file_attrs(block blocks, uint16 root_dir, file f);
int get_file_date(block blocks, uint16 root_dir, file f);
unsigned char * get_file_data(block blocks, uint16 root_dir, file f);

#define FAT_16_SIGNATURE 0x36
#define NUM_FATS 0x10
#define FAT_SIZE 0x16
#define BYTES_PER_BLOCK 0x0b
#define TOTAL_BLOCKS 0x13
#define TOTAL_BLOCKS_2 0x20
#define FIRST_CLUSTER 0x0002
#define ROOT_ENTRIES 0x11
