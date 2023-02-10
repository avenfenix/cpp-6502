#include "aven6502.h"
#include "Bus.h"
#include <iostream>
#include <string>
#include <bitset>
#include <iomanip>
#include <inttypes.h>
#include <sstream>

#include "Engine.h"



std::string hex1byte0x(int x) {
	std::stringstream stream;
	stream << "0x" << std::setfill('0') << std::setw(2) << std::uppercase << std::hex << x << std::endl;
	std::string A_t(stream.str().substr(0, 4));
	return A_t;
}

std::string hex2byte0x(int x) {
	std::stringstream stream;
	stream << "0x" << std::setfill('0') << std::setw(4) << std::uppercase << std::hex << x << std::endl;
	std::string A_t(stream.str().substr(0, 6));
	return A_t;
}

std::string hex1bytedolar(int x) {
	std::stringstream stream;
	stream << "$" << std::setfill('0') << std::setw(2) << std::uppercase << std::hex << x << std::endl;
	std::string A_t(stream.str().substr(0, 3));
	return A_t;
}

std::string hex2bytedolar(int x) {
	std::stringstream stream;
	stream << "$" << std::setfill('0') << std::setw(2) << std::uppercase << std::hex << x << std::endl;
	std::string A_t(stream.str().substr(0, 5));
	return A_t;
}

std::string number2hex(int x, int out)
{
	if (out == 0)
	{
		return hex1byte0x(x);
	}
	else if (out == 1)
	{
		return hex2byte0x(x);
	}
	else if (out == 2)
	{
		return hex1bytedolar(x);
	}
	else if (out == 3)
	{
		return hex2bytedolar(x);
	}
}
uint16_t swap16(uint16_t x) {
	return (x << 8) | (x >> 8);
}

class App : public Engine {

	void appUpdate() override;
	void appCreate() override;
	struct memformat {
		uint16_t address;
		uint8_t byte;
	};
	//D:\\Emulation\\6502
	std::vector<memformat> rom =
	{
		{0x1FFE, 0x00}, {0x1FFF, 0x80},
		{0x2001, 0x23},
		{0x2002, 0x24},
		{0x2003, 0x25},
		{0x0080, 0x26},
		{0x0081, 0x27},
		{0x0082, 0x00}, {0x0083, 0x30},
		{0x0084, 0x00}, {0x0085, 0x30},
		{0x3000, 0x28}, {0x3001, 0x20},
		{0x3002, 0x29 },
		{0xFFFC, 0x00}, {0xFFFD, 0x80},
		
		{0x8000, 0xA9}, {0x8001, 0xFF},					// LDA #$FF
		{0x8002, 0x10}, {0x8003, 0x01},					// BPL $01
		{0x8004, 0xEA},									// NOP
		{0x8005, 0x78},									// SEI
		{0x8006, 0xAD}, {0x8007, 0x01}, {0x8008, 0x20}, // LDA $2001
		{0x8009, 0xA2}, {0x800A, 0x01},					// LDX #01
		{0x800B, 0xBD}, {0x800C, 0x01}, {0x800D, 0x20}, // LDA $2001, X
		{0x800E, 0xA0}, {0x800F, 0x02},					// LDY #02
		{0x8010, 0xB9}, {0x8011, 0x01}, {0x8012, 0x20}, // LDA $2001, Y
		{0x8013, 0xA5}, {0x8014, 0x80},					// LDA $80
		{0x8015, 0xB5}, {0x8016, 0x80},					// LDA $80, X
		{0x8017, 0xB6}, {0x8018, 0x80},					// LDX $80, Y
		{0x8019, 0xA2}, {0x801A, 0x01},
		{0x801B, 0xA1}, {0x801C, 0x81},					// LDA ($81, X)
		{0x801D, 0xB1}, {0x801E, 0x84},					// LDA ($84), Y
		{0x801F, 0x6C}, {0x8020, 0xFE}, {0x8021, 0x1F},	// JMP ($1FFE)

	};

	bool pass = false;

	Bus DataBus;
	struct colors_s {
		SDL_Color WHITE = { 255,255,255,255 };
		SDL_Color RED = { 255,0,0,255 };
		SDL_Color BLACK = { 0,0,0,255 };
	}COLORS;
	

	Sprite sprite = Sprite(256, 240);
	uint32_t cycle = 0;
	uint32_t scanline = 0;
	bool draw = true;
	bool frame_complete = false;
	
