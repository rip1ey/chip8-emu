#include "chip8.h"

void print_emu_usage()
{
	printf("Usage:\n\t./chip8 <rom>\n");
	exit(0);
}

void exec_0_op(uint16_t inst, chip8 *chip)
{
  uint16_t nnn = (inst & 0x0FFF);

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
			printf("0NNN -> Call routine at NNN\n");
      chip->pc = nnn;
			break;
	}
}

void exec_1_op(uint16_t inst, chip8* chip)
{
	printf("1NNN -> Jump to addr NNN\n");
  if((inst & 0x0FFF) < 512 || (inst & 0x0FFF) > 4096)
  {
    printf("Attempted to jump to address out of bounds. Exiting...\n");
    exit(1);
  }
  else
  {
	  chip->pc = inst & 0x0FFF;
  }
  
  uint16_t next_inst = chip->memory[chip->pc] << 8 | chip->memory[chip->pc + 1];
  printf("Next instruction to be executed: %04X\n", next_inst);
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
      chip->v[0xF] = 0;

      if(chip->v[x] + chip->v[y] > 255)
      {
        chip->v[0xF] = 1;
      }
      chip->v[x] += chip->v[y];
			break;
		case 0x05:
      printf("8XY5 -> Vy is subtracted from Vx, set VF to 0 if borrow, 1 otherwise\n");
      if(chip->v[y] > chip->v[x])
      {
        chip->v[0xF] = 0;
      }
      else
      {
        chip->v[0xF] = 1;
      }
      
      chip->v[x] -= chip->v[y];
			break;
		case 0x06:
      printf("8XY6 -> Store value of Vy shifted right once in Vx. Set VF to lsb prior to shift\n");
      chip->v[0xF] = chip->v[y] & 0x0001;
      chip->v[x] = chip->v[y] >> 1;
			break;
		case 0x07:
      printf("8XY7 -> Set Vx to Vy minus Vx, VF is set to 0 when borrow, 1 otherwise\n");
      if(chip->v[x] > chip->v[y])
      {
        chip->v[0xF] = 0;
      }
      else
      {
        chip->v[0xF] = 1;
      }

      chip->v[x] = chip->v[y] - chip->v[x];
			break;
    case 0x0E:
      printf("8XYE -> Store value of Vy shifted left once in Vx. Set VF to msb prior to shift\n");
      printf("IN 8XYE, MSB OF VY: %d, VALUE OF VY: %d, VF BEFORE SHIFT: %d\n", (chip->v[y] & 0x80) >> 7, chip->v[y], chip->v[0xF]);
      chip->v[0xF] = chip->v[y] >> 7;
      chip->v[x] = chip->v[y] << 1;
      printf("VF AFTER SHIFT: %d\n", chip->v[0xF]);
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
  x = chip->v[(inst & 0x0F00) >> 8];
  y = chip->v[(inst & 0x00F0) >> 4];

  chip->v[0xF] = 0;
  uint8_t bit_set = 0;
  // grab the sprite data from memory location
  // in i register
  // for each byte in the sprite data, XOR it
  // with the data that sits in the coordinate
  // currently
  for(int byte_num = 0; byte_num < n; byte_num++)
  {
    uint8_t mask = 0x80;
    uint8_t sprite_byte = chip->memory[chip->i + byte_num];
    for(int bit = 0, shift = 7; bit < 8; bit++)
    {
      int x_coord = (x + bit) % WIDTH;
      int y_coord = (y + byte_num) % HEIGHT;
      // focus on leftmost bit we need
      uint8_t fcs_sprite_bit = (sprite_byte & mask) >> shift;
      uint8_t gfx_pixel = chip->graphics[x_coord][y_coord];
      //printf("gfx[x][y] before xor: %d\n", chip->graphics[x_coord][y_coord]);
      if(gfx_pixel)
      {
        bit_set = 1;
      }
      
      chip->graphics[x_coord][y_coord] = fcs_sprite_bit ^ gfx_pixel;
      // bit has been erased
      if(bit_set && !chip->graphics[x_coord][y_coord])
      {
        chip->v[0xF] = 1;
      }
      // check if any bits have been erased
      // modify mask / shift
      //printf("sprite_byte: %d\nMask: %d\nShift: %d\nx: %d\ny: %d\ngfx[x][y] after xor: %d\n", sprite_byte, mask, shift, x, y, chip->graphics[x_coord][y_coord]);
      mask /= 2;
      shift--;
    }
  }
  printf("DXYN -> Draw sprite\n");
	chip->pc+=2;
}

void exec_E_op(uint16_t inst, chip8* chip)
{
  uint8_t x = (inst & 0x0F00) >> 8;
  switch(inst & 0x00FF)
  {
    case 0x9E:
      printf("EX9E -> Skips next inst if key in Vx is pressed\n");
      if(chip->keypad[chip->v[x]] == 1)
      {
        printf("KEY %d has been pressed\n", chip->v[x]);
        chip->pc+=2;
      }
      break;
    case 0xA1:
      printf("EXA1 -> Skips next inst if key in Vx isn't pressed\n");
      if(chip->keypad[chip->v[x]] == 0)
      {
        printf("KEY %d has not been pressed\n", chip->v[x]);
        chip->pc+=2;
      }
      break;
  }
	chip->pc+=2;
}

/*
 * If current inst is FX0A, only
 * increase pc if a key is pressed
 */
void exec_F_op(uint16_t inst, chip8* chip)
{
	uint8_t x = (inst & 0x0F00) >> 8;
  uint8_t key_pressed;

	switch(inst & 0x00FF)
	{
		case 0x07:
			printf("FX07 -> Set Vx to delay timer\n");
			chip->v[x] = chip->delay_timer;
      chip->pc+=2;
			break;
		case 0x0A:
      printf("FX0A -> key press is awaited, then stored in Vx\n");
      for(int i = 0; i < 16; i++)
      {
        if(chip->keypad[i] == 1)
        {
          printf("KEY %d has been pressed\n", i);
          chip->v[x] = i;
          chip->pc+=2;
          key_pressed = i;
          break;
        } 
      }
      chip->keypad[key_pressed] = 0;
			break;
		case 0x15:
			printf("FX15 -> Set delay timer to Vx\n");
			chip->delay_timer = chip->v[x];
      chip->pc+=2;
			break;
		case 0x18:
			printf("FX18 -> Set sound timer to Vx\n");
      chip->sound_timer = chip->v[x];
      chip->pc+=2;
			break;
		case 0x1E:
			printf("FX1E -> Add Vx to I\n");
      chip->i += chip->v[x];
      chip->pc+=2;
			break;
		case 0x29:
      printf("FX29 -> Set reg I to sprite in Vx\n");
      chip->i = chip->v[x] * 5;
      chip->pc+=2;
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
      chip->pc+=2;
			break;
    }
		case 0x55:
      // stores v0-vx inclusive in memory
      // starting at address in I
      for(int i = 0; i <= x; i++)
      {
        chip->memory[chip->i + i] = chip->v[i];
      }
      chip->pc+=2;
			break;
		case 0x65:
      // fill v0-vx with the contents of the addr
      // contained in reg I
      for(int i = 0; i <= x; i++)
      {
        chip->v[i] = chip->memory[chip->i + i]; 
      }
      chip->pc+=2;
			break;
	}
}
