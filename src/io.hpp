#ifndef IO_HPP
#define IO_HPP

/**
 * @file io.hpp
 * @author Joel Hill (joel.hiil.87@gmail.com)
 * @brief File containing definition of ROM IO functions
 *
 * @copyright Copyright (c) 2020
 *
 */
#include<vector>

using namespace std;

/**
 * @brief Reads a CHIP-8 Rom into memory
 *
 * @param filename The filename of the ROM to read into memory
 * @return char* Byte array of the application to run in CHIP-8
 */
vector<char>* ReadRom(string filename);

#endif