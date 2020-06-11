#include <bitset>
#include <filesystem>
#include <iostream>
#include <sstream>
#include<vector>
#include "../src/chip-8_state.hpp"
#include "../src/op_codes.hpp"
#include "../src/display/display_interface.hpp"

using namespace std;

/**
 * @brief Ensures that the execution of op code 0x00E0 clears all display bits
 *
 */
void test00E0() {
    uint8_t* memory = new uint8_t[RAM_SIZE];
    uint8_t* v_registers = new uint8_t[V_REGISTER_COUNT];
    CHIP8_State* chip_8_state = new CHIP8_State(0x200, 0x200, 0, 0, v_registers, memory);

    // Activate all display pixels
    for (int j = 0; j < DISPLAY_HEIGHT; j++) {
        for (int i = 0; i < DISPLAY_WIDTH; i++) {
            chip_8_state->setDisplayValue(i, j, true);
        }
    }

    // Clear the screen
    Execute00E0(chip_8_state);

    // Assert all display bits are clear
    for (int j = 0; j < DISPLAY_HEIGHT; j++) {
        for (int i = 0; i < DISPLAY_WIDTH; i++) {
            assert(chip_8_state->displayValue(i, j) == false);
        }
    }

    delete chip_8_state;
}

/**
 * @brief Tests the op command used to return the program from a sub-routine
 *
 */
void test00EE() {
    uint8_t* memory = new uint8_t[RAM_SIZE];
    uint8_t* v_registers = new uint8_t[V_REGISTER_COUNT];

    uint16_t initial_pc = INITAL_PROGRAM_COUNTER;
    uint16_t stack_pc = initial_pc + 8;

    CHIP8_State* chip_8_state = new CHIP8_State(initial_pc, 0, 0, 0, v_registers, memory);
    chip_8_state->pushStack(stack_pc);

    Execute00EE(chip_8_state);

    assert(chip_8_state->programCounter() == stack_pc);
    assert(chip_8_state->stackPointer() == -2);
}

/**
 * @brief Test for op code used to
 *
 */
void test1NNN() {
    uint8_t* memory = new uint8_t[RAM_SIZE];
    uint8_t* v_registers = new uint8_t[V_REGISTER_COUNT];

    uint16_t initial_pc = INITAL_PROGRAM_COUNTER;
    uint16_t jump_address = 0x400;

    CHIP8_State* chip_8_state = new CHIP8_State(initial_pc, 0, 0, 0, v_registers, memory);

    Execute1NNN(chip_8_state, 0x1400);

    assert(chip_8_state->programCounter() == jump_address);
}

/**
 * @brief Test for the op code that calls a sub routine
 *
 */
void test2NNN() {
    uint8_t* memory = new uint8_t[RAM_SIZE];
    uint8_t* v_registers = new uint8_t[V_REGISTER_COUNT];

    uint16_t initial_pc = INITAL_PROGRAM_COUNTER;
    uint16_t sub_routine_address = 0x400;

    CHIP8_State* chip_8_state = new CHIP8_State(initial_pc, 0, 0, 0, v_registers, memory);

    Execute2NNN(chip_8_state, 0x1400);

    // Assert that the subroutine address is the program counter
    assert(chip_8_state->programCounter() == sub_routine_address);

    // Assert that the stack has been incremented once
    assert(chip_8_state->stackPointer() == 0);

    // Assert that the top of the call stack is the initial PC
    assert(chip_8_state->peekStack() == initial_pc);

}

/**
 * @brief Test for op code 3XNN. 0x3XNN - Skips the next instruction if VX equals NN
 *
 */
void test3XNN() {
    uint8_t* memory = new uint8_t[RAM_SIZE];
    uint8_t* v_registers = new uint8_t[V_REGISTER_COUNT];

    uint8_t v0 = 0x40;
    v_registers[0] = v0;

    uint16_t initial_pc = INITAL_PROGRAM_COUNTER;

    CHIP8_State* chip_8_state = new CHIP8_State(initial_pc, 0, 0, 0, v_registers, memory);

    // V0 does not equal 0x00, no change in PC
    Execute3XNN(chip_8_state, 0x3000);
    // Assert that the subroutine address is the program counter
    assert(chip_8_state->programCounter() == initial_pc);

    // V0 equals 0x40, PC incremented by 2
    Execute3XNN(chip_8_state, 0x3040);
    // Assert that the subroutine address is the program counter
    assert(chip_8_state->programCounter() == (initial_pc + 2));
}

/**
 * @brief Tests the 4XNN op code
 *
 * 0x4XNN - Skips the next instruction if VX does not equal NN
 *
 */
