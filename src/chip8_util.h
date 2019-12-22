#ifndef CHIP8_UTIL
#define CHIP8_UTIL

#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>

#define WIDTH		64
#define HEIGHT	32

#define MAX_ROM_SIZE 4096 - 512

/*
 *	The Chip8 has 4096 bytes of memory.
 *		16 8-bit registers
 *		1 16-bit address register
 *		35 opcodes
 *		16-level stack
 *		stack pointer and pc
 */
typedef struct
{
	uint8_t memory[4096];
	uint8_t v[16];				// registers
	uint16_t i;
	uint16_t sp;
	uint16_t pc;
	uint16_t stack[16];

	uint8_t sound_timer;
	uint8_t delay_timer;

	uint8_t graphics[WIDTH][HEIGHT];
} chip8;

void clear_screen(chip8 *);
void init_chip8_state(chip8 *);
void load_rom(char *, chip8 *);

#endif
