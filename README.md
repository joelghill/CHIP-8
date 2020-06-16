# CHIP-8
Run CHIP-8 roms in your terminal.

![](./docs/media/chip-demo.gif)

## Installation
```
git clone https://github.com/joelghill/CHIP-8.git
cd CHIP-8
mkdir build
cd build
cmake .
make chip-8
make install
```

## Usage
```
chip-8 <rom name>
```
Does not support high res roms.

## Known Issues
Games such as "Pong" or "Breakout" are not working due to a collision detection bug

