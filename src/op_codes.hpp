#ifndef OP_CODES_HPP
#define OP_CODES_HPP

#include <iostream>
#include <sstream>
#include "chip-8_state.hpp"
#include "input/input_interface.hpp"

static int DEFAULT_OP_CYCLES = 1;
static int BLOCKING_CALL = -1;
static uint8_t REGISTER_VF = 15;

/**
 * @brief Executes the 0x00E0 op code on the chip state
 *
 * 0x00E0 Clears the screen
 *
 * @param state Current chip state
 *
 * @return The number of cycles needed to perform the operation
 */
int Execute00E0(CHIP8_State* state);

/**
 * @brief Executes the 0x00EE op code on the chip state
 *
 * 0x00EE Returns the CHIP-8 from a subroutine
 *
 * @param state Current chip state
 *
 * @return The number of cycles needed to perform the operation
 */
int Execute00EE(CHIP8_State* state);

/**
 * @brief Executes the 0x1NNN op code on the chip state
 *
 * 0x1NNN Jumps the processor to the address at NNN
 *
 * @param state Current chip state
 *
 * @return The number of cycles needed to perform the operation
 */
int Execute1NNN(CHIP8_State* state, uint16_t op_code);

/**
 * @brief Executes the 0x2NNN op code on the chip state
 *
 * 0x2NNN - Calls subroutine at NNN
 *
 * @param state Current chip state
 *
 * @return The number of cycles needed to perform the operation
 */
int Execute2NNN(CHIP8_State* state, uint16_t op_code);

/**
 * @brief Executes the 0x3XNN op code on the chip state
 *
 * 0x3XNN - Skips the next instruction if VX equals NN
 *
 * @param state Current chip state
 * @param op_code The op code
 *
 * @return The number of cycles needed to perform the operation
 */
int Execute3XNN(CHIP8_State* state, uint16_t op_code);

/**
 * @brief Executes the 0x4XNN op code on the chip state
 *
 * 0x4XNN - Skips the next instruction if VX does not equal NN
 *
 * @param state Current chip state
 *
 * @return The number of cycles needed to perform the operation
 */
int Execute4XNN(CHIP8_State* state, uint16_t op_code);

/**
 * @brief Executes the 0x5XY0 op code on the chip state
 *
 * 0x5XY0 - Skips the next instruction if VX equals VY.
 *
 * @param state Current chip state
 *
 * @return The number of cycles needed to perform the operation
 */
int Execute5XY0(CHIP8_State* state, uint16_t op_code);

/**
 * @brief Executes the 0x6XNN op code on the chip state
 *
 * 0x6XNN - Sets VX to NN
 *
 * @param state Current chip state
 * @param op_code The op code to execute
 *
 * @return The number of cycles needed to perform the operation
 */
int Execute6XNN(CHIP8_State* state, uint16_t op_code);

/**
 * @brief Executes the 0x7XNN op code on the chip state
 *
 * 0x7XNN - Adds NN to VX. (Carry flag is not changed)
 *
 * @param state Current chip state
 * @param op_code The op code to execute
 *
 * @return The number of cycles needed to perform the operation
 */
int Execute7XNN(CHIP8_State* state, uint16_t op_code);

/**
 * @brief Executes the 0x8XY0 op code on the chip state
 *
 * 0x8XY0 - Sets VX to the value of VY.
 *
 * @param state Current chip state
 * @param op_code The op code to execute
 *
 * @return The number of cycles needed to perform the operation
 */
int Execute8XY0(CHIP8_State* state, uint16_t op_code);

/**
 * @brief Executes the 0x8XY1 op code on the chip state
 *
 * 0x8XY1 - Sets VX to VX or VY.
 *
 * @param state Current chip state
 * @param op_code The op code to execute
 *
 * @return The number of cycles needed to perform the operation
 */
int Execute8XY1(CHIP8_State* state, uint16_t op_code);

/**
 * @brief Executes the 0x8XY2 op code on the chip state
 *
 * 0x8XY2 - Sets VX to VX and VY.
 *
 * @param state Current chip state
 * @param op_code The op code to execute
 *
 * @return The number of cycles needed to perform the operation
 */
int Execute8XY2(CHIP8_State* state, uint16_t op_code);

/**
 * @brief Executes the 0x8XY3 op code on the chip state
 *
 * 0x8XY3 - Sets VX to VX xor VY.
 *
 * @param state Current chip state
 * @param op_code The op code to execute
 *
 * @return The number of cycles needed to perform the operation
 */
int Execute8XY3(CHIP8_State* state, uint16_t op_code);