void test4XNN() {
    uint8_t* memory = new uint8_t[RAM_SIZE];
    uint8_t* v_registers = new uint8_t[V_REGISTER_COUNT];

    uint8_t v0 = 0x40;
    v_registers[0] = v0;

    uint16_t initial_pc = INITAL_PROGRAM_COUNTER;

    CHIP8_State* chip_8_state = new CHIP8_State(initial_pc, 0, 0, 0, v_registers, memory);

    // V0 equals 0x40, no change in PC
    Execute4XNN(chip_8_state, 0x4040);
    // Assert that the subroutine address is the program counter
    assert(chip_8_state->programCounter() == initial_pc);

    // V0 does not equal 0x00, PC incremented by 2
    Execute4XNN(chip_8_state, 0x4000);
    // Assert that the subroutine address is the program counter
    assert(chip_8_state->programCounter() == (initial_pc + 2));
}

/**
 * @brief Tests op code 5XY0
 *
 * 0x5XY0 - Skips the next instruction if VX equals VY.
 */
void test5XY0() {
    uint8_t* memory = new uint8_t[RAM_SIZE];
    uint8_t* v_registers = new uint8_t[V_REGISTER_COUNT];

    uint8_t v0 = 0x40;
    uint8_t v1 = 0x40;
    uint8_t v2 = 0x00;

    v_registers[0] = v0;
    v_registers[1] = v1;
    v_registers[2] = v2;

    uint16_t initial_pc = INITAL_PROGRAM_COUNTER;

    CHIP8_State* chip_8_state = new CHIP8_State(initial_pc, 0, 0, 0, v_registers, memory);

    // V0 does not equal v1, no change in PC
    Execute5XY0(chip_8_state, 0x5020);
    // Assert that the subroutine address is the program counter
    assert(chip_8_state->programCounter() == initial_pc);

    // v0 equals v2, PC incremented by 2
    Execute5XY0(chip_8_state, 0x5010);
    // Assert that the subroutine address is the program counter
    assert(chip_8_state->programCounter() == (initial_pc + 2));
}

/**
 * @brief tests the 0x6XNN op code execution on the chip state
 *
 * 0x6XNN - Sets VX to NN
 *
 */
void test6XNN() {
    uint8_t* memory = new uint8_t[RAM_SIZE];
    uint8_t* v_registers = new uint8_t[V_REGISTER_COUNT];

    uint8_t new_v0 = 0x40;
    uint8_t new_v15 = 0xFF;

    CHIP8_State* chip_8_state = new CHIP8_State(INITAL_PROGRAM_COUNTER, 0, 0, 0, v_registers, memory);

    // Set V0 to 0x40
    Execute6XNN(chip_8_state, 0x6040);
    assert(chip_8_state->vRegister(0) == new_v0);

    // Set V15 to 0xFF
    Execute6XNN(chip_8_state, 0x6FFF);
    assert(chip_8_state->vRegister(15) == new_v15);
}

/**
 * @brief Tests the 0x7XNN op code on the chip state
 *
 * 0x7XNN - Adds NN to VX. (Carry flag is not changed)
 */
void test7XNN() {
    uint8_t* memory = new uint8_t[RAM_SIZE];
    uint8_t* v_registers = new uint8_t[V_REGISTER_COUNT];

    uint8_t v0v1 = 0xF8;

    CHIP8_State* chip_8_state = new CHIP8_State(INITAL_PROGRAM_COUNTER, 0, 0, 0, v_registers, memory);
    chip_8_state->setVRegister(0, v0v1);
    chip_8_state->setVRegister(1, v0v1);

    // Add 0x07 to V0
    Execute7XNN(chip_8_state, 0x7007);
    // Add 0x08 to V1
    Execute7XNN(chip_8_state, 0x7108);
    // v0 should not have overflown, set to 0xFF
    assert(chip_8_state->vRegister(0) == 0xFF);
    // Overflow, no flag set. Will be zero
    assert(chip_8_state->vRegister(1) == 0x00);
}

/**
 * @brief Tests the 0x8XY0 op code on the chip state
 *
 * 0x8XY0 - Sets VX to the value of VY.
 *
 */
