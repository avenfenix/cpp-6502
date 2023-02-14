#include "aven6502.h"
#include "Bus.h"
#include <iostream>
#include <vector>




using std::vector;

aven6502::aven6502()
{


	// olc - copiado directamente
	using a = aven6502;

	lista =
	{
		{ "BRK", &a::BRK, &a::IMM, 7 },{ "ORA", &a::ORA, &a::IDX, 6 },{ "???", &a::XXX, &a::IMP, 2 },{ "???", &a::XXX, &a::IMP, 8 },{ "???", &a::NOP, &a::IMP, 3 },{ "ORA", &a::ORA, &a::ZP0, 3 },{ "ASL", &a::ASL, &a::ZP0, 5 },{ "???", &a::XXX, &a::IMP, 5 },{ "PHP", &a::PHP, &a::IMP, 3 },{ "ORA", &a::ORA, &a::IMM, 2 },{ "ASL", &a::ASL, &a::IMP, 2 },{ "???", &a::XXX, &a::IMP, 2 },{ "???", &a::NOP, &a::IMP, 4 },{ "ORA", &a::ORA, &a::ABS, 4 },{ "ASL", &a::ASL, &a::ABS, 6 },{ "???", &a::XXX, &a::IMP, 6 },
		{ "BPL", &a::BPL, &a::REL, 2 },{ "ORA", &a::ORA, &a::IDY, 5 },{ "???", &a::XXX, &a::IMP, 2 },{ "???", &a::XXX, &a::IMP, 8 },{ "???", &a::NOP, &a::IMP, 4 },{ "ORA", &a::ORA, &a::ZPX, 4 },{ "ASL", &a::ASL, &a::ZPX, 6 },{ "???", &a::XXX, &a::IMP, 6 },{ "CLC", &a::CLC, &a::IMP, 2 },{ "ORA", &a::ORA, &a::ABY, 4 },{ "???", &a::NOP, &a::IMP, 2 },{ "???", &a::XXX, &a::IMP, 7 },{ "???", &a::NOP, &a::IMP, 4 },{ "ORA", &a::ORA, &a::ABX, 4 },{ "ASL", &a::ASL, &a::ABX, 7 },{ "???", &a::XXX, &a::IMP, 7 },
		{ "JSR", &a::JSR, &a::ABS, 6 },{ "AND", &a::AND, &a::IDX, 6 },{ "???", &a::XXX, &a::IMP, 2 },{ "???", &a::XXX, &a::IMP, 8 },{ "BIT", &a::BIT, &a::ZP0, 3 },{ "AND", &a::AND, &a::ZP0, 3 },{ "ROL", &a::ROL, &a::ZP0, 5 },{ "???", &a::XXX, &a::IMP, 5 },{ "PLP", &a::PLP, &a::IMP, 4 },{ "AND", &a::AND, &a::IMM, 2 },{ "ROL", &a::ROL, &a::IMP, 2 },{ "???", &a::XXX, &a::IMP, 2 },{ "BIT", &a::BIT, &a::ABS, 4 },{ "AND", &a::AND, &a::ABS, 4 },{ "ROL", &a::ROL, &a::ABS, 6 },{ "???", &a::XXX, &a::IMP, 6 },
		{ "BMI", &a::BMI, &a::REL, 2 },{ "AND", &a::AND, &a::IDY, 5 },{ "???", &a::XXX, &a::IMP, 2 },{ "???", &a::XXX, &a::IMP, 8 },{ "???", &a::NOP, &a::IMP, 4 },{ "AND", &a::AND, &a::ZPX, 4 },{ "ROL", &a::ROL, &a::ZPX, 6 },{ "???", &a::XXX, &a::IMP, 6 },{ "SEC", &a::SEC, &a::IMP, 2 },{ "AND", &a::AND, &a::ABY, 4 },{ "???", &a::NOP, &a::IMP, 2 },{ "???", &a::XXX, &a::IMP, 7 },{ "???", &a::NOP, &a::IMP, 4 },{ "AND", &a::AND, &a::ABX, 4 },{ "ROL", &a::ROL, &a::ABX, 7 },{ "???", &a::XXX, &a::IMP, 7 },
		{ "RTI", &a::RTI, &a::IMP, 6 },{ "EOR", &a::EOR, &a::IDX, 6 },{ "???", &a::XXX, &a::IMP, 2 },{ "???", &a::XXX, &a::IMP, 8 },{ "???", &a::NOP, &a::IMP, 3 },{ "EOR", &a::EOR, &a::ZP0, 3 },{ "LSR", &a::LSR, &a::ZP0, 5 },{ "???", &a::XXX, &a::IMP, 5 },{ "PHA", &a::PHA, &a::IMP, 3 },{ "EOR", &a::EOR, &a::IMM, 2 },{ "LSR", &a::LSR, &a::IMP, 2 },{ "???", &a::XXX, &a::IMP, 2 },{ "JMP", &a::JMP, &a::ABS, 3 },{ "EOR", &a::EOR, &a::ABS, 4 },{ "LSR", &a::LSR, &a::ABS, 6 },{ "???", &a::XXX, &a::IMP, 6 },
		{ "BVC", &a::BVC, &a::REL, 2 },{ "EOR", &a::EOR, &a::IDY, 5 },{ "???", &a::XXX, &a::IMP, 2 },{ "???", &a::XXX, &a::IMP, 8 },{ "???", &a::NOP, &a::IMP, 4 },{ "EOR", &a::EOR, &a::ZPX, 4 },{ "LSR", &a::LSR, &a::ZPX, 6 },{ "???", &a::XXX, &a::IMP, 6 },{ "CLI", &a::CLI, &a::IMP, 2 },{ "EOR", &a::EOR, &a::ABY, 4 },{ "???", &a::NOP, &a::IMP, 2 },{ "???", &a::XXX, &a::IMP, 7 },{ "???", &a::NOP, &a::IMP, 4 },{ "EOR", &a::EOR, &a::ABX, 4 },{ "LSR", &a::LSR, &a::ABX, 7 },{ "???", &a::XXX, &a::IMP, 7 },
		{ "RTS", &a::RTS, &a::IMP, 6 },{ "ADC", &a::ADC, &a::IDX, 6 },{ "???", &a::XXX, &a::IMP, 2 },{ "???", &a::XXX, &a::IMP, 8 },{ "???", &a::NOP, &a::IMP, 3 },{ "ADC", &a::ADC, &a::ZP0, 3 },{ "ROR", &a::ROR, &a::ZP0, 5 },{ "???", &a::XXX, &a::IMP, 5 },{ "PLA", &a::PLA, &a::IMP, 4 },{ "ADC", &a::ADC, &a::IMM, 2 },{ "ROR", &a::ROR, &a::IMP, 2 },{ "???", &a::XXX, &a::IMP, 2 },{ "JMP", &a::JMP, &a::IND, 5 },{ "ADC", &a::ADC, &a::ABS, 4 },{ "ROR", &a::ROR, &a::ABS, 6 },{ "???", &a::XXX, &a::IMP, 6 },
		{ "BVS", &a::BVS, &a::REL, 2 },{ "ADC", &a::ADC, &a::IDY, 5 },{ "???", &a::XXX, &a::IMP, 2 },{ "???", &a::XXX, &a::IMP, 8 },{ "???", &a::NOP, &a::IMP, 4 },{ "ADC", &a::ADC, &a::ZPX, 4 },{ "ROR", &a::ROR, &a::ZPX, 6 },{ "???", &a::XXX, &a::IMP, 6 },{ "SEI", &a::SEI, &a::IMP, 2 },{ "ADC", &a::ADC, &a::ABY, 4 },{ "???", &a::NOP, &a::IMP, 2 },{ "???", &a::XXX, &a::IMP, 7 },{ "???", &a::NOP, &a::IMP, 4 },{ "ADC", &a::ADC, &a::ABX, 4 },{ "ROR", &a::ROR, &a::ABX, 7 },{ "???", &a::XXX, &a::IMP, 7 },
		{ "???", &a::NOP, &a::IMP, 2 },{ "STA", &a::STA, &a::IDX, 6 },{ "???", &a::NOP, &a::IMP, 2 },{ "???", &a::XXX, &a::IMP, 6 },{ "STY", &a::STY, &a::ZP0, 3 },{ "STA", &a::STA, &a::ZP0, 3 },{ "STX", &a::STX, &a::ZP0, 3 },{ "???", &a::XXX, &a::IMP, 3 },{ "DEY", &a::DEY, &a::IMP, 2 },{ "???", &a::NOP, &a::IMP, 2 },{ "TXA", &a::TXA, &a::IMP, 2 },{ "???", &a::XXX, &a::IMP, 2 },{ "STY", &a::STY, &a::ABS, 4 },{ "STA", &a::STA, &a::ABS, 4 },{ "STX", &a::STX, &a::ABS, 4 },{ "???", &a::XXX, &a::IMP, 4 },
		{ "BCC", &a::BCC, &a::REL, 2 },{ "STA", &a::STA, &a::IDY, 6 },{ "???", &a::XXX, &a::IMP, 2 },{ "???", &a::XXX, &a::IMP, 6 },{ "STY", &a::STY, &a::ZPX, 4 },{ "STA", &a::STA, &a::ZPX, 4 },{ "STX", &a::STX, &a::ZPY, 4 },{ "???", &a::XXX, &a::IMP, 4 },{ "TYA", &a::TYA, &a::IMP, 2 },{ "STA", &a::STA, &a::ABY, 5 },{ "TXS", &a::TXS, &a::IMP, 2 },{ "???", &a::XXX, &a::IMP, 5 },{ "???", &a::NOP, &a::IMP, 5 },{ "STA", &a::STA, &a::ABX, 5 },{ "???", &a::XXX, &a::IMP, 5 },{ "???", &a::XXX, &a::IMP, 5 },
		{ "LDY", &a::LDY, &a::IMM, 2 },{ "LDA", &a::LDA, &a::IDX, 6 },{ "LDX", &a::LDX, &a::IMM, 2 },{ "???", &a::XXX, &a::IMP, 6 },{ "LDY", &a::LDY, &a::ZP0, 3 },{ "LDA", &a::LDA, &a::ZP0, 3 },{ "LDX", &a::LDX, &a::ZP0, 3 },{ "???", &a::XXX, &a::IMP, 3 },{ "TAY", &a::TAY, &a::IMP, 2 },{ "LDA", &a::LDA, &a::IMM, 2 },{ "TAX", &a::TAX, &a::IMP, 2 },{ "???", &a::XXX, &a::IMP, 2 },{ "LDY", &a::LDY, &a::ABS, 4 },{ "LDA", &a::LDA, &a::ABS, 4 },{ "LDX", &a::LDX, &a::ABS, 4 },{ "???", &a::XXX, &a::IMP, 4 },
		{ "BCS", &a::BCS, &a::REL, 2 },{ "LDA", &a::LDA, &a::IDY, 5 },{ "???", &a::XXX, &a::IMP, 2 },{ "???", &a::XXX, &a::IMP, 5 },{ "LDY", &a::LDY, &a::ZPX, 4 },{ "LDA", &a::LDA, &a::ZPX, 4 },{ "LDX", &a::LDX, &a::ZPY, 4 },{ "???", &a::XXX, &a::IMP, 4 },{ "CLV", &a::CLV, &a::IMP, 2 },{ "LDA", &a::LDA, &a::ABY, 4 },{ "TSX", &a::TSX, &a::IMP, 2 },{ "???", &a::XXX, &a::IMP, 4 },{ "LDY", &a::LDY, &a::ABX, 4 },{ "LDA", &a::LDA, &a::ABX, 4 },{ "LDX", &a::LDX, &a::ABY, 4 },{ "???", &a::XXX, &a::IMP, 4 },
		{ "CPY", &a::CPY, &a::IMM, 2 },{ "CMP", &a::CMP, &a::IDX, 6 },{ "???", &a::NOP, &a::IMP, 2 },{ "???", &a::XXX, &a::IMP, 8 },{ "CPY", &a::CPY, &a::ZP0, 3 },{ "CMP", &a::CMP, &a::ZP0, 3 },{ "DEC", &a::DEC, &a::ZP0, 5 },{ "???", &a::XXX, &a::IMP, 5 },{ "INY", &a::INY, &a::IMP, 2 },{ "CMP", &a::CMP, &a::IMM, 2 },{ "DEX", &a::DEX, &a::IMP, 2 },{ "???", &a::XXX, &a::IMP, 2 },{ "CPY", &a::CPY, &a::ABS, 4 },{ "CMP", &a::CMP, &a::ABS, 4 },{ "DEC", &a::DEC, &a::ABS, 6 },{ "???", &a::XXX, &a::IMP, 6 },
		{ "BNE", &a::BNE, &a::REL, 2 },{ "CMP", &a::CMP, &a::IDY, 5 },{ "???", &a::XXX, &a::IMP, 2 },{ "???", &a::XXX, &a::IMP, 8 },{ "???", &a::NOP, &a::IMP, 4 },{ "CMP", &a::CMP, &a::ZPX, 4 },{ "DEC", &a::DEC, &a::ZPX, 6 },{ "???", &a::XXX, &a::IMP, 6 },{ "CLD", &a::CLD, &a::IMP, 2 },{ "CMP", &a::CMP, &a::ABY, 4 },{ "NOP", &a::NOP, &a::IMP, 2 },{ "???", &a::XXX, &a::IMP, 7 },{ "???", &a::NOP, &a::IMP, 4 },{ "CMP", &a::CMP, &a::ABX, 4 },{ "DEC", &a::DEC, &a::ABX, 7 },{ "???", &a::XXX, &a::IMP, 7 },
		{ "CPX", &a::CPX, &a::IMM, 2 },{ "SBC", &a::SBC, &a::IDX, 6 },{ "???", &a::NOP, &a::IMP, 2 },{ "???", &a::XXX, &a::IMP, 8 },{ "CPX", &a::CPX, &a::ZP0, 3 },{ "SBC", &a::SBC, &a::ZP0, 3 },{ "INC", &a::INC, &a::ZP0, 5 },{ "???", &a::XXX, &a::IMP, 5 },{ "INX", &a::INX, &a::IMP, 2 },{ "SBC", &a::SBC, &a::IMM, 2 },{ "NOP", &a::NOP, &a::IMP, 2 },{ "???", &a::SBC, &a::IMP, 2 },{ "CPX", &a::CPX, &a::ABS, 4 },{ "SBC", &a::SBC, &a::ABS, 4 },{ "INC", &a::INC, &a::ABS, 6 },{ "???", &a::XXX, &a::IMP, 6 },
		{ "BEQ", &a::BEQ, &a::REL, 2 },{ "SBC", &a::SBC, &a::IDY, 5 },{ "???", &a::XXX, &a::IMP, 2 },{ "???", &a::XXX, &a::IMP, 8 },{ "???", &a::NOP, &a::IMP, 4 },{ "SBC", &a::SBC, &a::ZPX, 4 },{ "INC", &a::INC, &a::ZPX, 6 },{ "???", &a::XXX, &a::IMP, 6 },{ "SED", &a::SED, &a::IMP, 2 },{ "SBC", &a::SBC, &a::ABY, 4 },{ "NOP", &a::NOP, &a::IMP, 2 },{ "???", &a::XXX, &a::IMP, 7 },{ "???", &a::NOP, &a::IMP, 4 },{ "SBC", &a::SBC, &a::ABX, 4 },{ "INC", &a::INC, &a::ABX, 7 },{ "???", &a::XXX, &a::IMP, 7 },
	};
	// olc
}

