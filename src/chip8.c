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
			printf("00E0\n");
			break;
		case 0x00EE:
			printf("00EE\n");
			break;
		default:
			printf("Unrecognized instruction\n");
			break;
	}
}

void exec_1_op(uint16_t inst, chip8* chip)
{

}

void exec_2_op(uint16_t inst, chip8* chip)
{

}

void exec_3_op(uint16_t inst, chip8* chip)
{
}

void exec_4_op(uint16_t inst, chip8* chip)
{

}

void exec_5_op(uint16_t inst, chip8* chip)
{

}

void exec_6_op(uint16_t inst, chip8* chip)
{
	printf("Op 6\n");
}

void exec_7_op(uint16_t inst, chip8* chip)
{

}

void exec_8_op(uint16_t inst, chip8* chip)
{

}

void exec_9_op(uint16_t inst, chip8* chip)
{

}

void exec_A_op(uint16_t inst, chip8* chip)
{
	printf("Executing A opcode\n");
}

void exec_B_op(uint16_t inst, chip8* chip)
{

}

void exec_C_op(uint16_t inst, chip8* chip)
{

}

void exec_D_op(uint16_t inst, chip8* chip)
{

}

void exec_E_op(uint16_t inst, chip8* chip)
{

}

void exec_F_op(uint16_t inst, chip8* chip)
{

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

	// cuts down on the giant switch statement
	void (*func_arr[])(uint16_t, chip8*) = {
												exec_0_op, exec_1_op, exec_2_op, exec_3_op,
												exec_4_op, exec_5_op, exec_6_op, exec_7_op,
												exec_8_op, exec_9_op, exec_A_op, exec_B_op,
												exec_C_op, exec_D_op, exec_E_op, exec_F_op,
										   };

	exit_chip8 = -2;
	while(exit_chip8 != 3)
	{
		// fetch
		uint8_t nibble = chip.memory[chip.pc] >> 4; 
		uint16_t inst = chip.memory[chip.pc] << 8 | chip.memory[chip.pc + 1]; 
		printf("Nibble: %02X\n", nibble);
		printf("Instruction: %04X\n", inst);
		func_arr[nibble](inst, &chip);
		exit_chip8++;
		chip.pc+=2;
	}
}