/**
 * @brief Executes the 0x8XY4 op code on the chip state
 *
 * 0x8XY4 - Adds VY to VX.
 * VF is set to 1 when there's a carry, and to 0 when there isn't.
 *
 * @param state Current chip state
 * @param op_code The op code to execute
 *
 * @return The number of cycles needed to perform the operation
 */
int Execute8XY4(CHIP8_State* state, uint16_t op_code);

/**
 * @brief Executes the 0x8XY5 op code on the chip state
 *
 * 0x8XY5 - VY is subtracted from VX.
 * VF is set to 0 when there's a borrow, and 1 when there isn't.
 *
 * @param state Current chip state
 * @param op_code The op code to execute
 *
 * @return The number of cycles needed to perform the operation
 */
int Execute8XY5(CHIP8_State* state, uint16_t op_code);

/**
 * @brief Executes the 0x8XY6 op code on the chip state
 *
 * 0x8XY6 - Stores the least significant bit of VX in VF and then shifts VX to the right by 1.
 *
 * @param state Current chip state
 * @param op_code The op code to execute
 *
 * @return The number of cycles needed to perform the operation
 */
int Execute8XY6(CHIP8_State* state, uint16_t op_code);

/**
 * @brief Executes the 0x8XY7 op code on the chip state
 *
 * Set Vx = Vy - Vx, set VF = NOT borrow.
 * If Vy > Vx, then VF is set to 1, otherwise 0.
 * Then Vx is subtracted from Vy, and the results stored in Vx.
 *
 * @param state Current chip state
 * @param op_code The op code to execute
 *
 * @return The number of cycles needed to perform the operation
 */
int Execute8XY7(CHIP8_State* state, uint16_t op_code);

/**
 * @brief Executes the 0x8XYE op code on the chip state
 *
 * 0x8XYE - Stores the most significant bit of VX in VF and then shifts VX to the left by 1.
 *
 * @param state Current chip state
 * @param op_code The op code to execute
 *
 * @return The number of cycles needed to perform the operation
 */
int Execute8XYE(CHIP8_State* state, uint16_t op_code);

/**
 * @brief Executes the 0x9XY0 op code on the chip state
 *
 * 0x9XY0 - Skips the next instruction if VX doesn't equal VY.
 * (Usually the next instruction is a jump to skip a code block)
 *
 * @param state Current chip state
 * @param op_code The op code to execute
 *
 * @return The number of cycles needed to perform the operation
 */
int Execute9XY0(CHIP8_State* state, uint16_t op_code);

/**
 * @brief Executes the 0xANNN op code on the chip state
 *
 * 0xANNN - Sets I (index register) to the address NNN.
 *
 * @param state Current chip state
 * @param op_code The op code to execute
 *
 * @return The number of cycles needed to perform the operation
 */
int ExecuteANNN(CHIP8_State* state, uint16_t op_code);

/**
 * @brief Executes the 0xBNNN op code on the chip state
 *
 * 0xBNNN - Jumps to the address NNN plus V0.
 *
 * @param state Current chip state
 * @param op_code The op code to execute
 *
 * @return The number of cycles needed to perform the operation
 */
int ExecuteBNNN(CHIP8_State* state, uint16_t op_code);

/**
 * @brief Executes the 0xCNNN op code on the chip state
 *
 * 0xCNNN - Sets VX to the result of a bitwise and operation on a random number and NN
 * (Typically: 0 to 255)
 * @param state Current chip state
 * @param op_code The op code to execute
 *
 * @return The number of cycles needed to perform the operation
 */
int ExecuteCNNN(CHIP8_State* state, uint16_t op_code);

/**
 * @brief Executes the 0xDXYN op code on the chip state
 *
 * 0xDXYN - Draws a sprite at coordinate (VX, VY) that has a width of 8 pixels and a height of N pixels.
 * Each row of 8 pixels is read as bit-coded starting from memory location I;
 * I value doesn’t change after the execution of this instruction. As described above, VF is set to 1 if any screen
 *  pixels are flipped from set to unset when the sprite is drawn, and to 0 if that doesn’t happen
 *
 * @param state Current chip state
 * @param op_code The op code to execute
 *
 * @return The number of cycles needed to perform the operation
 */
int ExecuteDXYN(CHIP8_State* state, uint16_t op_code);

/**
 * @brief Executes the 0xEX9E op code on the chip state
 *
 * 0xEX9E -Skips the next instruction if the key stored in VX is pressed.
 * (Usually the next instruction is a jump to skip a code block).
 *
 * @param state Current chip state
 * @param op_code The op code to execute
 * @param input The input interface used to retrieve info on what keys are pressed
 *
 * @return The number of cycles needed to perform the operation
 */
