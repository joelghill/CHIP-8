/**
 * @file io.cpp
 * @author Joel Hill (joel.hiil.87@gmail.com)
 * @brief File containing implementation of ROM IO functions
 *
 * @copyright Copyright (c) 2020
 *
 */
#include <iostream>
#include<iterator>
#include <fstream>
#include <vector>

#include "io.hpp"

using namespace std;

vector<char>* ReadRom(string filename) {
    ifstream rom_file(filename, ios::in);

    if (rom_file) {

        vector<char>* rom_bytes = new vector<char>(
            (istreambuf_iterator<char>(rom_file)),
            istreambuf_iterator<char>()
        );

        return rom_bytes;
    }

    throw "Could not load rom";
}