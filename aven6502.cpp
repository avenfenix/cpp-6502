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
	SetFlag(V, (byte >= 80) ? true : false);
}

void aven6502::irq() {

}

void aven6502::nmi() {

}

void aven6502::clock() {

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
	uint16_t addr_low  = read(++PC);
	uint16_t addr_high = read(++PC);
	address_absolute = (addr_high << 8) + addr_low;
	PC++;
	return 0x00;
}

uint8_t aven6502::ABX(void)
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

uint8_t aven6502::ABY(void)
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

uint8_t aven6502::IND(void)
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

uint8_t aven6502::IDX(void)
{
	uint16_t address = 0x00FF & (read(++PC) + X);
	uint16_t addr_low  = read(address + 0);
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

	uint8_t PCH = (uint8_t) ((PC & 0xFF00) >> 8);
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
	return 0x00;
}


// Operaciones sobre la memoria

void aven6502::INC(void)
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

void aven6502::DEC(void)
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

void aven6502::CMP(void)
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

void aven6502::CPX(void)
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

void aven6502::CPY(void)
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

void aven6502::BIT(void)
{
	uint8_t byte = read(address_absolute);
	SetFlag(N, (((0b10000000 & byte) >> 7) == 0x01) ? true : false);
	SetFlag(V, (((0b01000000 & byte) >> 6) == 0x01) ? true : false);
	byte &= A;
	isZero(byte);
}

void aven6502::LSR(void)
{
	uint8_t byte = read(address_absolute);
	isCarry(byte, RIGTH);
	byte >> 1;
	write(address_absolute, byte);
	isZero(byte);
	SetFlag(N, false);
}

void aven6502::ASL(void)
{
	uint8_t byte = read(address_absolute);
	isCarry(byte, LEFT);
	byte << 1;
	write(address_absolute, byte);
	isZero(byte);
	isNegative(byte);
}

void aven6502::ROL(void)
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

void aven6502::ROR(void)
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

void aven6502::LDA(void)
{
	A = read(address_absolute);
	isNegative(A);
	isZero(A);
}

void aven6502::LDX(void)
{
	X = read(address_absolute);
	isNegative(X);
	isZero(X);
}

void aven6502::LDY(void)
{
	Y = read(address_absolute);
	isNegative(Y);
	isZero(Y);
}

void aven6502::STA(void)
{
	write(address_absolute, A);
}

void aven6502::STX(void)
{
	write(address_absolute, X);
}

void aven6502::STY(void)
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

	uint8_t carry = GetFlag(C);
	uint8_t bit7 = 0x00;
	int8_t op1 = (int8_t) A;
	int8_t op2 = (int8_t) read(address_absolute);

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
}

void aven6502::SBC()
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


// Branch y salto ** add 1 to cycles if branch occurs on same page

void aven6502::JMP()
{
	uint16_t low = read(address_absolute);
	uint16_t high = read(address_absolute + 1) << 8;
	PC = high + low;
}

void aven6502::BPL(void)
{
	if (GetFlag(N) == false) PC += address_relative;
}

void aven6502::BMI(void)
{
	if (GetFlag(N) == true) PC += address_relative;
}

void aven6502::BVC(void)
{
	if (GetFlag(V) == false) PC += address_relative;
}

void aven6502::BVS(void)
{
	if (GetFlag(V) == true) PC += address_relative;
}

void aven6502::BCS(void)
{
	if (GetFlag(C) == true) PC += address_relative;
}

void aven6502::BCC(void)
{
	if (GetFlag(C) == false) PC += address_relative;
}

void aven6502::BEQ(void)
{
	if (GetFlag(Z) == true) PC += address_relative;
}

void aven6502::BNE(void)
{
	if (GetFlag(Z) == false) PC += address_relative;
}


// Manejo de subrutinas y pila de llamadas

void aven6502::JSR(void)
{
	uint8_t LSB_PC = (uint8_t)PC & 0x00FF;
	uint8_t MSB_PC = (uint8_t)(PC & 0xFF00 >> 8);
	write(0x0100 + SP--, MSB_PC);
	write(0x0100 + SP--, LSB_PC);
	PC = address_absolute;
}

void aven6502::RTS(void)
{
	uint8_t LSB = read(0x0100 + ++SP);
	write(SP, 0x00);
	uint8_t MSB = read(0x0100 + ++SP);
	write(SP, 0x00);
	PC = ((uint16_t)MSB << 8) + ((uint16_t)LSB); 
	PC++;
}

void aven6502::PHA(void)
{
	write(0x0100 + SP--, A);
	PC++;
}

void aven6502::PHP(void)
{
	write(0x0100 + SP--, P);
	PC++;
}

void aven6502::PLA(void)
{
	A = read(0x0100 + ++SP);
	write(0x0100 + SP, 0x00);
	isNegative(A);
	isZero(A);
	PC++;
}

void aven6502::PLP(void)
{
	P = read(0x0100 + ++SP);
	write(SP, 0x00);
	PC++;
}


// Manejo de interrupciones

void aven6502::RTI()
{
	P = read(0x0100 + ++SP);
	write(SP, 0x00);

	uint8_t LSB = read(0x0100 + ++SP);
	write(SP, 0x00);
	uint8_t MSB = read(0x0100 + ++SP);
	write(SP, 0x00);
	PC = ((uint16_t)MSB << 8) + ((uint16_t)LSB);
}

void aven6502::BRK()
{
	SetFlag(I, true);
	write(0x0100 + SP--, (PC + 2 & 0x00FF)); // push PCH
	write(0x0100 + SP--, (PC + 2 & 0xFF00 >> 8)); // push PCL

	SetFlag(B, false);
	write(0x0100 + SP--, P);
	SetFlag(B, false);

	PC = (uint16_t)read(0xFFFE) | (uint16_t)(read(0xFFFF) << 8);
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
