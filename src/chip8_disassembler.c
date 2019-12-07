#include "chip8_util.h"

void print_disasm_usage()
{
	printf("Usage:\n\t./c8_disassembler <rom_name>\n");
	exit(0);
}

int main(int argc, char *argv[])
{
	chip8 chip;

	if(argc != 2)
	{
		print_disasm_usage();
	}

	init_chip8_state(&chip);
	load_rom(argv[1], &chip);

	for(int i = 512; i < (sizeof(chip.memory) / sizeof(chip.memory[0])) - 512; i++)
	{
		printf("%x", chip.memory[i]);
	}
}
