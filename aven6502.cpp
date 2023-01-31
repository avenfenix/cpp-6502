#include "aven6502.h"

aven6502::aven6502()
{
	reset();
}

aven6502::~aven6502()
{
}

void aven6502::fetch()
{
}

void aven6502::decode()
{
}

void aven6502::execute()
{
}

uint8_t aven6502::read(uint16_t address)
{
	return 0;
}

void aven6502::write(uint16_t address, uint8_t data)
{
}

void aven6502::reset() {
	A = 0x00;
	X = 0x00;
	Y = 0x00;
	P = 0x00;
	SP = 0xFD;
	PC = 0x0000;
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
	
}

void aven6502::irq() {

}

void aven6502::nmi() {

}

void aven6502::clock() {

}


// Direccionamiento

uint8_t aven6502::IMM() {
	address_absolute = ++PC;
	PC++;
	return 0x00;
}

uint8_t aven6502::ZP0()
{
	address_absolute = 0x00FF & read(++PC);
	PC++;
	return 0x00;
}

uint8_t aven6502::ZPX()
{
	address_absolute = 0x00FF & (read(++PC) + X);
	PC++;
	return 0x00;
}

uint8_t aven6502::ZPY()
{
	address_absolute = 0x00FF & (read(++PC) + Y);
	PC++;
	return 0x00;
}

uint8_t aven6502::ABS()
{
	uint16_t addr_low  = read(++PC);
	uint16_t addr_high = read(++PC);
	address_absolute = (addr_high << 8) + addr_low;
	PC++;
	return 0x00;
}

uint8_t aven6502::ABX()
{
	uint16_t addr_low  = read(++PC);
	uint16_t addr_high = read(++PC);
	addr_low += X;
	address_absolute = (addr_high << 8) + addr_low; 
	if (addr_low > 0x00FF) {
		return 0x01;
	}
	PC++;
	return 0x00;
}

uint8_t aven6502::ABY()
{
	uint16_t addr_low  = read(++PC);
	uint16_t addr_high = read(++PC);
	addr_low += Y;
	address_absolute = (addr_high << 8) + addr_low;
	if (addr_low > 0x00FF) {
		return 0x01;
	}
	PC++;
	return 0x00;
}

uint8_t aven6502::IND()
{
	uint16_t addr_low  = read(++PC);
	uint16_t addr_high = read(++PC);
	uint16_t address = (addr_high << 8) + addr_low;
	addr_low  = read(address + 0);
	addr_high = read(address + 1);
	address_absolute = (addr_high << 8) + addr_low;
	PC++;
	return 0x00;
}

uint8_t aven6502::IDX()
{
	uint16_t address = 0x00FF & (read(++PC) + X);
	uint16_t addr_low  = read(address + 0);
	uint16_t addr_high = read(address + 1);
	address_absolute = (addr_high << 8) + addr_low;
	PC++;
	return 0x00;
}

uint8_t aven6502::IDY()
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

uint8_t aven6502::REL()
{
	uint8_t unoperand = read(++PC);
	int8_t operand;
	memcpy(&operand, &unoperand, sizeof operand);
	address_relative = operand;
	return 0x00;
}

uint8_t aven6502::IMP()
{
	return 0x00;
}


// Operaciones sobre la memoria

void aven6502::INC()
{
	if (read(address_absolute) == 0xFF) {
		write(address_absolute, 0x00);
		SetFlag(Z, true);
	}
	else {
		write(address_absolute, read(address_absolute) + 0x01);
		SetFlag(Z, false);
	}
}

void aven6502::DEC()
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
}

void aven6502::CMP()
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
}

void aven6502::CPX()
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
}

void aven6502::CPY()
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
}

void aven6502::BIT()
{
	uint8_t byte = read(address_absolute);
	SetFlag(N, (((0b10000000 & byte) >> 7) == 0x01) ? true : false);
	SetFlag(V, (((0b01000000 & byte) >> 6) == 0x01) ? true : false);
	byte &= A;
	isZero(byte);
}

void aven6502::LSR()
{
	uint8_t byte = read(address_absolute);
	isCarry(byte, RIGTH);
	byte >> 1;
	write(address_absolute, byte);
	isZero(byte);
	SetFlag(N, false);
}

void aven6502::ASL()
{
	uint8_t byte = read(address_absolute);
	isCarry(byte, LEFT);
	byte << 1;
	write(address_absolute, byte);
	isZero(byte);
	isNegative(byte);
}

