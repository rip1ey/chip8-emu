#ifndef CHIP8
#define CHIP8

#include "chip8_util.h"

void print_emu_usage();
void exec_0_op(uint16_t inst, chip8 *chip);
void exec_1_op(uint16_t inst, chip8 *chip);
void exec_2_op(uint16_t inst, chip8 *chip);
void exec_3_op(uint16_t inst, chip8 *chip);
void exec_4_op(uint16_t inst, chip8 *chip);
void exec_5_op(uint16_t inst, chip8 *chip);
void exec_6_op(uint16_t inst, chip8 *chip);
void exec_7_op(uint16_t inst, chip8 *chip);
void exec_8_op(uint16_t inst, chip8 *chip);
void exec_9_op(uint16_t inst, chip8 *chip);
void exec_A_op(uint16_t inst, chip8 *chip);
void exec_B_op(uint16_t inst, chip8 *chip);
void exec_C_op(uint16_t inst, chip8 *chip);
void exec_D_op(uint16_t inst, chip8 *chip);
void exec_E_op(uint16_t inst, chip8 *chip);
void exec_F_op(uint16_t inst, chip8 *chip);

#endif
