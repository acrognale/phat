SOURCES = main.c fat.c

all:
	gcc $(SOURCES) -o phat -ggdb

fs:
	dd bs=512 count=20000 if=/dev/zero of=test_file_system.img
	mkfs.msdos -F 16 test_file_system.img
	sudo mkdir /mnt/phat
	sudo mount -o loop,rw test_file_system.img /mnt/phat
	sudo echo "This is a test file for phat." > /mnt/phat/test_file.txt
	sudo umount /mnt/phat
