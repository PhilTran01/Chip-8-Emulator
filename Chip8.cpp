
#include "Chip8.hpp"

void Chip8::initialize() {
	reg.PC = 0x200;
	reg.SP = 0;
	reg.I = 0;
	reg.DT = 0;
	reg.ST = 0;

	// init graphics display
	for (int i = 0; i < 64*32; i++) {
		gfx_display[i] = 0;
	}

	// init memory
	for (int i = 0; i < 0x200; i++) {
		c_memory[i] = 0;
	}

	// init keypad
	for (int i = 0; i < 16; i++) {
		key_input[i] = 0;
	}

	// init the general purpose registers
	for (int i = 0; i < 16; i++) {
		reg.V[i] = 0;
	}

}

auto Chip8::instructions() -> void {
	// Get opcode. I'm incrementing the PC in here as well, equivalent to PC++
	uint16_t opcode = ( (c_memory[reg.PC] << 8) | c_memory[++reg.PC] );	// Fetch, 2 bytes long, stored big endian

	// Decode and Execute
	switch(opcode & 0xF000) {
		case 0x0000:
			switch(opcode & 0x000F) {
				case 0x0000:	// 0x00E0 Clear Screen
					CLS();
					break;

				case 0x000E:	// 0x00EE Return from subroutine
					RET();
					break;

				default:
					printf("Unknown opcode 0x%X\n", opcode);
			}

			switch(opcode & 0xF000) {
				case 0x1000:	// 0x1NNN Jump to address NNN
					JP(opcode);
					break;

				case 0x2000:	// 0x2NNN Call subroutine at NNN
					CALL(opcode);
					break;

				case 0x3000:	// 0x3XKK Skips the next instruction if VX = KK
					SE_VX_BYTE(opcode);
					break;

				case 0x4000:	// 0x4XKK Skips the next instruction if VX != KK
					break;

				case 0x5000:	// 0x5XY0 Skip next instruction if VX = VY.
					break;

				case 0x6000:	// 0x6XKK Set VX = KK
					break;

				case 0x7000:	// 0x7XKK Set VX = VX + KK
					break;

				case 0x8000:
					switch(opcode & 0x000F) {
						case 0x0000:	// 0x8000 Set VX = VY
							break;

						// TODO: FINISH LISTING OPCODES
					}
					break;
			}
	}

}

auto Chip8::CLS() -> void {
	for (int i = 0; i < 64*32; i++) {
		gfx_display[i] = 0;
	}
	reg.PC++;
}

auto Chip8::RET() -> void {
	reg.PC = reg.S[reg.SP--];
}

auto Chip8::JP(uint16_t opcode) -> void {
	reg.PC = (opcode & 0x0FFF);
}

auto Chip8::CALL(uint16_t opcode) -> void {
	reg.S[++reg.SP] = reg.PC;
	reg.PC = (opcode & 0x0FFF);
}

auto Chip8::SE_VX_BYTE(uint16_t opcode) -> void {
	if ( reg.V[ (opcode & 0x0F00) >> 8 ] == (opcode & 0x00FF) ) {
		reg.PC += 3;
	}
	else {
		reg.PC ++;
	}
}

auto Chip8::SE_VX_VY(uint16_t opcode) -> void{
	if ( reg.V[ (opcode & 0x0F00) >> 8 ] == (opcode & 0x00FF) ) {
		// FINISH THIS FUNCTION
	}
}
// Not really needed for now
//uint8_t Chip8::read(uint16_t addr) {
//	return c_memory[addr];
//}
//
//void Chip8::write(uint16_t addr, uint8_t data) {
//	c_memory[addr] = data;
//}