void test8XY0() {
    uint8_t* memory = new uint8_t[RAM_SIZE];
    uint8_t* v_registers = new uint8_t[V_REGISTER_COUNT];

    uint8_t v14 = 0xF8;
    uint8_t v5 = 0x3B;

    CHIP8_State* chip_8_state = new CHIP8_State(INITAL_PROGRAM_COUNTER, 0, 0, 0, v_registers, memory);
    chip_8_state->setVRegister(14, v14);
    chip_8_state->setVRegister(5, v5);

    // Set V14 to the value of V5
    Execute8XY0(chip_8_state, 0x8E50);

    assert(chip_8_state->vRegister(14) == v5);
    assert(chip_8_state->vRegister(5) == v5);
}

/**
 * @brief Tests the 0x8XY1 op code on the chip state
 *
 * 0x8XY1 - Sets VX to VX or VY.
 */
void test8XY1() {
    uint8_t* memory = new uint8_t[RAM_SIZE];
    uint8_t* v_registers = new uint8_t[V_REGISTER_COUNT];

    uint8_t v9 = 0xAA;
    uint8_t v3 = 0xC1;

    uint8_t v3_or_v9 = v3 | v9;

    CHIP8_State* chip_8_state = new CHIP8_State(INITAL_PROGRAM_COUNTER, 0, 0, 0, v_registers, memory);
    chip_8_state->setVRegister(9, v9);
    chip_8_state->setVRegister(3, v3);

    // Set V14 to the value of V5
    Execute8XY1(chip_8_state, 0x8931);

    assert(chip_8_state->vRegister(9) == v3_or_v9);
    assert(chip_8_state->vRegister(3) == v3);
}

/**
 * @brief Tests the 0x8XY2 op code on the chip state
 *
 * 0x8XY2 - Sets VX to VX and VY.
 *
 */
void test8XY2() {
    uint8_t* memory = new uint8_t[RAM_SIZE];
    uint8_t* v_registers = new uint8_t[V_REGISTER_COUNT];

    uint8_t v9 = 0xAA;
    uint8_t v3 = 0xC1;

    uint8_t v3_and_v9 = v3 & v9;

    CHIP8_State* chip_8_state = new CHIP8_State(INITAL_PROGRAM_COUNTER, 0, 0, 0, v_registers, memory);
    chip_8_state->setVRegister(9, v9);
    chip_8_state->setVRegister(3, v3);

    // Set V14 to the value of V5
    Execute8XY2(chip_8_state, 0x8932);

    assert(chip_8_state->vRegister(9) == v3_and_v9);
    assert(chip_8_state->vRegister(3) == v3);
}

/**
 * @brief Tests the 0x8XY3 op code on the chip state
 *
 * 0x8XY3 - Sets VX to VX xor VY.
 *
 */
void test8XY3() {
    uint8_t* memory = new uint8_t[RAM_SIZE];
    uint8_t* v_registers = new uint8_t[V_REGISTER_COUNT];

    uint8_t v9 = 0xAA;
    uint8_t v3 = 0xC1;

    uint8_t v3_xor_v9 = v3 ^ v9;

    CHIP8_State* chip_8_state = new CHIP8_State(INITAL_PROGRAM_COUNTER, 0, 0, 0, v_registers, memory);
    chip_8_state->setVRegister(9, v9);
    chip_8_state->setVRegister(3, v3);

    // Set V14 to the value of V5
    Execute8XY3(chip_8_state, 0x8933);

    assert(chip_8_state->vRegister(9) == v3_xor_v9);
    assert(chip_8_state->vRegister(3) == v3);
}

/**
 * @brief Executes the 0x8XY4 op code on the chip state
 *
 * 0x8XY4 - Adds VY to VX.
 * VF is set to 1 when there's a carry, and to 0 when there isn't.
 *
 */
void test8XY4() {
    uint8_t* memory = new uint8_t[RAM_SIZE];
    uint8_t* v_registers = new uint8_t[V_REGISTER_COUNT];

    uint8_t v0 = 0x05;
    uint8_t v1 = 0xC1;
    uint8_t v2 = 0xFF;
    uint8_t v3 = 0x02;

    CHIP8_State* chip_8_state = new CHIP8_State(INITAL_PROGRAM_COUNTER, 0, 0, 0, v_registers, memory);
    chip_8_state->setVRegister(0, v0);
    chip_8_state->setVRegister(1, v1);
    chip_8_state->setVRegister(2, v2);
    chip_8_state->setVRegister(3, v3);
    chip_8_state->setVRegister(15, 0);

    // V1 is added to V0
    Execute8XY4(chip_8_state, 0x8014);
    assert(chip_8_state->vRegister(0) == (v0 + v1));
    assert(chip_8_state->vRegister(1) == v1);
    // No carryover
    assert(chip_8_state->vRegister(15) == 0);

    // V3 is added to V2
    Execute8XY4(chip_8_state, 0x8234);
    assert(chip_8_state->vRegister(2) == (uint8_t)(v2 + v3));
    assert(chip_8_state->vRegister(3) == v3);
    // Carryover expected
    assert(chip_8_state->vRegister(15) == 1);
}

