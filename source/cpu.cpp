#include "cpu.h"
#include <iostream>
#include <fstream>
#include <cstdlib>
#include <cstring>
#include <ctime>    



void CPU::op_00E0(){
    memset(gfx, 0, sizeof(gfx));
}


void CPU::op_00EE(){
    sp--;
    pc = stack[sp];
}


void CPU::op_1nnn(){
    uint16_t adress = opcode & 0x0FFFu;
    pc = adress;
}

void CPU::op_2nnn(){
    stack[sp] = pc;
    sp++;

    uint16_t adress = opcode & 0x0FFFu; 
    pc = adress;
}

void CPU::op_3xkk(){
    uint8_t x = (opcode & 0x0F00u) >> 8;
    uint8_t value = opcode & 0x00FFu;
    
    if(V[x] == value)
        pc+=2;
}

void CPU::op_4xkk(){
    uint8_t x = (opcode & 0x0F00u) >> 8;
    uint8_t value = opcode & 0x00FFu;

    if(V[x] != value)
        pc+=2;
}

void CPU::op_5xy0(){
    uint8_t x = (opcode & 0x0F00u) >> 8;
    uint8_t y = (opcode & 0x00F0u) >> 4;

    if(V[x] == V[y])
        pc+=2;
}

void CPU::op_6xkk(){
    uint8_t x = (opcode & 0x0F00u) >> 8;
    uint8_t value = opcode & 0x00FFu;
    
    V[x] = value;
}

void CPU::op_7xkk(){
    uint8_t x = (opcode & 0x0F00u) >> 8;
    uint8_t value = opcode & 0x00FFu;

    V[x] += opcode & 0x00FFu;
}

void CPU::op_9xy0(){
    uint8_t x = (opcode & 0x0F00u) >> 8;
    uint8_t y = (opcode & 0x00F0u) >> 4;
    
    if(V[x] != V[y])
        pc+=2;
}

void CPU::op_Annn(){
    uint16_t adress = opcode & 0x0FFFu;
    I = adress;
}

void CPU::op_Bnnn(){
    uint16_t adress = opcode & 0x0FFFu; 
    pc = V[0] + adress;
}

void CPU::op_Cxkk(){
    uint8_t x = (opcode & 0x0F00u) >> 8;
    uint8_t value = opcode & 0x00FFu;
    uint8_t random = (uint8_t) rand() % 256;

    V[x] = random & value;

}

void CPU::op_Dxyn(){
    uint8_t x = (opcode & 0x0F00) >> 8;
    uint8_t y = (opcode & 0x00F0) >> 4;
    uint8_t height = opcode & 0x000F;

    uint8_t posx = V[x] % VIDEO_WIDTH;
    uint8_t posy = V[y] % VIDEO_HEIGHT;  

    V[0xF] = 0;

    for(unsigned int row=0; row<height; row++){

        uint8_t spriteByte = memory[I + row];

        for (unsigned int col = 0; col < 8; col++){
            uint8_t spritePixel = spriteByte & (0x80 >> col);
            uint32_t *screenPixel = gfx + ((posy+row) * VIDEO_WIDTH + (posx+col));

            if(spritePixel){
                if(*screenPixel == 0xFFFFFFFF){
                    V[0xF] = 1;
                }

                *screenPixel^=0xFFFFFFFF;
            }
        }
    }
}


void CPU::op_8xy0(){
    uint8_t x = (opcode & 0x0F00u) >> 8;
    uint8_t y = (opcode & 0x00F0u) >> 4;

    V[x] = V[y];

}

void CPU::op_8xy1(){
    uint8_t x = (opcode & 0x0F00u) >> 8;
    uint8_t y = (opcode & 0x00F0u) >> 4;

    V[x] |= V[y];
}

void CPU::op_8xy2(){
    uint8_t x = (opcode & 0x0F00u) >> 8;
    uint8_t y = (opcode & 0x00F0u) >> 4;

    V[x] &= V[y];
}

