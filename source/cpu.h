#include <cstdint>

const uint32_t VIDEO_HEIGHT = 32;
const uint32_t VIDEO_WIDTH = 64;

const unsigned int FONTSET_SIZE = 80;
const unsigned int START_ADDRESS = 0x200;
const unsigned int FONTSET_START_ADDRESS = 0x50;
const unsigned char fontset[FONTSET_SIZE] =
{ 
  0xF0, 0x90, 0x90, 0x90, 0xF0, // 0
  0x20, 0x60, 0x20, 0x20, 0x70, // 1
  0xF0, 0x10, 0xF0, 0x80, 0xF0, // 2
  0xF0, 0x10, 0xF0, 0x10, 0xF0, // 3
  0x90, 0x90, 0xF0, 0x10, 0x10, // 4
  0xF0, 0x80, 0xF0, 0x10, 0xF0, // 5
  0xF0, 0x80, 0xF0, 0x90, 0xF0, // 6
  0xF0, 0x10, 0x20, 0x40, 0x40, // 7
  0xF0, 0x90, 0xF0, 0x90, 0xF0, // 8
  0xF0, 0x90, 0xF0, 0x10, 0xF0, // 9
  0xF0, 0x90, 0xF0, 0x90, 0x90, // A
  0xE0, 0x90, 0xE0, 0x90, 0xE0, // B
  0xF0, 0x80, 0x80, 0x80, 0xF0, // C
  0xE0, 0x90, 0x90, 0x90, 0xE0, // D
  0xF0, 0x80, 0xF0, 0x80, 0xF0, // E
  0xF0, 0x80, 0xF0, 0x80, 0x80  // F
};

class CPU{
    
    public:
        CPU();
        void run();
        void load(const char* file_path);
        uint8_t* get_keypad();
        uint32_t* get_video();

    private:
        uint16_t opcode;
        uint16_t pc;
        uint16_t I;
        uint8_t  V[16];
        uint8_t  memory[4096];
        uint16_t stack[16];
        uint16_t sp;
        uint8_t  delay_timer;
        uint8_t  sound_timer;
        uint32_t gfx[VIDEO_HEIGHT * VIDEO_WIDTH];
        uint8_t  key[16];

        void op_00E0();
        void op_00EE();

        void op_1nnn();
        void op_2nnn();
        void op_3xkk();
        void op_4xkk();
        void op_5xy0();
        void op_6xkk();
        void op_7xkk();

        void op_8xy0();
        void op_8xy1();
        void op_8xy2();
        void op_8xy3();
        void op_8xy4();
        void op_8xy5();
        void op_8xy6();
        void op_8xy7();
        void op_8xyE();

        void op_9xy0();
        void op_Annn();
        void op_Bnnn();
        void op_Cxkk();
        void op_Dxyn();

        void op_Ex9E();
        void op_ExA1();

        void op_Fx07();
		void op_Fx0A();
		void op_Fx15();
		void op_Fx18();
		void op_Fx1E();
		void op_Fx29();
		void op_Fx33();
		void op_Fx55();
		void op_Fx65();

        void fetch();
        void decode();
        void execute();
        void updateTimers();
        void log();
}; 