	struct resolveStruct {
		aven6502::instruction ins;
		int16_t pc;
	};
	string N = "N"; string O = "O"; string U = "U"; string B = "B"; string D = "D"; string I = "I"; string Z = "Z"; string C = "C";
	string ram1; string ram2; string ram3; string ram4; string ram5;
	void drawNoiseOrder();
	void DrawCPU();
	void DrawRAM();
	void resetDebug();
	int resolveRAM(resolveStruct args, string& addr);

	
};

void App::resetDebug() {
	std::stringstream b;
	string addr; int ind = 0;
	aven6502::instruction x1 = DataBus.CPU.lista[DataBus.read(DataBus.CPU.PC - 2)];
	b << "[ " << number2hex(DataBus.CPU.PC - 2, 1) << " ] - " << number2hex(DataBus.read(DataBus.CPU.PC - 2), 0); ram1 = b.str(); b.str(std::string());

	aven6502::instruction x2 = DataBus.CPU.lista[DataBus.read(DataBus.CPU.PC - 1)];
	b << "[ " << number2hex(DataBus.CPU.PC - 1, 1) << " ] - " << number2hex(DataBus.read(DataBus.CPU.PC - 1), 0); ram2 = b.str(); b.str(std::string());

	aven6502::instruction x3 = DataBus.CPU.lista[DataBus.read(DataBus.CPU.PC + 0)];
	ind = resolveRAM({ x3, (int16_t)DataBus.CPU.PC }, addr);
	b << "[ " << number2hex(DataBus.CPU.PC, 1) << " ] - " << number2hex(DataBus.read(DataBus.CPU.PC), 0) << " = " << x3.nombre << "  " << addr; ram3 = b.str(); b.str(std::string());

	aven6502::instruction x4 = DataBus.CPU.lista[DataBus.read(DataBus.CPU.PC + 1 + ind)];
	ind += resolveRAM({ x4, (int16_t)(DataBus.CPU.PC + 1 + ind) }, addr);
	b << "[ " << number2hex(DataBus.CPU.PC + 0 + ind, 1) << " ] - " << number2hex(DataBus.read(DataBus.CPU.PC + 0 + ind), 0) << " = " << x4.nombre << " - " << addr; ram4 = b.str(); b.str(std::string());

	aven6502::instruction x5 = DataBus.CPU.lista[DataBus.read(DataBus.CPU.PC + 2 + ind)];
	ind += resolveRAM({ x5, (int16_t)(DataBus.CPU.PC + 2 + ind) }, addr);
	b << "[ " << number2hex(DataBus.CPU.PC + 1 + ind, 1) << " ] - " << number2hex(DataBus.read(DataBus.CPU.PC + 1 + ind), 0) << " = " << x5.nombre << " - " << addr; ram5 = b.str(); b.str(std::string());
}

int App::resolveRAM(resolveStruct args, string& addr) {
	int ind = 0;

	string xd = "";
	
	if (args.ins.address == &aven6502::IMM) {
		
		addr = "#";
		xd = number2hex(DataBus.read(args.pc + 1), 2);
		addr += xd;
		ind = 1;
	}
	if (args.ins.address == &aven6502::REL) {
		addr = "";
		xd = number2hex(DataBus.read(args.pc + 1), 2);
		addr += xd;
		ind = 1;
	}
	if (args.ins.address == &aven6502::IMP) {
		addr = " ( IMP )";
		ind = 0;
	}
	if (args.ins.address == &aven6502::ABS) {
		
		addr = "";
		uint16_t lsb = DataBus.read(args.pc + 1);
		uint16_t msb = DataBus.read(args.pc + 2);
		xd = number2hex(lsb + (msb<<8), 3);
		addr += xd; 
		ind = 2;
	}
	if (args.ins.address == &aven6502::ABX) {
		addr = "";
		uint16_t lsb = DataBus.read(args.pc + 1);
		uint16_t msb = DataBus.read(args.pc + 2);
		xd = number2hex(lsb + (msb << 8), 3);
		addr += xd;
		addr += ", X ";
		ind = 2;
	}
	if (args.ins.address == &aven6502::ABY) {
		addr = "";
		uint16_t lsb = DataBus.read(args.pc + 1);
		uint16_t msb = DataBus.read(args.pc + 2);
		xd = number2hex(lsb + (msb << 8), 3);
		addr += xd;
		addr += ", Y ";
		ind = 2;
	}
	if (args.ins.address == &aven6502::ZP0) {
		addr = "";
		xd = number2hex(DataBus.read(args.pc + 1), 2);
		addr += xd;
		ind = 1;
	}
	if (args.ins.address == &aven6502::ZPX) {
		addr = "";
		xd = number2hex(DataBus.read(args.pc + 1), 2);
		addr += xd;
		addr += ", X";
		ind = 1;
	}
	if (args.ins.address == &aven6502::ZPY) {
		addr = "";
		xd = number2hex(DataBus.read(args.pc + 1), 2);
		addr += xd;
		addr += ", Y";
		ind = 1;
	}
	if (args.ins.address == &aven6502::IND) {
		addr = "(";
		uint16_t lsb = DataBus.read(args.pc + 1);
		uint16_t msb = DataBus.read(args.pc + 2);
		xd = number2hex(lsb + (msb << 8), 3);
		addr += xd;
		addr += ") ";
		ind = 2;
	}
	if (args.ins.address == &aven6502::IDX) {
		addr = "(";
		xd = number2hex(DataBus.read(args.pc + 1), 2);
		addr += xd;
		addr += ", X)";
		ind = 1;
	}
	if (args.ins.address == &aven6502::IDY) {
		addr = "(";
		xd = number2hex(DataBus.read(args.pc + 1), 2);
		addr += xd;
		addr += "), Y";
		ind = 1;
	}

	return ind;
}

