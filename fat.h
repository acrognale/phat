/* fat.h -
 * Header file that contains various definitions about the FAT file system
 */

typedef unsigned short int uint16;

int check_fat16(unsigned char *buf);
int find_root_dir(unsigned char *buf);
uint16 get_byte_array(unsigned char *buf, uint16 offset, uint16 length);
uint16 ctouint16(unsigned char *buf, size_t length);

#define FAT_16_SIGNATURE 0x36
#define NUM_FATS 0x10
#define FAT_SIZE 0x16
#define BYTES_PER_BLOCK 0x0b