/**
 * @brief Executes the 0x8XY5 op code on the chip state
 *
 * 0x8XY5 - VY is subtracted from VX.
 * VF is set to 0 when there's a borrow, and 1 when there isn't.
 */
void test8XY5() {
    uint8_t* memory = new uint8_t[RAM_SIZE];
    uint8_t* v_registers = new uint8_t[V_REGISTER_COUNT];

    uint8_t v0 = 0x05;
    uint8_t v1 = 0xC5;
    uint8_t v2 = 0x01;
    uint8_t v3 = 0x00;

    CHIP8_State* chip_8_state = new CHIP8_State(INITAL_PROGRAM_COUNTER, 0, 0, 0, v_registers, memory);
    chip_8_state->setVRegister(0, v0);
    chip_8_state->setVRegister(1, v1);
    chip_8_state->setVRegister(2, v2);
    chip_8_state->setVRegister(3, v3);
    chip_8_state->setVRegister(15, 0);

    // V1 is added to V0
    Execute8XY5(chip_8_state, 0x8105);
    assert(chip_8_state->vRegister(1) == (uint8_t)(v1 - v0));
    assert(chip_8_state->vRegister(0) == v0);
    // No borrow
    assert(chip_8_state->vRegister(15) == 0);

    // V3 is added to V2
    Execute8XY5(chip_8_state, 0x8325);
    assert(chip_8_state->vRegister(3) == (uint8_t)(v3 - v2));
    assert(chip_8_state->vRegister(2) == v2);
    // There should be a borrow here
    assert(chip_8_state->vRegister(15) == 1);
}

/**
 * @brief Executes the 0x8XY6 op code on the chip state
 *
 * 0x8XY6 - Stores the least significant bit of VX in VF and then shifts VX to the right by 1.
 *
 */
void test8XY6() {
    uint8_t* memory = new uint8_t[RAM_SIZE];
    uint8_t* v_registers = new uint8_t[V_REGISTER_COUNT];

    uint8_t v0 = 0b11100010;

    CHIP8_State* chip_8_state = new CHIP8_State(INITAL_PROGRAM_COUNTER, 0, 0, 0, v_registers, memory);
    chip_8_state->setVRegister(0, v0);
    chip_8_state->setVRegister(15, 0);

    Execute8XY6(chip_8_state, 0x8006);
    assert(chip_8_state->vRegister(0) == 0b01110001);
    assert(chip_8_state->vRegister(15) == 0b00000000);
}

/**
 * @brief Tests the 0x8XY7 op code on the chip state
 *
 * 0x8XY7 - VX is set to VY minus VX. No borrow
 * If Vx > Vy, then VF is set to 1, otherwise 0.
 * Then Vy is subtracted from Vx, and the results stored in Vx.
 *
 */
void test8XY7() {
    uint8_t* memory = new uint8_t[RAM_SIZE];
    uint8_t* v_registers = new uint8_t[V_REGISTER_COUNT];

    uint8_t v0 = 0x05;
    uint8_t v1 = 0xC5;
    uint8_t v2 = 0x01;
    uint8_t v3 = 0x00;

    CHIP8_State* chip_8_state = new CHIP8_State(INITAL_PROGRAM_COUNTER, 0, 0, 0, v_registers, memory);
    chip_8_state->setVRegister(0, v0);
    chip_8_state->setVRegister(1, v1);
    chip_8_state->setVRegister(2, v2);
    chip_8_state->setVRegister(3, v3);
    chip_8_state->setVRegister(15, 0);

    // V1 == VX, V0 == VY.
    Execute8XY7(chip_8_state, 0x8107);
    assert(chip_8_state->vRegister(1) == (uint8_t)(v0 - v1));
    assert(chip_8_state->vRegister(0) == v0);
    // 0xC5 > 0x05
    assert(chip_8_state->vRegister(15) == 1);

    // V3 is added to V2
    Execute8XY7(chip_8_state, 0x8327);
    assert(chip_8_state->vRegister(3) == (uint8_t)(v2 - v3));
    assert(chip_8_state->vRegister(2) == v2);
    // 0x00 ! > 0x01
    assert(chip_8_state->vRegister(15) == 0);
}

/**
 * @brief Executes the 0x8XYE op code on the chip state
 *
 * 0x8XYE - Stores the most significant bit of VX in VF and then shifts VX to the left by 1.
 *
 */