void App::DrawRAM() {
	std::stringstream b;
	string addr; int ind1 = 0, ind2 = 0, ind3 = 0;
	aven6502::instruction x1 = DataBus.CPU.lista[DataBus.read(DataBus.CPU.PC - 2)];
	b << "[ " << number2hex(DataBus.CPU.PC - 2, 1) << " ] - " << number2hex(DataBus.read(DataBus.CPU.PC - 2), 0); ram1 = b.str(); b.str(std::string());

	aven6502::instruction x2 = DataBus.CPU.lista[DataBus.read(DataBus.CPU.PC - 1)];
	b << "[ " << number2hex(DataBus.CPU.PC - 1, 1) << " ] - " << number2hex(DataBus.read(DataBus.CPU.PC - 1), 0); ram2 = b.str(); b.str(std::string());

	aven6502::instruction x3 = DataBus.CPU.lista[DataBus.read(DataBus.CPU.PC + 0)];
	ind1 = resolveRAM({ x3, (int16_t)DataBus.CPU.PC }, addr);
	b << "[ " << number2hex(DataBus.CPU.PC, 1) << " ] - " << number2hex(DataBus.read(DataBus.CPU.PC), 0) << " = " << x3.nombre << "  " << addr; ram3 = b.str(); b.str(std::string());

	aven6502::instruction x4 = DataBus.CPU.lista[DataBus.read(DataBus.CPU.PC + ind1 + 1)];
	ind2 = resolveRAM({ x4, (int16_t)(DataBus.CPU.PC + ind1 + 1) }, addr);
	b << "[ " << number2hex(DataBus.CPU.PC + ind1 + 1, 1) << " ] - " << number2hex(DataBus.read(DataBus.CPU.PC + ind1 + 1), 0) << " = " << x4.nombre << "  " << addr; ram4 = b.str(); b.str(std::string());

	aven6502::instruction x5 = DataBus.CPU.lista[DataBus.read(DataBus.CPU.PC + ind1 + ind2 + 2)];
	ind3 = resolveRAM({ x5, (int16_t)(DataBus.CPU.PC + ind1 + ind2 + 2) }, addr);
	b << "[ " << number2hex(DataBus.CPU.PC + ind1 + ind2 + 2, 1) << " ] - " << number2hex(DataBus.read(DataBus.CPU.PC + ind1 + ind2 + 2), 0) << " = " << x5.nombre << "  " << addr; ram5 = b.str(); b.str(std::string());
	
	DrawString(10, 120, ram1, COLORS.WHITE);
	DrawString(10, 140, ram2, COLORS.WHITE);
	DrawString(10, 160, ram3, COLORS.RED);
	DrawString(10, 180, ram4, COLORS.WHITE);
	DrawString(10, 200, ram5, COLORS.WHITE);

	
}

void App::drawNoiseOrder() {
	sprite.SetPixel(cycle - 1, scanline, (rand() % 2 ? COLORS.WHITE : COLORS.BLACK));
	cycle++;
	if (cycle >= 341) {
		cycle = 0;
		scanline++;
		if (scanline >= 261) {
			scanline = -1;
			frame_complete = true;
		}
	}

}

void App::appCreate() {
	loadFont("C:\\Users\\juanp\\source\\repos\\2D-Engine-VS\\x64\\Debug\\fonts\\retro.ttf", 12);
	
	for (int i = 0; i < rom.size(); i++)
	{
		DataBus.write(rom[i].address, rom[i].byte);
	}
	DataBus.reset();
	
}

