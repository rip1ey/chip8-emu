#include "chip8.h"

void print_emu_usage()
{
	printf("Usage:\n\t./chip8 <rom>\n");
	exit(0);
}

void exec_0_op(uint16_t inst, chip8 *chip)
{
	switch(inst)
	{
		case 0x00E0:
			printf("00E0 -> Clear screen\n");
			clear_screen(chip);
			chip->pc+=2;
			break;
		case 0x00EE:
			printf("00EE -> Return from subroutine\n");
			uint16_t ret_addr = chip->stack[chip->sp];
			chip->sp--;
			chip->pc = ret_addr;
			break;
		default:
			printf("Unrecognized instruction\n");
			break;
	}
}

void exec_1_op(uint16_t inst, chip8* chip)
{
	printf("1NNN -> Jump to addr NNN\n");
	chip->pc = inst & 0x0FFF;
}

/*
 * - Stack pointer needs to be moved up
 * - The addr of new instruction needs to
 *	 be saved on the stack
 * - PC needs to be set to the addr NNN
 */
void exec_2_op(uint16_t inst, chip8* chip)
{
	printf("2NNN -> Call routine at addr NNN\n");
	chip->sp++;
	chip->stack[chip->sp] = chip->pc+2;
	chip->pc = inst & 0x0FFF;
}

void exec_3_op(uint16_t inst, chip8* chip)
{
	printf("3XNN -> Skip next inst if Vx == NN\n");
	uint8_t x = (inst & 0x0F00) >> 8;
	uint8_t nn = inst & 0x00FF;

	if(chip->v[x] == nn)
	{
		chip->pc+=2;
	}

	chip->pc+=2;
}

void exec_4_op(uint16_t inst, chip8* chip)
{
	printf("4XNN -> Skip next inst if Vx != NN\n");
	uint8_t x = (inst & 0x0F00) >> 8;
	uint8_t nn = inst & 0x00FF;

	if(chip->v[x] != nn)
	{
		chip->pc+=2;
	}

	chip->pc+=2;
}

void exec_5_op(uint16_t inst, chip8* chip)
{
	printf("5XY0 -> Skip next inst if Vx == Vy\n");
	uint8_t x = (inst & 0x0F00) >> 8;
	uint8_t y = (inst & 0x00F0) >> 4;

	if(chip->v[x] == chip->v[y])
	{
		chip->pc+=2;
	}

	chip->pc+=2;
}

void exec_6_op(uint16_t inst, chip8* chip)
{
	printf("6XNN -> Set Vx to NN\n");
	uint8_t x = (inst & 0x0F00) >> 8;
	uint8_t nn = inst & 0x00FF;

	chip->v[x] = nn;
	chip->pc+=2;
}

void exec_7_op(uint16_t inst, chip8* chip)
{
	printf("7XNN -> Adds NN to Vx\n");
	uint8_t x = (inst & 0x0F00) >> 8;
	uint8_t nn = inst & 0x00FF;

	chip->v[x] += nn;
	chip->pc+=2;
}

void exec_8_op(uint16_t inst, chip8* chip)
{
  uint8_t x = (inst & 0x0F00) >> 8;
  uint8_t y = (inst & 0x00F0) >> 4;

	switch(inst & 0x000F)
	{
    case 0x00:
      printf("8XY0 -> Sets the value of Vx to the value of Vy\n");
      chip->v[x] = chip->v[y];
      break;
		case 0x01:
      printf("8XY1 -> Sets Vx to Vx OR Vy\n");
      chip->v[x] |= chip->v[y];
			break;
		case 0x02:
      printf("8XY2 -> Sets Vx to Vx AND Vy\n");
      chip->v[x] &= chip->v[y];
			break;
		case 0x03:
      printf("8XY3 -> Sets Vx to Vx XOR Vy\n");
      chip->v[x] ^= chip->v[y];
			break;
		case 0x04:
      printf("8XY4 -> Add Vy to Vx, set VF to 1 if carry, 0 otherwise\n");
			break;
		case 0x05:
      printf("8XY5 -> Vy is subtracted from Vx, set VF to 1 if borrow, 0 otherwise\n");
			break;
		case 0x06:
      printf("8XY6 -> Stores the lsb of Vx into VF, shift Vx right by 1\n");
      chip->v[0xF] = chip->v[x] & 0x0001;
      chip->v[x] >>= 1;
			break;
		case 0x07:
      printf("8XY7 -> Set Vx to Vy minus Vx, VF is set to 0 when borrow, 1 otherwise\n");
			break;
    case 0x0E:
      printf("8XYE -> Store msb of Vx in VF, shift Vx to left by 1\n");
      chip->v[0xF] = (chip->v[x] & 0xF000) >> 7;
      chip->v[x] <<= 1;
      break;
	}
	chip->pc+=2;
}

void exec_9_op(uint16_t inst, chip8* chip)
{
	printf("9XY0 -> Skip next inst if Vx != Vy\n");
	uint8_t x = (inst & 0x0F00) >> 8;
	uint8_t y = (inst & 0x00F0) >> 4;

	if(chip->v[x] != chip->v[y])
	{
		chip->pc+=2;
	}
	chip->pc+=2;
}