aven6502::~aven6502()
{
}

uint8_t aven6502::fetch()
{
	return this->read(PC);
}

void aven6502::decode()
{
}

void aven6502::execute()
{
}


uint8_t aven6502::read(uint16_t address)
{
	return this->bus->read(address);
}

void aven6502::write(uint16_t address, uint8_t data)
{
	this->bus->write(address, data);
}

void aven6502::ConnectOnBus(Bus* bus) {
	std::shared_ptr<Bus> tmp(bus);
	this->bus = tmp;
}

void aven6502::reset() {
	A = 0x00;
	X = 0x00;
	Y = 0x00;
	P = 0x00;
	SP = 0xFD;
	uint8_t PCL = this->read(0xFFFC);
	uint8_t PCH = this->read(0xFFFD);

	PC = (((uint16_t)PCH) << 8) + ((uint16_t)PCL);

	cycles = 7; // El reset toma 7 ciclos de CPU
}

uint8_t aven6502::GetFlag(flags entry)
{
	return (P & entry) > 0 ? 1 : 0;
}

void aven6502::SetFlag(flags entry, bool value)
{
	if (value)
	{
		P = P | entry;
	}
	else
	{
		P = P & ~entry;
	}
}

void aven6502::isNegative(uint8_t byte)
{
	SetFlag(N, (((byte & 0b1000000) >> 7) == 0x01) ? true : false);
}