void CPU::op_8xy3(){
    uint8_t x = (opcode & 0x0F00u) >> 8;
    uint8_t y = (opcode & 0x00F0u) >> 4;

    V[x] ^= V[y];
}
void CPU::op_8xy4(){
    uint8_t x = (opcode & 0x0F00u) >> 8;
    uint8_t y = (opcode & 0x00F0u) >> 4;

    uint16_t sum = V[x] + V[y];

    if (sum > 0xFF)
        V[0xFu] = 1;
    else
        V[0xFu] = 0;
    
    V[x] = sum & 0x00FFu;
}

void CPU::op_8xy5(){
    uint8_t x = (opcode & 0x0F00u) >> 8;
    uint8_t y = (opcode & 0x00F0u) >> 4;

    if(V[x] > V[y])
        V[0xFu] = 1;
    else 
        V[0xFu] = 0;

    V[x] -= V[y];
}

void CPU::op_8xy6(){
    uint8_t x = (opcode & 0x0F00u) >> 8;
    V[0xFu] = V[x] & 0x1u;

    V[x] >>=1;
}
void CPU::op_8xy7(){
    uint8_t x = (opcode & 0x0F00u) >> 8;
    uint8_t y = (opcode & 0x00F0u) >> 4;

    if(V[y] > V[x])
        V[0xFu] = 1;
    else 
        V[0xFu] = 0;

    V[x] = V[y] - V[x];
}
void CPU::op_8xyE(){
    uint8_t x = (opcode & 0x0F00u) >> 8;
    V[0xFu] = (V[x] & 0x80u) >> 7;

    V[x] <<=1;
}

void CPU::op_Ex9E(){
    uint8_t x  = (opcode & 0x0F00u) >> 8;
    uint8_t k = V[x];

    if(key[k]){
        pc+=2;
    }
}

void CPU::op_ExA1(){
    uint8_t x  = (opcode & 0x0F00u) >> 8;
    uint8_t k = V[x];

    if(!key[k]){
        pc+=2;
    }
}


void CPU::op_Fx07(){
    uint8_t x = (opcode & 0x0F00u) >> 8u;
    V[x] = delay_timer;
}

void CPU::op_Fx0A(){
    uint8_t x = (opcode & 0x0F00u) >> 8u;
    
    for(uint8_t i = 0; i<0x10; i++){
        if(key[i]){
            V[x] = i;
            return;
        }
    }

    pc-=2;
}

void CPU::op_Fx15(){
    uint8_t x = (opcode & 0x0F00u) >> 8u;
    delay_timer = V[x];
}

void CPU::op_Fx18(){
    uint8_t x = (opcode & 0x0F00u) >> 8u;
    sound_timer = V[x];
}

void CPU::op_Fx1E(){
    uint8_t x = (opcode & 0x0F00u) >> 8u;
    I += V[x];
}

void CPU::op_Fx29(){
    uint8_t x = (opcode & 0x0F00u) >> 8u;
    uint16_t offset = (uint16_t) 5*V[x];
    
    I = START_ADDRESS + offset;
}

void CPU::op_Fx33(){
    uint8_t x = (opcode & 0x0F00) >> 8u;
    uint8_t content = V[x];

    for(int16_t i = 2; i >= 0; i--){
        memory[I+i] = content % 10;
        content/=10;
    }
}

void CPU::op_Fx55(){
    uint8_t x = (opcode & 0x0F00) >> 8u;

    for(uint16_t i=0; i<=x; i++){
        memory[I+i] = V[i];
    }
}

void CPU::op_Fx65(){
    uint8_t x = (opcode & 0x0F00) >> 8u;

    for(uint16_t i=0; i<=x; i++){
        V[i] = memory[I+i];
    }
}


CPU::CPU(){
    pc     = START_ADDRESS;  // Program counter starts at START_ADDRESS
    opcode = 0;      // Reset current opcode	
    I      = 0;      // Reset index register
    sp     = 0;      // Reset stack pointer

    srand(time(0));
    
    // Clear display	
    // Clear stack
    // Clear registers V0-VF
    // Clear memory
    
    // Load fontset
    for(int i = 0; i < 80; ++i)
      memory[i] = fontset[i];		
    
    // Reset timers
}