void exec_A_op(uint16_t inst, chip8* chip)
{
	printf("ANNN -> I = addr NNN\n");

	chip->i = inst & 0x0FFF;
	chip->pc+=2;
}

void exec_B_op(uint16_t inst, chip8* chip)
{
	printf("BNNN -> Jump to addr NNN + V0\n");

	chip->pc = (inst & 0x0FFF) + chip->v[0];
}

void exec_C_op(uint16_t inst, chip8* chip)
{

  printf("CXNN -> Set Vx to the result of a bitwise AND on a random number NN\n");
  uint8_t x = (inst & 0x0F00) >> 8;
  uint8_t nn = (inst & 0x00FF);

  chip->v[x] = (rand() % 255) & nn;
	chip->pc+=2;
}

void exec_D_op(uint16_t inst, chip8* chip)
{
  uint8_t x, y, n;

  n = (inst & 0x000F);
  x = chip->v[(inst & 0x0F00) >> 8] % WIDTH;
  y = chip->v[(inst & 0x00F0) >> 4] % HEIGHT;

  chip->v[0xF] = 0;
  // grab the sprite data from memory location
  // in i register
  // for each byte in the sprite data, XOR it
  // with the data that sits in the coordinate
  // currently
  for(int byte_num = 0; byte_num < n; byte_num++)
  {
    uint8_t bit_set = 0;
    uint8_t mask = 0x80;
    uint8_t sprite_byte = chip->memory[chip->i + byte_num];
    for(int bit = 0, shift = 7; bit < 8; bit++)
    {
      // focus on leftmost bit we need
      uint8_t fcs_sprite_bit = (sprite_byte & mask) >> shift;
      uint8_t gfx_pixel = chip->graphics[x][y];
      if(gfx_pixel)
      {
        bit_set = 1;
      }
      
      chip->graphics[x][y] = fcs_sprite_bit ^ gfx_pixel;
      // bit has been erased
      if(gfx_pixel && !chip->graphics[x][y])
      {
        chip->v[0xF] = 1;
      }
      // check if any bits have been erased
      // modify mask / shift
      mask /= 2;
      shift--;
      x++;
      x %= WIDTH;
    }
    y++;
    y %= HEIGHT;
  }
  printf("DXYN -> Draw sprite\n");
	chip->pc+=2;
}

void exec_E_op(uint16_t inst, chip8* chip)
{
  switch(inst & 0x00FF)
  {
    case 0x9E:
      break;
    case 0xA1:
      break;
  }
	chip->pc+=2;
}

void exec_F_op(uint16_t inst, chip8* chip)
{
	uint8_t x = (inst & 0x0F00) >> 8;

	switch(inst & 0x00FF)
	{
		case 0x07:
			printf("FX07 -> Set Vx to delay timer\n");
			chip->v[x] = chip->delay_timer;
			break;
		case 0x0A:
      printf("FX0A -> key press is awaited, then stored in Vx\n");
			break;
		case 0x15:
			printf("FX15 -> Set delay timer to Vx\n");
			chip->delay_timer = chip->v[x];
			break;
		case 0x18:
			printf("FX18 -> Set sound timer to Vx\n");
      chip->sound_timer = chip->v[x];
			break;
		case 0x1E:
			printf("FX1E -> Add Vx to I\n");
      chip->i += chip->v[x];
			break;
		case 0x29:
      printf("FX29 -> Set reg I to sprite in Vx\n");
      chip->i = chip->v[x];
			break;
		case 0x33:
    {
      uint8_t div = 100;
      uint8_t dec_val = chip->v[x];
      printf("FX33 -> Store binary coded decimal in I, I+1, I+2\n");
      for(uint8_t i = 0; i < 3; i++)
      {
        chip->memory[chip->i + i] = dec_val / div;
        dec_val %= div;
        div /= 10;
      }
			break;
    }
		case 0x55:
      // stores v0-vx inclusive in memory
      // starting at address in I
      for(int i = 0; i < 16; i++)
      {
        chip->memory[chip->i + i] = chip->v[i];
      }
			break;
		case 0x65:
      // fill v0-vx with the contents of the addr
      // contained in reg I
      for(int i = 0; i < 16; i++)
      {
        chip->v[i] = chip->memory[chip->i + i]; 
      }
			break;
	}

	chip->pc+=2;
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
  srand(time(0));

	// cuts down on the giant switch statement
	void (*func_arr[])(uint16_t, chip8*) =
	{
		exec_0_op, exec_1_op, exec_2_op, exec_3_op,
		exec_4_op, exec_5_op, exec_6_op, exec_7_op,
		exec_8_op, exec_9_op, exec_A_op, exec_B_op,
		exec_C_op, exec_D_op, exec_E_op, exec_F_op,
	};

	exit_chip8 = -2;
	while(exit_chip8 != 3)
	{
		uint8_t nibble = chip.memory[chip.pc] >> 4; 
		uint16_t inst = chip.memory[chip.pc] << 8 | chip.memory[chip.pc + 1]; 
		printf("Nibble: %02X\n", nibble);
		printf("Instruction: %04X\n", inst);
		func_arr[nibble](inst, &chip);
		exit_chip8++;
	}
}