void aven6502::isZero(uint8_t byte)
{
	SetFlag(Z, byte == 0x00 ? true : false);
}

void aven6502::isCarry(uint8_t byte, Utils direction) {

	if (direction == RIGTH) {
		SetFlag(C, (byte & 0b00000001) == 0x01 ? true : false);
	}
	else if (direction == LEFT) {
		SetFlag(C, (byte & 0b10000000) == 0x01 ? true : false);
	}

}

void aven6502::isOverflow(uint8_t byte) {
	SetFlag(V, (byte >= 80) ? true : false);
}

void aven6502::irq() {

}

void aven6502::nmi() {

}

void aven6502::clock() {


	if (cycles == 0) {

		uint8_t opcode = fetch();

		uint8_t cycles_ad = (this->*lista[opcode].address)();
		uint8_t cycles_op = (this->*lista[opcode].execute)();

		cycles = lista[opcode].cycles + cycles_ad + cycles_op;

		complete = true;

		return;
	}

	cycles--;
}


// Direccionamiento

uint8_t aven6502::IMM(void) {
	address_absolute = ++PC;
	PC++;
	return 0x00;
}

uint8_t aven6502::ZP0(void)
{
	address_absolute = 0x00FF & read(++PC);
	PC++;
	return 0x00;
}

