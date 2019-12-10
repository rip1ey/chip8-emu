#include "chip8_util.h"
#include "chip8_disassembler.h"

void print_disasm_usage()
{
	printf("Usage:\n\t./c8_disassembler <rom_name>\n");
	exit(0);
}

void read_instructions(chip8 *chip)
{
	int max_rom = 4096 - 512;

	for(int i = 512; i < max_rom; i+=2)
	{
		uint16_t inst = ((chip->memory[i] << 8) | chip->memory[i+1]);
		//printf("%04x ", inst);
		decode_instruction(inst);
	}
}

/*
 *	Each Chip8 opcode is 16 bits long
 *	
 *
 */
void decode_instruction(uint16_t inst)
{
	switch(inst & 0xF000)
	{
		case 0x0000:
			determine_0_opcode(inst);
			break;
		case 0x1000:
			determine_1_opcode(inst);
			break;
		case 0x2000:
			determine_2_opcode(inst);
			break;
		case 0x3000:
			determine_3_opcode(inst);
			break;
		case 0x4000:
			determine_4_opcode(inst);
			break;
		case 0x5000:
			determine_5_opcode(inst);
			break;
		case 0x6000:
			determine_6_opcode(inst);
			break;
		case 0x7000:
			determine_7_opcode(inst);
			break;
		case 0x8000:
			determine_8_opcode(inst);
			break;
		case 0x9000:
			determine_9_opcode(inst);
			break;
		case 0xA000:
			determine_A_opcode(inst);
			break;
		case 0xB000:
			determine_B_opcode(inst);
			break;
		case 0xC000:
			determine_C_opcode(inst);
			break;
		case 0xD000:
			determine_D_opcode(inst);
			break;
		case 0xE000:
			determine_E_opcode(inst);
			break;
		case 0xF000:
			determine_F_opcode(inst);
			break;
		default:
			printf("Invalid instruction\n");
			break;
	}
}

/*
 *	Determine which of the 0x0000 opcodes
 *	are being used
 */
void determine_0_opcode(uint16_t inst)
{
	switch(inst)
	{
		case 0x00E0:
			printf("00E0 -> CLR (Clear screen)\n");
			break;
		case 0x00EE:
			printf("00EE -> RET (Return from subroutine)\n");
			break;
		default:
			break;
	}
}

void determine_1_opcode(uint16_t inst)
{
	uint16_t addr = inst & 0x0FFF;
	printf("1NNN -> JMP %04X (Jump to address)\n", addr);
}

void determine_2_opcode(uint16_t inst)
{
	uint16_t subroutine = inst & 0x0FFF;
	printf("2NNN -> CALL %04X (Call subroutine)\n", subroutine);
}

void determine_3_opcode(uint16_t inst)
{
	unsigned char x = (inst & 0x0F00) >> 8;
	unsigned char nn = inst & 0x00FF;
	printf("3XNN -> Skip next inst if V%X == %02X\n", x, nn);
}

void determine_4_opcode(uint16_t inst)
{
	unsigned char x = (inst & 0x0F00) >> 8;
	unsigned char nn = inst & 0x00FF;
	printf("4XNN -> Skip next inst if V%X != %02X\n", x, nn);
}

void determine_5_opcode(uint16_t inst)
{
	unsigned char x = (inst & 0x0F00) >> 8;
	unsigned char y = (inst & 0x00F0) >> 4;
	printf("5XY0 -> Skip next inst if V%X == V%X\n", x, y);
}

void determine_6_opcode(uint16_t inst)
{
	unsigned char x = (inst & 0x0F00) >> 8;
	unsigned char nn = inst & 0x00FF;
	printf("6XNN -> Set V%X to %02X\n", x, nn);
}

void determine_7_opcode(uint16_t inst)
{
	unsigned char x = (inst & 0x0F00) >> 8;
	unsigned char nn = inst & 0x00FF;
	printf("7XNN -> ADD V%X, %02X (Adds NN to VX)\n", x, nn);
}