void CPU::load(const char* file_path){
    
    std::ifstream rom(file_path, std::ifstream::binary | std::ios::ate); 

    if(rom.is_open()){
        // Get size of file and allocate a buffer to hold the contents
		std::streampos size = rom.tellg();
		char* buffer = new char[size];    

        // Go back to the beginning of the file and fill the buffer
		rom.seekg(0, std::ios::beg);
		rom.read(buffer, size);
		rom.close();

		// Load the ROM contents into the chip8's memory, starting at START_ADDRESS
		for (long i = 0; i < size; ++i){
			memory[START_ADDRESS + i] = buffer[i];
        }

		// Free the buffer
		delete[] buffer;
    }
}

void CPU::run(){

    log();
    fetch();
    execute();

    updateTimers();  
}


void CPU::fetch(){
    opcode = (memory[pc] << 8) | memory[pc+1];
    pc+=2;
}

void CPU::execute(){
    switch ((opcode & 0xF000u) >> 12u)
    {
    case 0x0u:
        switch (opcode & 0x000Fu)
        {
        case 0x0u:
            op_00E0();
            break;
        case 0xEu:
            op_00EE();
            break;
        default:
            break;
        }
        break;
    
    case 0x1u:
        op_1nnn();
        break;
    
    case 0x2u:
        op_2nnn();
        break;
    
    case 0x3u:
        op_3xkk();
        break;

    case 0x4u:
        op_4xkk();
        break;
    
    case 0x5u:
        op_5xy0();
        break;

    case 0x6u:
        op_6xkk();
        break;

    case 0x7u:
        op_7xkk();
        break;

    case 0x8u:
        switch (opcode & 0x000Fu)
        {
        case 0x0u:
            op_8xy0();
            break;
        case 0x1u:
            op_8xy1();
            break;
        case 0x2u:
            op_8xy2();
            break;
        case 0x3u:
            op_8xy3();
            break;
        case 0x4u:
            op_8xy4();
            break;
        case 0x5u:
            op_8xy5();
            break;
        case 0x6u:
            op_8xy6();
            break;
        case 0x7u:
            op_8xy7();
            break;
        case 0xEu:
            op_8xyE();
            break;
        default:
            break;
        }
        break;
    case 0x9u:
        op_9xy0();
        break;
    case 0xAu:
        op_Annn();
        break;
    case 0xBu:
        op_Bnnn();
        break;
    case 0xCu:
        op_Cxkk();
        break;
    case 0xDu:
        op_Dxyn();
        break;
    case 0xEu:
        switch (opcode & 0x000Fu)
        {
        case 0xEu:
            op_Ex9E();
            break;
        case 0x1u:
            op_ExA1();
            break;
        default:
            break;
        }
        break;
    case 0xFu:
        switch ((opcode & 0x00F0u) >> 4u)
        {
        case 0x0u:
            switch (opcode & 0x000Fu)
            {
            case 0x7u:
                op_Fx07();
                break;
            case 0xAu:
                op_Fx0A();
                break;
            default:
                break;
            }
            break;
        case 0x1u:
            switch (opcode & 0x000Fu)
            {
            case 0x5:
                op_Fx15();
                break;
            case 0x8:
                op_Fx18();
                break;
            case 0xE:
                op_Fx1E();
                break;
            default:
                break;
            }
            break;
        case 0x2u:
            op_Fx29();
            break;
        case 0x3u:
            op_Fx33();
            break;
        case 0x5:
            op_Fx55();
            break;
        case 0x6:
            op_Fx65();
            break;

        default:
            break;
        }

    default:
        break;
    }
}

void CPU::updateTimers(){
    if(delay_timer > 0)
        --delay_timer;
 
    if(sound_timer > 0)
    {
        --sound_timer;
    }
}

void CPU::log(){
    
    for(int i=0; i<0x10; i++){
        printf("%X ", V[i]);
    }
    std::cout << std::endl;
    
    std::cout << "PC: " << std::hex << pc << std::endl;
    std::cout << "Opcode: " << std::hex << opcode << std::endl;
    std::cout << "Index: " << std::hex << I << std::endl;

    std::cout << std::endl;


}

uint8_t* CPU::get_keypad(){
    return key;
}

uint32_t* CPU::get_video(){
    return gfx;
}