uint8_t aven6502::ZPX(void)
{
	address_absolute = 0x00FF & (read(++PC) + X);
	PC++;
	return 0x00;
}

uint8_t aven6502::ZPY(void)
{
	address_absolute = 0x00FF & (read(++PC) + Y);
	PC++;
	return 0x00;
}

uint8_t aven6502::ABS(void)
{
	uint16_t addr_low = read(++PC);
	uint16_t addr_high = read(++PC);
	address_absolute = (addr_high << 8) + addr_low;
	PC++;
	return 0x00;
}

uint8_t aven6502::ABX(void)
{
	uint16_t addr_low = read(++PC);
	uint16_t addr_high = read(++PC);
	addr_low += X;
	address_absolute = (addr_high << 8) + addr_low;
	if (addr_low > 0x00FF) {
		return 0x01;
	}
	PC++;
	return 0x00;
}

uint8_t aven6502::ABY(void)
{
	uint16_t addr_low = read(++PC);
	uint16_t addr_high = read(++PC);
	addr_low += Y;
	address_absolute = (addr_high << 8) + addr_low;
	if (addr_low > 0x00FF) {
		return 0x01;
	}
	PC++;
	return 0x00;
}

uint8_t aven6502::IND(void)
{
	uint16_t addr_low = read(++PC);
	uint16_t addr_high = read(++PC);
	uint16_t address = (addr_high << 8) + addr_low;
	addr_low = read(address + 0);
	addr_high = read(address + 1);
	address_absolute = (addr_high << 8) + addr_low;
	PC++;
	return 0x00;
}

