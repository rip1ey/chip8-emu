#include "chip8.h"

void print_emu_usage()
{
	printf("Usage:\n\t./chip8 <rom>\n");
	exit(0);
}

int main(int argc, char *argv[])
{
	chip8 chip;
	int exit_chip8 = 0;

	if(argc != 2)
	{
		print_emu_usage();
	}

	init_chip8_state(&chip);
	load_rom(argv[1], &chip);

	while(!exit_chip8)
	{
		// fetch
		// decode
		// execute
	}
}