void test8XYE() {
    uint8_t* memory = new uint8_t[RAM_SIZE];
    uint8_t* v_registers = new uint8_t[V_REGISTER_COUNT];

    uint8_t v0 = 0b11100010;

    CHIP8_State* chip_8_state = new CHIP8_State(INITAL_PROGRAM_COUNTER, 0, 0, 0, v_registers, memory);
    chip_8_state->setVRegister(0, v0);
    chip_8_state->setVRegister(15, 0);

    Execute8XYE(chip_8_state, 0x800E);
    assert(chip_8_state->vRegister(0) == 0b11000100);
    assert(chip_8_state->vRegister(15) == 1);
}

/**
 * @brief Executes the 0x9XY0 op code on the chip state
 *
 * 0x9XY0 - Skips the next instruction if VX doesn't equal VY.
 * (Usually the next instruction is a jump to skip a code block)
 *
 */
void test9XY0() {
    uint8_t* memory = new uint8_t[RAM_SIZE];
    uint8_t* v_registers = new uint8_t[V_REGISTER_COUNT];

    uint8_t v0 = 0x00;
    uint8_t v1 = 0x00;
    uint8_t v2 = 0x42;

    CHIP8_State* chip_8_state = new CHIP8_State(INITAL_PROGRAM_COUNTER, 0, 0, 0, v_registers, memory);
    chip_8_state->setVRegister(0, v0);
    chip_8_state->setVRegister(1, v1);
    chip_8_state->setVRegister(2, v2);

    // compare v0 to v1
    Execute9XY0(chip_8_state, 0x9010);
    // v0 == v1, no skip
    assert(chip_8_state->programCounter() == INITAL_PROGRAM_COUNTER);

    // compare v0 to v2, not same.
    Execute9XY0(chip_8_state, 0x9020);
    // Assert PC was skipped ahead by two
    assert(chip_8_state->programCounter() == INITAL_PROGRAM_COUNTER + 2);
}

/**
 * @brief Tests the 0xANNN op code on the chip state
 *
 * 0xANNN - Sets I (index register) to the address NNN.
 *
 */
void testANNN() {
    uint8_t* memory = new uint8_t[RAM_SIZE];
    uint8_t* v_registers = new uint8_t[V_REGISTER_COUNT];

    CHIP8_State* chip_8_state = new CHIP8_State(INITAL_PROGRAM_COUNTER, 0, 0, 0, v_registers, memory);

    // Set I to 0x400
    ExecuteANNN(chip_8_state, 0xA400);
    assert(chip_8_state->indexRegister() == 0x400);
}

/**
 * @brief Tests the 0xBNNN op code on the chip state
 *
 * 0xBNNN - Jumps to the address NNN plus V0.
 *
 */
void testBNNN() {
    uint8_t* memory = new uint8_t[RAM_SIZE];
    uint8_t* v_registers = new uint8_t[V_REGISTER_COUNT];

    uint8_t v0 = 0xBB;

    CHIP8_State* chip_8_state = new CHIP8_State(INITAL_PROGRAM_COUNTER, 0, 0, 0, v_registers, memory);
    chip_8_state->setVRegister(0, v0);

    // Set I to 0x400 + V0
    ExecuteBNNN(chip_8_state, 0xB400);
    assert(chip_8_state->programCounter() == (0x400 + 0xBB));
}

/**
 * @brief Tests the 0xCXNN op code on the chip state
 *
 * 0xCXNN - Sets VX to the result of a bitwise and operation on a random number and NN
 * (Typically: 0 to 255)
 */
void testCNNN() {
    uint8_t* memory = new uint8_t[RAM_SIZE];
    uint8_t* v_registers = new uint8_t[V_REGISTER_COUNT];

    uint8_t v0 = 0xBB;

    CHIP8_State* chip_8_state = new CHIP8_State(INITAL_PROGRAM_COUNTER, 0, 0, 0, v_registers, memory);
    chip_8_state->setVRegister(0, v0);

    // Set I to 0x400 + V0
    ExecuteCNNN(chip_8_state, 0xC0AA);
    assert(chip_8_state->vRegister(0) != 0xBB);
}

/**
 * @brief Tests the 0xFX15 op code on the chip state
 *
 * 0xFX15 - Sets the delay timer to VX.
 *
 */