uint8_t aven6502::IDX(void)
{
	uint16_t address = 0x00FF & (read(++PC) + X);
	uint16_t addr_low = read(address + 0);
	uint16_t addr_high = read(address + 1);
	address_absolute = (addr_high << 8) + addr_low;
	PC++;
	return 0x00;
}

uint8_t aven6502::IDY(void)
{
	uint16_t address = 0x00FF & read(++PC);
	uint16_t addr_low = read(address + 0);
	uint16_t addr_high = read(address + 1);
	addr_low += Y;
	address_absolute = (addr_high << 8) + addr_low;
	if (addr_low > 0x00FF) {
		return 0x01;
	}
	PC++;
	return 0x00;
}

uint8_t aven6502::REL(void)
{
	uint8_t unoperand = read(++PC);
	int8_t operand;
	memcpy(&operand, &unoperand, sizeof operand);
	address_relative = operand;

	uint8_t PCH = (uint8_t)((PC & 0xFF00) >> 8);
	PC += address_relative;
	bool cruzo = ((uint8_t)((PC & 0xFF00) >> 8)) != PCH ? true : false;

	if (cruzo) {
		return 0x02; // Add 2 ciclos si cruza la pagina.
	}
	else {
		return 0x01; // Add 1 ciclo si se mantiene en la pagina.
	}


}

