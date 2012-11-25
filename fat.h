/* fat.h -
 * Header file that contains various definitions about the FAT file system
 */

typedef unsigned short int uint16;

int check_fat16(char *buf);
int find_root_dir(char *buf);
uint16 get_byte_array(char *buf, uint16 offset, uint16 length);
uint16 ctouint16(char *buf, size_t length);