void testFX15() {
    uint8_t* memory = new uint8_t[RAM_SIZE];
    uint8_t* v_registers = new uint8_t[V_REGISTER_COUNT];

    uint8_t v0 = 0xBB;

    CHIP8_State* chip_8_state = new CHIP8_State(INITAL_PROGRAM_COUNTER, 0, 0, 0, v_registers, memory);
    chip_8_state->setVRegister(0, v0);

    // Set delay timer to v)
    ExecuteFX15(chip_8_state, 0xF015);
    assert(chip_8_state->delayTimer() == v0);
}

/**
 * @brief Tests the 0xFX18 op code on the chip state
 *
 * 0xFX18 - Sets the sound timer to VX.
 *
 */
void testFX18() {
    uint8_t* memory = new uint8_t[RAM_SIZE];
    uint8_t* v_registers = new uint8_t[V_REGISTER_COUNT];

    uint8_t v0 = 0xBB;

    CHIP8_State* chip_8_state = new CHIP8_State(INITAL_PROGRAM_COUNTER, 0, 0, 0, v_registers, memory);
    chip_8_state->setVRegister(0, v0);

    // Set sound timer to v)
    ExecuteFX18(chip_8_state, 0xF018);
    assert(chip_8_state->soundTimer() == v0);
}

/**
 * @brief Tests the 0xFX1E op code on the chip state
 *
 * 0xFX1E - Adds VX to I.
 *
 */
void testFX1E() {
    uint8_t* memory = new uint8_t[RAM_SIZE];
    uint8_t* v_registers = new uint8_t[V_REGISTER_COUNT];

    uint16_t initial_i = 0xFFE;
    uint8_t v0 = 0x01;
    uint8_t v1 = 0xFF;

    CHIP8_State* chip_8_state = new CHIP8_State(INITAL_PROGRAM_COUNTER, initial_i, 0, 0, v_registers, memory);
    chip_8_state->setVRegister(0, v0);
    chip_8_state->setVRegister(1, v1);
    chip_8_state->setVRegister(0xF, 0);

    ExecuteFX1E(chip_8_state, 0xF01E);
    assert(chip_8_state->indexRegister() == (v0 + initial_i));

    initial_i = chip_8_state->indexRegister();
    ExecuteFX1E(chip_8_state, 0xF11E);
    assert(chip_8_state->indexRegister() == (v1 + initial_i));
}

/**
 * @brief Tests the 0xFX29 op code on the chip state
 *
 * 0xFX29 - Sets I to the location of the sprite for the character in VX.
 * Characters 0-F (in hexadecimal) are represented by a 4x5 font.
 */
void testsFX29() {
    uint8_t* memory = new uint8_t[RAM_SIZE];
    uint8_t* v_registers = new uint8_t[V_REGISTER_COUNT];

    uint8_t v0 = 0x1;
    uint8_t v15 = 0xe;

    CHIP8_State* chip_8_state = new CHIP8_State(INITAL_PROGRAM_COUNTER, 0, 0, 0, v_registers, memory);
    chip_8_state->setVRegister(0, v0);
    chip_8_state->setVRegister(14, v15);

    // Move "1" character to index
    ExecuteFX29(chip_8_state, 0xF029);
    assert(chip_8_state->indexRegister() == 5);

    // Move "d" character to index
    ExecuteFX29(chip_8_state, 0xFE29);
    assert(chip_8_state->indexRegister() == 70);
}

/**
 * @brief Executes the 0xFX33 op code on the chip state
 *
 * 0xFX33 - Stores the binary-coded decimal representation of VX,
 * with the most significant of three digits at the address in I,
 * the middle digit at I plus 1, and the least significant digit at I plus 2.
 * (In other words, take the decimal representation of VX,
 * place the hundreds digit in memory at location in I,
 * the tens digit at location I+1, and the ones digit at location I+2.)
 */
void testFX33() {
    uint8_t* memory = new uint8_t[RAM_SIZE];
    uint8_t* v_registers = new uint8_t[V_REGISTER_COUNT];

    uint8_t v0 = 0xFF;
    uint8_t v1 = 0x06;

    CHIP8_State* chip_8_state = new CHIP8_State(INITAL_PROGRAM_COUNTER, INITAL_PROGRAM_COUNTER, 0, 0, v_registers, memory);
    chip_8_state->setVRegister(0, v0);
    chip_8_state->setVRegister(1, v1);

    ExecuteFX33(chip_8_state, 0xF033);

    uint16_t index_register = chip_8_state->indexRegister();

    assert(chip_8_state->memoryValue(index_register) == 0x02);
    assert(chip_8_state->memoryValue(index_register + 1) == 0x05);
    assert(chip_8_state->memoryValue(index_register + 2) == 0x05);

    ExecuteFX33(chip_8_state, 0xF133);

    assert(chip_8_state->memoryValue(index_register) == 0x00);
    assert(chip_8_state->memoryValue(index_register + 1) == 0x00);
    assert(chip_8_state->memoryValue(index_register + 2) == 0x06);
}

