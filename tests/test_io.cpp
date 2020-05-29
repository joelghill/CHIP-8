#include <filesystem>
#include <iostream>
#include <sstream>
#include<vector>
#include "../src/io.hpp"

using namespace std;

void test_read_rom(string filename, int expectded_size)
{
  cout << "Loading " << filename << endl;
  vector<char>* rom_data = ReadRom(filename);

  if (rom_data->size() != expectded_size) {
    throw "Rom is not of expected size";
  }
}

int main(int argc, char** argv)
{
  // This test assumes it is called from the test executable directory
  test_read_rom("../../roms/programs/Clock Program [Bill Fisher, 1981].ch8", 280);
}