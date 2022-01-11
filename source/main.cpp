#include <iostream>
#include <fstream>
#include <string>
#include <chrono>
#include "cpu.h"
#include "platform.h"

using namespace std;

int main(){

    const int videoScale = 10;
    const int cycleDelay = 3;

    Platform* platform = new Platform("CHIP-8 Emu", VIDEO_WIDTH * videoScale, 
                    VIDEO_HEIGHT * videoScale, VIDEO_WIDTH, VIDEO_HEIGHT);
    
    CPU* cpu = new CPU();
    cpu->load("./roms/Tetris.ch8");


    auto lastCycleTime = std::chrono::high_resolution_clock::now();
    int videoPitch = sizeof(cpu->get_video()[0]) * VIDEO_WIDTH;
    bool quit = false;

    while(!quit){
        quit = platform->process_input(cpu->get_keypad());

        auto currentTime = std::chrono::high_resolution_clock::now();
		float dt = std::chrono::duration<float, std::chrono::milliseconds::period>(currentTime - lastCycleTime).count();

        if (dt > cycleDelay)
		{
			lastCycleTime = currentTime;

            cpu->run();

            platform->update(cpu->get_video(), videoPitch);
		}

    }

    delete cpu;
    delete platform;

    return 0;

}