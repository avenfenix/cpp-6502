#include "rockwell6502.h"

rockwell6502::rockwell6502()
{
	reset();
}

rockwell6502::~rockwell6502()
{
}

void rockwell6502::fetch()
{
}

void rockwell6502::decode()
{
}

void rockwell6502::execute()
{
}

uint8_t rockwell6502::read(uint16_t address)
{
	return 0;
}

void rockwell6502::write(uint16_t address, uint8_t data)
{
}

void rockwell6502::reset() {
	A = 0x00;
	X = 0x00;
	Y = 0x00;
	P = 0x00;
	SP = 0xFF;
	PC = 0x0000;
}

uint8_t rockwell6502::GetFlag(flags entry)
{
	return (P & entry) > 0 ? 1 : 0;
}

void rockwell6502::SetFlag(flags entry, bool value)
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

void rockwell6502::irq() {

}

void rockwell6502::nmi() {

}

void rockwell6502::clock() {

}


// Direccionamiento


uint8_t rockwell6502::IMM() {
	address_absolute = ++PC;
	return 0x00;
}

uint8_t rockwell6502::ZP0()
{
	address_absolute = 0x00FF & read(++PC);
	return 0x00;
}
uint8_t rockwell6502::ZPX()
{
	address_absolute = 0x00FF & (read(++PC) + X);
	return 0x00;
}
uint8_t rockwell6502::ZPY()
{
	address_absolute = 0x00FF & (read(++PC) + Y);
	return 0x00;
}

uint8_t rockwell6502::ABS()
{
	uint16_t addr_low  = read(++PC);
	uint16_t addr_high = read(++PC);
	address_absolute = (addr_high << 8) + addr_low;
	return 0x00;
}

uint8_t rockwell6502::ABX()
{
	uint16_t addr_low  = read(++PC);
	uint16_t addr_high = read(++PC);
	addr_low += X;
	address_absolute = (addr_high << 8) + addr_low; 
	if (addr_low > 0x00FF) {
		return 0x01;
	}
	return 0x00;
}

uint8_t rockwell6502::ABY()
{
	uint16_t addr_low  = read(++PC);
	uint16_t addr_high = read(++PC);
	addr_low += Y;
	address_absolute = (addr_high << 8) + addr_low;
	if (addr_low > 0x00FF) {
		return 0x01;
	}
	return 0x00;
}

uint8_t rockwell6502::IND()
{
	uint16_t addr_low  = read(++PC);
	uint16_t addr_high = read(++PC);
	uint16_t address = (addr_high << 8) + addr_low;
	addr_low  = read(address + 0);
	addr_high = read(address + 1);
	address_absolute = (addr_high << 8) + addr_low;
	return 0x00;
}

uint8_t rockwell6502::IDX()
{
	uint16_t address = 0x00FF & (read(++PC) + X);
	uint16_t addr_low  = read(address + 0);
	uint16_t addr_high = read(address + 1);
	address_absolute = (addr_high << 8) + addr_low;
	return 0x00;
}
uint8_t rockwell6502::IDY()
{
	uint16_t address = 0x00FF & read(++PC);
	uint16_t addr_low = read(address + 0);
	uint16_t addr_high = read(address + 1);
	addr_low += Y;
	address_absolute = (addr_high << 8) + addr_low;
	if (addr_low > 0x00FF) {
		return 0x01;
	}
	return 0x00;
}

uint8_t rockwell6502::REL()
{
	uint8_t unoperand = read(++PC);
	int8_t operand;
	memcpy(&operand, &unoperand, sizeof operand);
	address_relative = operand;
	return 0x00;
}

uint8_t rockwell6502::IMP()
{
	return 0x00;
}


// Instrucciones


void rockwell6502::INC()
{
}

void rockwell6502::DEC()
{
}

void rockwell6502::EOR()
{
}

void rockwell6502::CMP()
{
}

void rockwell6502::CPX()
{
}

void rockwell6502::CPY()
{
}

void rockwell6502::BIT()
{
}

void rockwell6502::LSR()
{
}

void rockwell6502::ASL()
{
}

void rockwell6502::ROL()
{
}

void rockwell6502::ROR()
{
}

void rockwell6502::JMP()
{
}
// Load accumulator with memory
void rockwell6502::LDA()
{
}

void rockwell6502::LDX()
{
}

void rockwell6502::LDY()
{
}

void rockwell6502::STA()
{
}

void rockwell6502::STX()
{
}

void rockwell6502::STY()
{
}

void rockwell6502::ADC()
{
}

void rockwell6502::SBC()
{
}

void rockwell6502::AND()
{
}

void rockwell6502::ORA()
{
}

void rockwell6502::TAX(void)
{
}

void rockwell6502::TAY(void)
{
}

void rockwell6502::TXA(void)
{
}

void rockwell6502::TYA(void)
{
}

void rockwell6502::TXS(void)
{
}

void rockwell6502::TSX(void)
{
}

void rockwell6502::DEY(void)
{
}

void rockwell6502::DEX(void)
{
}

void rockwell6502::INX(void)
{
}

void rockwell6502::INY(void)
{
}

void rockwell6502::LSRA(void)
{
}

void rockwell6502::ASLA(void)
{
}

void rockwell6502::ROLA(void)
{
}

void rockwell6502::RORA(void)
{
}

void rockwell6502::BMI(void)
{
}

void rockwell6502::BVC(void)
{
}

void rockwell6502::BPL(void)
{
}

void rockwell6502::BCS(void)
{
}

void rockwell6502::BVS(void)
{
}

void rockwell6502::BCC(void)
{
}

void rockwell6502::BEQ(void)
{
}

void rockwell6502::BNE(void)
{
}

void rockwell6502::JSR(void)
{
}

void rockwell6502::PHA(void)
{
}

void rockwell6502::RTS(void)
{
}

void rockwell6502::PHP(void)
{
}

void rockwell6502::PLA(void)
{
}

void rockwell6502::PLP(void)
{
}

void rockwell6502::RTI()
{
}

void rockwell6502::BRK()
{
}

void rockwell6502::NOP()
{
}
// Set carry
void rockwell6502::SEC()
{
	SetFlag(C, true);
}
// Clear carry
void rockwell6502::CLC()
{
	SetFlag(C, false);
}
// Clear overflow
void rockwell6502::CLV()
{
	SetFlag(O, false);
}
// Set interrupt disable
void rockwell6502::SEI()
{
	SetFlag(I, true);
}
// Clear interrupt disable
void rockwell6502::CLI()
{
	SetFlag(I, false);
}
// Set decimal mode
void rockwell6502::SED()
{
	SetFlag(D, true);
}
// Clear decimal mode
void rockwell6502::CLD()
{
	SetFlag(I, false);
}
