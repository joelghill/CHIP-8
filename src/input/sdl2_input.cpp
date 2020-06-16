#include <exception>
#include <iostream>
#include <map>
#include <SDL2/SDL.h>
#include <stdio.h>
#include <thread>
#include "sdl2_input.hpp"

using namespace std;

SDL2Input::SDL2Input() {
    /* Initialise SDL */
    if( SDL_Init( SDL_INIT_EVENTS | SDL_INIT_VIDEO) < 0){
        fprintf( stderr, "Could not initialise SDL: %s\n", SDL_GetError() );
        exit( -1 );
    }
    // Create an application window with the following settings:
    this->window_ = SDL_CreateWindow(
        "An SDL2 window",                  // window title
        SDL_WINDOWPOS_UNDEFINED,           // initial x position
        SDL_WINDOWPOS_UNDEFINED,           // initial y position
        640,                               // width, in pixels
        480,                               // height, in pixels
        SDL_WINDOW_OPENGL                  // flags - see below
    );
}

SDL2Input::~SDL2Input() {
    atexit(SDL_Quit);
}

bool SDL2Input::isPressed(uint8_t input_code) {
    const uint8_t* state = SDL_GetKeyboardState(NULL);
    cerr << "Checking if " << (uint16_t)input_code << " is pressed" << endl;
    cerr << "State " << (uint16_t)state[SDL_SCANCODE_8] << " is pressed" << endl;
    return state[SDL_SCANCODE_8] > 0;
}

uint8_t SDL2Input::getInput() {
    SDL_Event event;
    while (true) {
        while (SDL_WaitEvent( &event )) {
            if ( event.type == SDL_KEYDOWN) {
                cerr << "Key Pressed: " << event.key.keysym.sym << endl;
                return this->keymap_[event.key.keysym.scancode];
            }
        }
    }
}