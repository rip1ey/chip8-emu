#include "chip8_util.h"

void print_usage(char *prog_name)
{
	printf("Usage:\n\t./%s <ROM>\n", prog_name);
}

void init_chip8_state(chip8 *chip)
{
	for(int i = 0; i < 4096; i++)
	{
		chip->memory[i] = 0;
	}

	for(int j = 0; j < 16; j++)
	{
		chip->stack[j] = 0;
		chip->v[j] = 0;
	}
}

/*
 *	First open a file, get its size -> fopen, fseek, ftell
 *	Check its size against max size
 *	If rom size is small enough, read
 *	it into the chip8's memory region
 *	starting at 0x200 or 512
 */
void load_rom(char *rom, chip8 *chip)
{
	FILE *fp;
	int rom_size;
	int rom_bytes_read;

	if((fp = fopen(rom, "r")) == NULL)
	{
		printf("File could not be opened for reading.\n");
		exit(1);
	}

	fseek(fp, 0, SEEK_END);
	rom_size = ftell(fp);
	rewind(fp);

	if(rom_size > MAX_ROM_SIZE)
	{
		printf("Not enough memory for ROM.\n");
		exit(0);
	}
	else if(rom_size < 8)
	{
		printf("Not a valid ROM.\n");
		exit(0);
	}

	uint8_t buffer[rom_size];
	if((rom_bytes_read = fread(buffer, 1, rom_size, fp)) < rom_size)
	{
		printf("Failed to read ROM into memory.\n");
		exit(1);
	}

	for(int i = 0; i < rom_size; i++)
	{
		chip->memory[i + 512] = buffer[i];
	}
	fclose(fp);
}