uint8_t aven6502::IMP(void)
{
	PC++;
	return 0x00;
}


// Operaciones sobre la memoria

uint8_t aven6502::INC(void)
{
	if (read(address_absolute) == 0xFF) {
		write(address_absolute, 0x00);
		SetFlag(Z, true);
	}
	else {
		write(address_absolute, read(address_absolute) + 0x01);
		SetFlag(Z, false);
	}
	return 0x00;
}

uint8_t aven6502::DEC(void)
{
	if (read(address_absolute) == 0x00) {
		write(address_absolute, 0xFF);
	}
	else {
		write(address_absolute, read(address_absolute) - 0x01);
		if (read(address_absolute) == 0x00) {
			SetFlag(Z, true);
		}
		else {
			SetFlag(Z, false);
		}
	}
	return 0x00;
}

uint8_t aven6502::CMP(void)
{
	uint8_t resta = A - read(address_absolute);
	isZero(resta);
	isNegative(resta);
	if (A >= read(address_absolute)) {
		SetFlag(C, true);
	}
	else {
		SetFlag(C, false);
	}
	return 0x00;
}

uint8_t aven6502::CPX(void)
{
	uint8_t resta = X - read(address_absolute);
	isZero(resta);
	isNegative(resta);
	if (X >= read(address_absolute)) {
		SetFlag(C, true);
	}
	else {
		SetFlag(C, false);
	}
	return 0x00;
}

uint8_t aven6502::CPY(void)
{
	uint8_t resta = Y - read(address_absolute);
	isZero(resta);
	isNegative(resta);
	if (Y >= read(address_absolute)) {
		SetFlag(C, true);
	}
	else {
		SetFlag(C, false);
	}
	return 0x00;
}

uint8_t aven6502::BIT(void)
{
	uint8_t byte = read(address_absolute);
	SetFlag(N, (((0b10000000 & byte) >> 7) == 0x01) ? true : false);
	SetFlag(V, (((0b01000000 & byte) >> 6) == 0x01) ? true : false);
	byte &= A;
	isZero(byte);
	return 0x00;
}

uint8_t aven6502::LSR(void)
{
	uint8_t byte = read(address_absolute);
	isCarry(byte, RIGTH);
	byte >>= 1;
	write(address_absolute, byte);
	isZero(byte);
	SetFlag(N, false);
	return 0x00;
}

uint8_t aven6502::ASL(void)
{
	uint8_t byte = read(address_absolute);
	isCarry(byte, LEFT);
	byte <<=1;
	write(address_absolute, byte);
	isZero(byte);
	isNegative(byte);
	return 0x00;
}

uint8_t aven6502::ROL(void)
{
	uint8_t carry = GetFlag(C);
	uint8_t byte = read(address_absolute);
	isCarry(byte, LEFT);
	byte = byte << 1;
	byte = byte | carry;
	write(address_absolute, byte);
	isNegative(byte);
	isZero(byte);
	return 0x00;
}

uint8_t aven6502::ROR(void)
{
	uint8_t carry = GetFlag(C);
	uint8_t byte = read(address_absolute);
	isCarry(byte, RIGTH);
	byte = byte >> 1;
	byte = byte | (carry << 7);
	write(address_absolute, byte);
	isNegative(byte);
	isZero(byte);
	return 0x00;
}


// Transferencia de bytes

uint8_t aven6502::LDA(void)
{
	A = read(address_absolute);
	isNegative(A);
	isZero(A);
	return 0x00;
}

uint8_t aven6502::LDX(void)
{
	X = read(address_absolute);
	isNegative(X);
	isZero(X);
	return 0x00;
}

uint8_t aven6502::LDY(void)
{
	Y = read(address_absolute);
	isNegative(Y);
	isZero(Y);
	return 0x00;
}

uint8_t aven6502::STA(void)
{
	write(address_absolute, A);
	return 0x00;
}

uint8_t aven6502::STX(void)
{
	write(address_absolute, X);
	return 0x00;
}

uint8_t aven6502::STY(void)
{
	write(address_absolute, Y);
	return 0x00;
}

uint8_t aven6502::TAX(void)
{
	X = A;
	isNegative(X);
	isZero(X);
	return 0x00;
}

uint8_t aven6502::TAY(void)
{
	Y = A;
	isNegative(Y);
	isZero(Y);
	return 0x00;
}

