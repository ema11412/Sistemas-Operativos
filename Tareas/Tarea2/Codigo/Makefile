all: build run

clean:
	cd bin && rm -rf ./*.bin

build:
	nasm -fbin ./src/kernel.asm -o ./bin/kernel.bin
	nasm -fbin ./src/bootloader.asm -o ./bin/bootloader.bin
	cat ./bin/bootloader.bin ./bin/kernel.bin > ./bin/Finding_Treasure_booteable.bin

run:
	qemu-system-i386 ./bin/Finding_Treasure_booteable.bin
