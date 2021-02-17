#include "chip8_util.h"

void print_usage(char *prog_name)
{
  printf("Usage:\n\t./%s <ROM>\n", prog_name);
}

void print_registers(chip8 *chip)
{
  for(int i = 0; i < 16; i++)
  {
    printf("V[%X]: 0x%02X\n", i, chip->v[i]);
  }
}

void print_stack(chip8 *chip)
{
  for(int i = 0; i < 16; i++)
  {
    printf("Stack[%d]: 0x%04X\n", i, chip->stack[i]);
  }
}

void print_gfx(chip8 *chip)
{
  for(int y = 0; y < HEIGHT; y++)
  {
    for(int x = 0; x < WIDTH; x++)
    {
      if(chip->graphics[x][y] == 0)
      {
        printf("0");
      }
      else
      {
        printf(" ");
      }
    }
    printf("\n");
  }
printf("\n");
}

/*
 *  Print contents of sprite to be drawn to screen
 */
void print_sprite_data(chip8 *chip, int num_bytes, uint16_t start_addr)
{
  for(int i = 0; i < num_bytes; i++)
  {
    uint8_t mask = 0x80; // 0b10000000
    uint8_t shift = 7;
    uint8_t byte = chip->memory[start_addr + i];
    for(int j = 0; j < 8; j++)
    {
      uint8_t bit = (byte & mask) >> shift;
      printf("%d ", bit);
      shift--;
      mask /= 2;
    }
    printf("\n");
  }
}

uint8_t font_set[80] =
{
  0xF0, 0x90, 0x90, 0x90, 0xF0, // 0
  0x20, 0x60, 0x20, 0x20, 0x70, // 1
  0xF0, 0x10, 0xF0, 0x80, 0xF0, // 2
  0xF0, 0x10, 0xF0, 0x10, 0xF0, // 3
  0x90, 0x90, 0xF0, 0x10, 0x10, // 4
  0xF0, 0x80, 0xF0, 0x10, 0xF0, // 5
  0xF0, 0x80, 0xF0, 0x90, 0xF0, // 6
  0xF0, 0x10, 0x20, 0x40, 0x40, // 7
  0xF0, 0x90, 0xF0, 0x90, 0xF0, // 8
  0xF0, 0x90, 0xF0, 0x10, 0xF0, // 9
  0xF0, 0x90, 0xF0, 0x90, 0x90, // A
  0xE0, 0x90, 0xE0, 0x90, 0xE0, // B
  0xF0, 0x80, 0x80, 0x80, 0xF0, // C
  0xE0, 0x90, 0x90, 0x90, 0xE0, // D
  0xF0, 0x80, 0xF0, 0x80, 0xF0, // E
  0xF0, 0x80, 0xF0, 0x80, 0x80  // F
};

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

  for(int k = 0; k < 80; k++)
  {
    chip->memory[k] = font_set[k];
  }

  for(int l = 0; l < 16; l++)
  {
    chip->keypad[l] = 0;
  }

  clear_screen(chip);
  chip->pc = 512;
}

void clear_screen(chip8 *chip)
{
  for(int i = 0; i < HEIGHT; i++)
  {
    for(int j = 0; j < WIDTH; j++)
    {
      chip->graphics[j][i] = 0;
    }
  }
}

/*
 *  First open a file, get its size -> fopen, fseek, ftell
 *  Check its size against max size
 *  If rom size is small enough, read
 *  it into the chip8's memory region
 *  starting at 0x200 or 512
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