/**
 * @brief Tests the 0xFX55 op code on the chip state
 *
 * 0xFX55 - Stores V0 to VX (including VX) in memory starting at address I.
 * The offset from I is increased by 1 for each value written, but I itself is left unmodified.
 */
void testFX55() {
    uint8_t* memory = new uint8_t[RAM_SIZE];
    uint8_t* v_registers = new uint8_t[V_REGISTER_COUNT];

    uint8_t v0 = 0xFF;
    uint8_t v1 = 0x1A;
    uint8_t v2 = 0x2B;


    CHIP8_State* chip_8_state = new CHIP8_State(INITAL_PROGRAM_COUNTER, INITAL_PROGRAM_COUNTER, 0, 0, v_registers, memory);
    chip_8_state->setVRegister(0, v0);
    chip_8_state->setVRegister(1, v1);
    chip_8_state->setVRegister(2, v2);

    // Store V0 to V2 in memory starting at I
    ExecuteFX55(chip_8_state, 0xF255);

    uint16_t index_register = chip_8_state->indexRegister();

    assert(chip_8_state->memoryValue(index_register) == v0);
    assert(chip_8_state->memoryValue(index_register + 1) == v1);
    assert(chip_8_state->memoryValue(index_register + 2) == v2);
}

/**
 * @brief Executes the 0xFX65 op code on the chip state
 *
 * 0xFX65 - Fills V0 to VX (including VX) with values from memory starting at address I.
 * The offset from I is increased by 1 for each value written, but I itself is left unmodified.
 *
 */
void testFX65() {
    uint8_t* memory = new uint8_t[RAM_SIZE];
    uint8_t* v_registers = new uint8_t[V_REGISTER_COUNT];

    uint8_t i0 = 0xFF;
    uint8_t i1 = 0x1A;
    uint8_t i2 = 0x2B;

    CHIP8_State* chip_8_state = new CHIP8_State(INITAL_PROGRAM_COUNTER, INITAL_PROGRAM_COUNTER, 0, 0, v_registers, memory);

    uint16_t index_register = chip_8_state->indexRegister();
    chip_8_state->setMemoryValue(index_register, i0);
    chip_8_state->setMemoryValue(index_register + 1, i1);
    chip_8_state->setMemoryValue(index_register + 2, i2);

    // Fill V0 to V2 with values from I, I+1... etc
    ExecuteFX65(chip_8_state, 0xF265);

    assert(chip_8_state->vRegister(0) == i0);
    assert(chip_8_state->vRegister(1) == i1);
    assert(chip_8_state->vRegister(2) == i2);
}

/**
 * @brief Executes the 0xDXYN op code on the chip state
 *
 * 0xDXYN - Draws a sprite at coordinate (VX, VY) that has a width of 8 pixels and a height of N pixels.
 * Each row of 8 pixels is read as bit-coded starting from memory location I;
 * I value doesn’t change after the execution of this instruction. As described above, VF is set to 1 if any screen
 *  pixels are flipped from set to unset when the sprite is drawn, and to 0 if that doesn’t happen
 */
void testDXYN() {
    uint8_t* memory = new uint8_t[RAM_SIZE];
    uint8_t* v_registers = new uint8_t[V_REGISTER_COUNT];

    uint8_t v0 = 0x07;
    uint8_t v1 = 0x00;

    CHIP8_State* chip_8_state = new CHIP8_State(INITAL_PROGRAM_COUNTER, INITAL_PROGRAM_COUNTER, 0, 0, v_registers, memory);

    // Set the V Registers
    chip_8_state->setVRegister(0, v0);
    chip_8_state->setVRegister(1, v1);
    chip_8_state->setVRegister(0xF, 0);

    // Adding a sprite 1 row high into memory.
    // Sprite is: ########
    uint16_t index_register = chip_8_state->indexRegister();
    chip_8_state->setMemoryValue(index_register, 0xFF);

    // Draw the single row sprite at 0, 0 (V1, V1)
    ExecuteDXYN(chip_8_state, 0xF111);

    // Ensure the sprite are active
    for (int i = 0; i < 8; i++) {
        assert(chip_8_state->displayValue(i, 0) == true);
    }
    // Ensure VF was not set
    assert(chip_8_state->vRegister(0xF) == false);

    // Draw the single row sprite at 0, 7 (V0, V1)
    // This should overlap with the first sprite
    ExecuteDXYN(chip_8_state, 0xF011);

    // Ensure the pixels are active except where they overlap
    // Display should look like this: ####### #######
    for (int i = 0; i < 15; i++) {

        if (i == 7) {
            assert(chip_8_state->displayValue(i, 0) == false);
        } else {
            assert(chip_8_state->displayValue(i, 0) == true);
        }
    }

    // Ensure VF was set since a bit was flipped from set to unset
    assert(chip_8_state->vRegister(0xF) == true);
}