uint8_t aven6502::TXA(void)
{
	A = X;
	isNegative(A);
	isZero(A);
	return 0x00;
}

uint8_t aven6502::TYA(void)
{
	A = Y;
	isNegative(A);
	isZero(A);
	return 0x00;
}

uint8_t aven6502::TXS(void)
{
	SP = X;
	return 0x00;
}

uint8_t aven6502::TSX(void)
{
	X = SP;
	isNegative(X);
	isZero(X);
	return 0x00;
}


// Operaciones matematicas

uint8_t aven6502::ADC()
{

	uint8_t carry = GetFlag(C);
	uint8_t bit7 = 0x00;
	int8_t op1 = (int8_t)A;
	int8_t op2 = (int8_t)read(address_absolute);

	int8_t cmp1 = op1;
	int8_t cmp2 = op2;
	uint8_t bit7_op1 = ((op1 & 0b10000000) >> 7);
	uint8_t bit7_op2 = ((op2 & 0b10000000) >> 7);

	if (bit7_op1 == 0x01) cmp1 = ~cmp1 + 1;
	if (bit7_op2 == 0x01) cmp2 = ~cmp2 + 1;

	if (cmp1 >= cmp2) {
		bit7 = bit7_op1;
	}
	else {
		bit7 = bit7_op2;
	}

	uint16_t sum = A + read(address_absolute) + carry;

	SetFlag(V, (sum & 0b10000000 >> 7) == bit7 ? false : true);

	if (sum > 0x00FF) {
		SetFlag(C, true);
		A = ((uint8_t)sum) & 0xFF;
	}
	else {
		SetFlag(C, false);
		A = ((uint8_t)sum);
	}
	isZero(A);
	isNegative(A);
	PC++;
	return 0x00;
}

uint8_t aven6502::SBC()
{
	uint8_t carry = GetFlag(C);
	uint8_t bit7 = 0x00;
	int8_t op1 = (int8_t)A;
	int8_t op2 = (int8_t)read(address_absolute);

	if (op1 >= op2) {
		SetFlag(C, true);
	}
	else {
		SetFlag(C, false);
	}

	op2 = ~op2; // Two complement
	op2 = op2 + carry; // True Two complement
	int16_t sum = op1 + op2;
	A = (uint8_t)sum;


	SetFlag(V, sum > 127 && sum < -127 ? true : false);
	isNegative(A);
	isZero(A);

	PC++;
	return 0x00;
}


// Operaciones bit a bit

uint8_t aven6502::AND()
{
	A = A & read(address_absolute);
	isNegative(A);
	isZero(A);
	return 0x00;
}

uint8_t aven6502::ORA()
{
	A = A | read(address_absolute);
	isNegative(A);
	isZero(A);
	return 0x00;
}
uint8_t aven6502::EOR()
{
	A = A ^ read(address_absolute);
	isNegative(A);
	isZero(A);
	return 0x00;
}


// Operaciones sobre los regitros

uint8_t aven6502::DEY(void)
{
	if (Y == 0x00) {
		Y = 0xFF;
	}
	else {
		Y -= 0x01;
		if (Y == 0x00) {
			SetFlag(Z, true);
		}
		else {
			SetFlag(Z, false);
		}
	}
	return 0x00;
}

uint8_t aven6502::DEX(void)
{
	if (X == 0x00) {
		X = 0xFF;
	}
	else {
		X -= 0x01;
		if (X == 0x00) {
			SetFlag(Z, true);
		}
		else {
			SetFlag(Z, false);
		}
	}
	return 0x00;
}

uint8_t aven6502::INX(void)
{
	if (X == 0xFF) {
		X = 0x00;
		SetFlag(Z, true);
	}
	else {
		X += 0x01;
		SetFlag(Z, false);
	}
	return 0x00;
}

uint8_t aven6502::INY(void)
{
	if (Y == 0xFF) {
		Y = 0x00;
		SetFlag(Z, true);
	}
	else {
		Y += 0x01;
		SetFlag(Z, false);
	}
	return 0x00;
}


// Branch y salto ** add 1 to cycles if branch occurs on same page

uint8_t aven6502::JMP()
{
	PC = address_absolute;
	return 0x00;
}