void aven6502::ROL()
{
	uint8_t carry = GetFlag(C);
	uint8_t byte = read(address_absolute);
	isCarry(byte, LEFT);
	byte = byte << 1;
	byte = byte | carry;
	write(address_absolute, byte);
	isNegative(byte);
	isZero(byte);
}

void aven6502::ROR()
{
	uint8_t carry = GetFlag(C);
	uint8_t byte = read(address_absolute);
	isCarry(byte, RIGTH);
	byte = byte >> 1;
	byte = byte | (carry << 7);
	write(address_absolute, byte);
	isNegative(byte);
	isZero(byte);
}


// Transferencia de bytes

void aven6502::LDA()
{
	A = read(address_absolute);
	isNegative(A);
	isZero(A);
}

void aven6502::LDX()
{
	X = read(address_absolute);
	isNegative(X);
	isZero(X);
}

void aven6502::LDY()
{
	Y = read(address_absolute);
	isNegative(Y);
	isZero(Y);
}

void aven6502::STA()
{
	write(address_absolute, A);
}

void aven6502::STX()
{
	write(address_absolute, X);
}

void aven6502::STY()
{
	write(address_absolute, Y);
}

void aven6502::TAX(void)
{
	X = A;
	isNegative(X);
	isZero(X);
}

void aven6502::TAY(void)
{
	Y = A;
	isNegative(Y);
	isZero(Y);
}

void aven6502::TXA(void)
{
	A = X;
	isNegative(A);
	isZero(A);
}

void aven6502::TYA(void)
{
	A = Y;
	isNegative(A);
	isZero(A);
}

void aven6502::TXS(void)
{
	SP = X;
}

void aven6502::TSX(void)
{
	X = SP;
	isNegative(X);
	isZero(X);
}


// Operaciones matematicas

void aven6502::ADC()
{
}

void aven6502::SBC()
{
}


// Operaciones bit a bit

void aven6502::AND()
{
	A = A & read(address_absolute);
	isNegative(A);
	isZero(A);
}

void aven6502::ORA()
{
	A = A | read(address_absolute);
	isNegative(A);
	isZero(A);
}
void aven6502::EOR()
{
	A = A ^ read(address_absolute);
	isNegative(A);
	isZero(A);
}


// Operaciones sobre los regitros

void aven6502::DEY(void)
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
}

void aven6502::DEX(void)
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
}

void aven6502::INX(void)
{
	if (X == 0xFF) {
		X = 0x00;
		SetFlag(Z, true);
	}
	else {
		X += 0x01;
		SetFlag(Z, false);
	}
}

void aven6502::INY(void)
{
	if (Y == 0xFF) {
		Y = 0x00;
		SetFlag(Z, true);
	}
	else {
		Y += 0x01;
		SetFlag(Z, false);
	}
}


// Branch y salto

void aven6502::JMP()
{
	uint16_t low = read(address_absolute);
	uint16_t high = read(address_absolute + 1) << 8;
	PC = high + low;
}

void aven6502::BPL(void)
{
	if (GetFlag(N) == false) PC = address_relative;
}

void aven6502::BMI(void)
{
	if (GetFlag(N) == true) PC = address_relative;
}

void aven6502::BVC(void)
{
	if (GetFlag(V) == false) PC = address_relative;
}

void aven6502::BVS(void)
{
	if (GetFlag(V) == true) PC = address_relative;
}

void aven6502::BCS(void)
{
	if (GetFlag(C) == true) PC = address_relative;
}

void aven6502::BCC(void)
{
	if (GetFlag(C) == false) PC = address_relative;
}

void aven6502::BEQ(void)
{
	if (GetFlag(Z) == true) PC = address_relative;
}

void aven6502::BNE(void)
{
	if (GetFlag(Z) == false) PC = address_relative;
}


// Manejo de subrutinas y pila de llamadas

void aven6502::JSR(void)
{
}

void aven6502::PHA(void)
{
}

void aven6502::RTS(void)
{
}

void aven6502::PHP(void)
{
}

void aven6502::PLA(void)
{
}

void aven6502::PLP(void)
{
}


// Manejo de interrupciones

void aven6502::RTI()
{
}

void aven6502::BRK()
{
}

void aven6502::NOP()
{
}


// Manejo del estado del procesador

void aven6502::SEC()
{
	SetFlag(C, true);
}

void aven6502::CLC()
{
	SetFlag(C, false);
}

void aven6502::CLV()
{
	SetFlag(V, false);
}

void aven6502::SEI()
{
	SetFlag(I, true);
}

void aven6502::CLI()
{
	SetFlag(I, false);
}

void aven6502::SED()
{
	SetFlag(D, true);
}

void aven6502::CLD()
{
	SetFlag(I, false);
}