void determine_8_opcode(uint16_t inst)
{
	unsigned char x = (inst & 0x0F00) >> 8;
	unsigned char y = (inst & 0x00F0) >> 4;

	switch(inst & 0x000F)
	{
		case 0x0:
			printf("8XY0 -> MOV V%X, V%X (Sets VX to VY)\n", x, y);
			break;
		case 0x1:
			printf("8XY1 -> OR V%X, V%X (Sets VX to VX OR VY)\n", x, y);
			break;
		case 0x2:
			printf("8XY2 -> AND V%X, V%X (Sets VX to VX AND VY)\n", x, y);
			break;
		case 0x3:
			printf("8XY3 -> XOR V%X, V%X (Sets VX to VX XOR VY)\n", x, y);
			break;
		case 0x4:
			printf("8XY4 -> ADD V%X, V%X (Sets VX to VX + VY, VF set 1 one if there is a carry)\n", x, y);
			break;
		case 0x5:
			printf("8XY5 -> SUB V%X, V%X (Sets VX to VX - VY)\n", x, y);
			break;
		case 0x6:
			printf("8XY6 -> STOR LSB of V%X in VF, then shift V%X right by one\n", x, x);
			break;
		case 0x7:
			printf("8XY7 -> V%X = V%X - V%X (Sets VX to VY - VX)\n", x, y, x);
			break;
		case 0xE:
			printf("8XYE -> STOR MSB of V%X in VF, then shift V%X to left by one\n", x, x);
			break;
		default:
			break;
	}
}

void determine_9_opcode(uint16_t inst)
{
	unsigned char x = (inst & 0x0F00) >> 8;
	unsigned char y = (inst & 0x00F0) >> 4;

	switch(inst & 0xF00F)
	{
		case 0x9000:
			printf("9XY0 -> Jump to next instruction if V%X != V%X\n", x, y);
			break;
		default:
			break;
	}
}

void determine_A_opcode(uint16_t inst)
{
	printf("ANNN -> MOV I, %04X (Sets I register to address NNN)\n", (inst & 0x0FFF));
}

void determine_B_opcode(uint16_t inst)
{
	printf("BNNN -> PC = V0 + %04X (Sets PC to V0 + address NNN)\n", (inst & 0x0FFF));
}

void determine_C_opcode(uint16_t inst)
{
	unsigned char x = (inst & 0x0F00) >> 8;
	unsigned char nn = (inst & 0x00FF);

	printf("CXNN -> V%X = rand() & %02X (Sets VX to random num AND NN)\n", x, nn);
}

void determine_D_opcode(uint16_t inst)
{
	unsigned char x = (inst & 0x0F00) >> 8;
	unsigned char y = (inst & 0x00F0) >> 4;
	unsigned char n = inst & 0x000F;
	printf("DXYN -> Draw(V%X, V%X, %X) (Draw sprite at coordinate VX, VY that has width of 8 pixels and height of N pixels)\n", x, y, n);
}

void determine_E_opcode(uint16_t inst)
{
	unsigned char x = (inst & 0x0F00) >> 8;

	switch(inst & 0xF0FF)
	{
		case 0xE09E:
			printf("EX9E -> PC + 1 if key==V%X (Skips next instruction if key()==VX)\n", x);
			break;
		case 0xE0A1:
			printf("EXA1 -> PC + 1 if key!=V%X (Skips next instruction if key()!=VX)\n", x);
			break;
	}
}

void determine_F_opcode(uint16_t inst)
{
	switch(inst & 0x00FF)
	{
		case 0x07:
			break;
		case 0x0A:
			break;
		case 0x15:
			break;
		case 0x18:
			break;
		case 0x1E:
			break;
		case 0x29:
			break;
		case 0x33:
			break;
		case 0x55:
			break;
		case 0x65:
			break;
	}
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

	read_instructions(&chip);
}

