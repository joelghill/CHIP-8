/**
 * @file chip-8_state.cpp
 * @author Joel Hill (joel.hill.87@gmail.com)
 * @brief
 * @version 0.1
 * @date 2020-05-30
 *
 * @copyright Copyright (c) 2020
 *
 */
#include <iostream>
#include <vector>
#include "chip-8_state.hpp"
#include "exceptions.hpp"

using namespace std;

CHIP8_State::CHIP8_State(
        int programCounter,
        uint16_t indexRegister,
        uint8_t delayTimer,
        uint8_t soundTimer,
        uint8_t* vRegisters,
        uint8_t* memory) {

    // Initialize program counter
    this->setProgramCounter(programCounter);

    // Initialiize the index register
    this->setIndexRegister(indexRegister);

    // Timers should be initialized to 0
    this->setDelayTimer(delayTimer);
    this->setSoundTimer(soundTimer);

    // Initialize the V Registers
    if (vRegisters == NULL) {
        this->vRegisters_ = new uint8_t[V_REGISTER_COUNT];
    } else {
        this->vRegisters_ = vRegisters;
    }

    // Initialize memory for CHIP-8 RAM
    if (memory == NULL) {
        this->memory_ = new uint8_t[RAM_SIZE];
    } else {
        this->memory_ = memory;
    }

    // The 96 bytes below the display are reserved for the call stack
    // 0xEA0-0xEFF
   this->stack_ = &(this->memory_[STACK_MEMORY_LOCATION]);

    // Load the font set into the first 80 bytes of memory
    for (uint8_t character_index = 0; character_index < 80; character_index++) {
        uint8_t character_address = character_index + FONT_MEMORY_LOCATION;
        this->memory_[character_address] = this->fontset_[character_index];
    }
}

void CHIP8_State::pushStack(uint16_t address) {
    // All instructions are 2 bytes long and are stored most-significant-byte first.
    uint8_t ms_address = (address & 0xFF00) >> 8;
    uint8_t ls_address = address & 0x00FF;

    this->stackPointer_ += 2;

    this->stack_[this->stackPointer_] = ms_address;
    this->stack_[this->stackPointer_ + 1] = ls_address;
}

uint16_t CHIP8_State::popStack() {
    // If the stack is empty prevent pop
    if (this->stackPointer_ < 0) {
        throw InvalidStackOperationException("Stack is empty");
    }

    // Get 16 bit address from top of stack then decrement pointer
    // Stack is array of bytes, so 16 bit address is grabbed in two parts
    uint16_t ms_address = (uint16_t)this->stack_[this->stackPointer_];
    uint16_t ls_address = (uint16_t)this->stack_[this->stackPointer_ + 1];
    this->stackPointer_ -= 2;

    uint16_t address = (ms_address << 8) | ls_address;
    return address;
}

uint16_t CHIP8_State::peekStack() {
    // If the stack is empty prevent pop
    if (this->stackPointer_ < 0) {
        throw InvalidStackOperationException("Stack is empty");
    }

    // Get 16 bit address from top of stack then decrement pointer
    // Stack is array of bytes, so 16 bit address is grabbed in two parts
    uint16_t ms_address = (uint16_t)this->stack_[this->stackPointer_];
    uint16_t ls_address = (uint16_t)this->stack_[this->stackPointer_ + 1];

    uint16_t address = (ms_address << 8) | ls_address;
    return address;
}

int16_t CHIP8_State::stackPointer() {
    return this->stackPointer_;
}

uint8_t CHIP8_State::vRegister(uint8_t register_index) {
    return this->vRegisters_[register_index];
}

void CHIP8_State::setVRegister(uint8_t register_index, uint8_t value) {
    this->vRegisters_[register_index] = value;
}

uint16_t CHIP8_State::indexRegister() {
    return this->indexRegister_;
}

void CHIP8_State::setIndexRegister(uint16_t value) {
    this->indexRegister_ = value;
}

uint16_t CHIP8_State::programCounter() {
    return this->programCounter_;
}

void CHIP8_State::setProgramCounter(uint16_t value) {
    this->programCounter_ = value;
}

void CHIP8_State::incrementProgramCounter(int value) {
    this->programCounter_ += value;
}

uint8_t CHIP8_State::delayTimer() {
    return this->delayTimer_;
}

void CHIP8_State::setDelayTimer(uint8_t value) {
    this->delayTimer_ = value;
}

uint8_t CHIP8_State::soundTimer() {
    return this->soundTimer_;
}

void  CHIP8_State::setSoundTimer(uint8_t value) {
    this->soundTimer_ = value;
}

uint8_t CHIP8_State::memoryValue(uint16_t index) {
    return this->memory_[index];
}

void CHIP8_State::setMemoryValue(uint16_t index, uint8_t value) {
    this->memory_[index] = value;
}

bool CHIP8_State::displayValue(int x, int y ) {
    return this->display_[x][y];
}

void CHIP8_State::setDisplayValue(int x, int y, bool value) {
    this->display_[x][y] = value;
}