/**
 * @brief Executes the 0xDXYN op code when an overflow on the x axis occurs
 *
 */
void testDXYNOverflowX() {
    uint8_t* memory = new uint8_t[RAM_SIZE];
    uint8_t* v_registers = new uint8_t[V_REGISTER_COUNT];

    uint8_t v0 = (uint8_t)63;
    uint8_t v1 = 0x00;

    CHIP8_State* chip_8_state = new CHIP8_State(INITAL_PROGRAM_COUNTER, INITAL_PROGRAM_COUNTER, 0, 0, v_registers, memory);

    // Set the V Registers
    chip_8_state->setVRegister(0, v0);
    chip_8_state->setVRegister(1, v1);
    chip_8_state->setVRegister(0xF, 0);

    // Adding a sprite 1 row high into memory.
    // Sprite is: ########
    uint16_t index_register = chip_8_state->indexRegister();
    chip_8_state->setMemoryValue(index_register, 0xFF);

    // Draw the single row sprite at 63, 0 (V0, V1)
    // Majority of sprite should overlap: |####### .. screen .. #|
    ExecuteDXYN(chip_8_state, 0xD011);
    assert(chip_8_state->displayValue(0, 0) == true);
    assert(chip_8_state->displayValue(1, 0) == true);
    assert(chip_8_state->displayValue(2, 0) == true);
    assert(chip_8_state->displayValue(3, 0) == true);
    assert(chip_8_state->displayValue(4, 0) == true);
    assert(chip_8_state->displayValue(5, 0) == true);
    assert(chip_8_state->displayValue(6, 0) == true);
    assert(chip_8_state->displayValue(7, 0) == false);
    assert(chip_8_state->displayValue(63, 0) == true);
}

/**
 * @brief Executes the 0xDXYN op code when an overflow on the y axis occurs
 *
 */
void testDXYNOverflowY() {
    uint8_t* memory = new uint8_t[RAM_SIZE];
    uint8_t* v_registers = new uint8_t[V_REGISTER_COUNT];

    uint8_t v0 = 0x00;
    uint8_t v1 = (uint8_t)32;

    CHIP8_State* chip_8_state = new CHIP8_State(INITAL_PROGRAM_COUNTER, INITAL_PROGRAM_COUNTER, 0, 0, v_registers, memory);

    // Set the V Registers
    chip_8_state->setVRegister(0, v0);
    chip_8_state->setVRegister(1, v1);
    chip_8_state->setVRegister(0xF, 0);

    // Adding a sprite 1 row high into memory.
    // Sprite is: ########
    uint16_t index_register = chip_8_state->indexRegister();
    chip_8_state->setMemoryValue(index_register, 0xFF);

    // Draw the single row sprite at 63, 0 (V0, V1)
    // Majority of sprite should overlap: |####### .. screen .. #|
    ExecuteDXYN(chip_8_state, 0xD011);
    assert(chip_8_state->displayValue(0, 0) == true);
    assert(chip_8_state->displayValue(1, 0) == true);
    assert(chip_8_state->displayValue(2, 0) == true);
    assert(chip_8_state->displayValue(3, 0) == true);
    assert(chip_8_state->displayValue(4, 0) == true);
    assert(chip_8_state->displayValue(5, 0) == true);
    assert(chip_8_state->displayValue(6, 0) == true);
    assert(chip_8_state->displayValue(7, 0) == true);
}

int main(int argc, char** argv){

    test00E0();
    test00EE();
    test1NNN();
    test2NNN();
    test3XNN();
    test4XNN();
    test5XY0();
    test6XNN();
    test7XNN();
    test8XY0();
    test8XY1();
    test8XY2();
    test8XY3();
    test8XY4();
    test8XY5();
    test8XY6();
    test8XY7();
    test8XYE();
    test9XY0();
    testANNN();
    testBNNN();
    testCNNN();
    testFX15();
    testFX1E();
    testsFX29();
    testFX33();
    testFX55();
    testFX65();
    testDXYN();
    testDXYNOverflowX();
    testDXYNOverflowY();

    return 0;
}