void App ::DrawCPU() {
	std::stringstream b;
	b << "P: " << number2hex(DataBus.CPU.P, 0) << " [" << (int)DataBus.CPU.P << "] "; string P = b.str(); b.str(std::string());
	b << "A: " << number2hex(DataBus.CPU.A, 0) << " [" << (int)DataBus.CPU.A << "] "; string A = b.str(); b.str(std::string());
	b << "X: " << number2hex(DataBus.CPU.X, 0) << " [" << (int)DataBus.CPU.X << "] "; string X = b.str(); b.str(std::string());
	b << "Y: " << number2hex(DataBus.CPU.Y, 0) << " [" << (int)DataBus.CPU.Y << "] "; string Y = b.str(); b.str(std::string());
	b << "S: " << number2hex(DataBus.CPU.SP, 0) << " [" << (int)DataBus.CPU.SP << "] "; string SP = b.str(); b.str(std::string());
	b << "PC: " << number2hex(DataBus.CPU.PC, 1); string PC = b.str(); b.str(std::string());

	b << "clock: " << (int ) DataBus.clock_counter ; string clock = b.str(); b.str(std::string());
	b << "need cycles: " << (int)DataBus.CPU.cycles; string need_cycle = b.str(); b.str(std::string());

	DrawString(145, 5, N, DataBus.CPU.GetFlag(aven6502::N) ? COLORS.RED : COLORS.WHITE); DrawString(160, 5, O, DataBus.CPU.GetFlag(aven6502::V) ? COLORS.RED : COLORS.WHITE);
	DrawString(175, 5, U, DataBus.CPU.GetFlag(aven6502::U) ? COLORS.RED : COLORS.WHITE); DrawString(190, 5, B, DataBus.CPU.GetFlag(aven6502::B) ? COLORS.RED : COLORS.WHITE);
	DrawString(205, 5, D, DataBus.CPU.GetFlag(aven6502::D) ? COLORS.RED : COLORS.WHITE); DrawString(220, 5, I, DataBus.CPU.GetFlag(aven6502::I) ? COLORS.RED : COLORS.WHITE);
	DrawString(235, 5, Z, DataBus.CPU.GetFlag(aven6502::Z) ? COLORS.RED : COLORS.WHITE); DrawString(250, 5, C, DataBus.CPU.GetFlag(aven6502::C) ? COLORS.RED : COLORS.WHITE);
	
	DrawString(10,  5, P, COLORS.WHITE);
	DrawString(10, 25, A, COLORS.WHITE);
	DrawString(10, 45, X, COLORS.WHITE);
	DrawString(10, 65, Y, COLORS.WHITE);
	DrawString(10, 85, SP, COLORS.WHITE);
	DrawString(145, 85, PC, COLORS.WHITE);
	DrawString(145, 25, clock, COLORS.WHITE);
	DrawString(145, 45, need_cycle, COLORS.WHITE);
}

void App::appUpdate() {
	if (getKeyPressed() == KEYS.SPACE) DataBus.clock();
	if (getKeyPressed() == KEYS.R) { DataBus.reset(); resetDebug(); }
	DrawCPU();
	DrawRAM();
	
		
	
}

int main(int argc, char* argv[]) {

	// 25-01-2023
	// Operaciones sobre el estado del procesador implementado.
	// Note: Leer sobre los modos de direccionamiento.

	// 28-01-2023
	// Modos de direccionamiento y sus ciclos.

	// 29-01-2023
	// Operaciones de transferencia de datos. // isNegative, isZero
	// Note: Proximo operaciones de shifteo de bits.

	// 30-01-2023
	// Operaciones de shifteo de bits // isCarry

	// 31-01-2023
	// Operaciones: sobre memoria, bit a bit, sobre registros, comparacion de bytes, Branch.
	// Temas: Cuando actualizar el Program Counter

	// 01-02-2023
	// Adicion y sustraccion
	// Revision de subrutinas

	// 02-02-2023
	// Instruccion BRK para debugging
	// Ciclos adicionales de branch

	// 06-02-2023
	// Reset y vector de opcodes
	// Se agrego el bus con memoria conectada por default.
	// Temas: Debuggear con SDL o Testing Google

	// 09-02-2023
	// Debug simple incorporado con mi motor 2D simple.

	// 09-02-2023
	// 

	App test;
	test.Setting("demo", 400, 400);
	test.Start();

	return 0;
}