int ExecuteEX9E(CHIP8_State* state, uint16_t op_code, InputInterface* input);

/**
 * @brief Executes the 0xEXA1 op code on the chip state
 *
 * 0xEXA1 -Skips the next instruction if the key stored in VX isn't pressed.
 * (Usually the next instruction is a jump to skip a code block).
 *
 * @param state Current chip state
 * @param op_code The op code to execute
 * @param input The input interface used to retrieve info on what keys are pressed
 *
 * @return The number of cycles needed to perform the operation
 */
int ExecuteEXA1(CHIP8_State* state, uint16_t op_code, InputInterface* input);

/**
 * @brief Executes the 0xFX07 op code on the chip state
 *
 * 0xFX07 -Sets VX to the value of the delay timer.
 *
 * @param state Current chip state
 * @param op_code The op code to execute
 *
 * @return The number of cycles needed to perform the operation
 */
int ExecuteFX07(CHIP8_State* state, uint16_t op_code);

/**
 * @brief Executes the 0xFX0A op code on the chip state
 *
 * 0xFX0A - A key press is awaited, and then stored in VX.
 * (Blocking Operation. All instruction halted until next key event)
 *
 * @param state Current chip state
 * @param op_code The op code to execute
 *
 * @return The number of cycles needed to perform the operation
 */
int ExecuteFX0A(CHIP8_State* state, uint16_t op_code, InputInterface* input);

/**
 * @brief Executes the 0xFX15 op code on the chip state
 *
 * 0xFX15 - Sets the delay timer to VX.
 *
 * @param state Current chip state
 * @param op_code The op code to execute
 *
 * @return The number of cycles needed to perform the operation
 */
int ExecuteFX15(CHIP8_State* state, uint16_t op_code);

/**
 * @brief Executes the 0xFX18 op code on the chip state
 *
 * 0xFX18 - Sets the sound timer to VX.
 *
 * @param state Current chip state
 * @param op_code The op code to execute
 *
 * @return The number of cycles needed to perform the operation
 */
int ExecuteFX18(CHIP8_State* state, uint16_t op_code);

/**
 * @brief Executes the 0xFX1E op code on the chip state
 *
 * 0xFX1E - Adds VX to I.
 *
 * @param state Current chip state
 * @param op_code The op code to execute
 *
 * @return The number of cycles needed to perform the operation
 */
int ExecuteFX1E(CHIP8_State* state, uint16_t op_code);

/**
 * @brief Executes the 0xFX29 op code on the chip state
 *
 * 0xFX29 - Sets I to the location of the sprite for the character in VX.
 * Characters 0-F (in hexadecimal) are represented by a 4x5 font.
 *
 * @param state Current chip state
 * @param op_code The op code to execute
 *
 * @return The number of cycles needed to perform the operation
 */
int ExecuteFX29(CHIP8_State* state, uint16_t op_code);

/**
 * @brief Executes the 0xFX33 op code on the chip state
 *
 * 0xFX33 - Stores the binary-coded decimal representation of VX,
 * with the most significant of three digits at the address in I,
 * the middle digit at I plus 1, and the least significant digit at I plus 2.
 * (In other words, take the decimal representation of VX,
 * place the hundreds digit in memory at location in I,
 * the tens digit at location I+1, and the ones digit at location I+2.)
 *
 * @param state Current chip state
 * @param op_code The op code to execute
 *
 * @return The number of cycles needed to perform the operation
 */
int ExecuteFX33(CHIP8_State* state, uint16_t op_code);

/**
 * @brief Executes the 0xFX55 op code on the chip state
 *
 * 0xFX55 - Stores V0 to VX (including VX) in memory starting at address I.
 * The offset from I is increased by 1 for each value written.
 *
 * @param state Current chip state
 * @param op_code The op code to execute
 *
 * @return The number of cycles needed to perform the operation
 */
int ExecuteFX55(CHIP8_State* state, uint16_t op_code);

/**
 * @brief Executes the 0xFX65 op code on the chip state
 *
 * 0xFX65 - Fills V0 to VX (including VX) with values from memory starting at address I.
 * The offset from I is increased by 1 for each value written.
 *
 * @param state Current chip state
 * @param op_code The op code to execute
 *
 * @return The number of cycles needed to perform the operation
 */
int ExecuteFX65(CHIP8_State* state, uint16_t op_code);

#endif