uint8_t aven6502::BPL(void)
{
	if (GetFlag(N) == falsebyte) PC += address_relative;
	return 0x00;
}

uint8_t aven6502::BMI(void)
{
	if (GetFlag(N) == truebyte) PC += address_relative;
	return 0x00;
}

uint8_t aven6502::BVC(void)
{
	if (GetFlag(V) == falsebyte) PC += address_relative;
	return 0x00;
}

uint8_t aven6502::BVS(void)
{
	if (GetFlag(V) == truebyte) PC += address_relative;
	return 0x00;
}

uint8_t aven6502::BCS(void)
{
	if (GetFlag(C) == truebyte) PC += address_relative;
	return 0x00;
}

uint8_t aven6502::BCC(void)
{
	if (GetFlag(C) == falsebyte) PC += address_relative;
	return 0x00;
}

uint8_t aven6502::BEQ(void)
{
	if (GetFlag(Z) == truebyte) PC += address_relative;
	return 0x00;
}

uint8_t aven6502::BNE(void)
{
	if (GetFlag(Z) == falsebyte) PC += address_relative;
	return 0x00;
}


// Manejo de subrutinas y pila de llamadas

uint8_t aven6502::JSR(void)
{
	uint8_t LSB_PC = (uint8_t)PC & 0x00FF;
	uint8_t MSB_PC = (uint8_t)(PC & 0xFF00 >> 8);
	write(0x0100 + SP--, MSB_PC);
	write(0x0100 + SP--, LSB_PC);
	PC = address_absolute;
	return 0x00;
}

uint8_t aven6502::RTS(void)
{
	uint8_t LSB = read(0x0100 + ++SP);
	write(SP, 0x00);
	uint8_t MSB = read(0x0100 + ++SP);
	write(SP, 0x00);
	PC = ((uint16_t)MSB << 8) + ((uint16_t)LSB);
	PC++;
	return 0x00;
}

uint8_t aven6502::PHA(void)
{
	write(0x0100 + SP--, A);
	PC++;
	return 0x00;
}

uint8_t aven6502::PHP(void)
{
	write(0x0100 + SP--, P);
	PC++;
	return 0x00;
}

uint8_t aven6502::PLA(void)
{
	A = read(0x0100 + ++SP);
	write(0x0100 + SP, 0x00);
	isNegative(A);
	isZero(A);
	PC++;
	return 0x00;
}

uint8_t aven6502::PLP(void)
{
	P = read(0x0100 + ++SP);
	write(SP, 0x00);
	PC++;
	return 0x00;
}


// Manejo de interrupciones

uint8_t aven6502::RTI()
{
	P = read(0x0100 + ++SP);
	write(SP, 0x00);

	uint8_t LSB = read(0x0100 + ++SP);
	write(SP, 0x00);
	uint8_t MSB = read(0x0100 + ++SP);
	write(SP, 0x00);
	PC = ((uint16_t)MSB << 8) + ((uint16_t)LSB);
	return 0x00;
}

uint8_t aven6502::BRK()
{
	SetFlag(I, true);
	write(0x0100 + SP--, (PC + 2 & 0x00FF)); // push PCH
	write(0x0100 + SP--, (PC + 2 & 0xFF00 >> 8)); // push PCL

	SetFlag(B, false);
	write(0x0100 + SP--, P);
	SetFlag(B, false);

	PC = (uint16_t)read(0xFFFE) | (uint16_t)(read(0xFFFF) << 8);
	return 0x00;
}

uint8_t aven6502::NOP()
{
	return 0x00;
}

uint8_t aven6502::XXX()
{
	return 0x00;
}


// Manejo del estado del procesador

uint8_t aven6502::SEC()
{
	SetFlag(C, true);
	return 0x00;
}

uint8_t aven6502::CLC()
{
	SetFlag(C, false);
	return 0x00;
}

uint8_t aven6502::CLV()
{
	SetFlag(V, false);
	return 0x00;
}

uint8_t aven6502::SEI()
{
	SetFlag(I, true);
	return 0x00;
}

uint8_t aven6502::CLI()
{
	SetFlag(I, false);
	return 0x00;
}

uint8_t aven6502::SED()
{
	SetFlag(D, true);
	return 0x00;
}

uint8_t aven6502::CLD()
{
	SetFlag(I, false);
	return 0x00;
}