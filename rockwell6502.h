#pragma once
#include <inttypes.h>
#include <string>

class rockwell6502
{
public:
	rockwell6502();
	~rockwell6502();
	
	void fetch();
	void decode();
	void execute();

public:
	struct instruction {
		std::string nombre;
		uint8_t (rockwell6502::*execute)(void) = nullptr; // Puntero a ejecutor de operacion
		uint8_t (rockwell6502::*address)(void) = nullptr; // Puntero al modo de addressing
		uint8_t cycles = 0;
	} lookup;

	// Estado del procesador
	void SEC(); void CLC(); void CLV();
	void SEI(); void CLI(); void SED();
	void CLD();

	// Transferencia de bytes
	void LDA(); void LDX(); void LDY();
	void STA(); void STX(); void STY();
	void TAX(); void TAY(); void TXA();
	void TXS(); void TYA(); void TSX();

	// Operacion matematica basica
	void ADC(); void SBC(); 
	
	// Operaciones bit a bit
	void AND(); void ORA(); void EOR();

	// Operaciones sobre registros
	void INX(void); void INY(void);
	void DEY(void);	void DEX(void);

	// Operaciones sobre la memoria
	void INC();	void DEC();

	// Comparacion de bytes
	void CMP(); void CPX();	void CPY();
	
	// Para testear bits
	void BIT(); 

	// Operaciones para shiftear bits
	void LSR(); void ASL(); 
	void ROR(); void ROL();
	void LSRA(void); void ASLA(void);
	void ROLA(void); void RORA(void);
	
	// Salto - JUMP
	void JMP();

	// Branch
	void BMI(void); void BVC(void);
	void BPL(void); void BCS(void);
	void BVS(void); void BCC(void);
	void BEQ(void); void BNE(void);
	
	// Operaciones para manejar subrutinas y pila de llamadas
	void RTS(void); void JSR(void);
	void PHA(void); void PHP(void);
	void PLA(void); void PLP(void);


	// Operacion para manejar interrupciones
	void RTI(); void BRK();  

	// Otras
	void NOP(); 

public:
	// Eventos
	void reset();
	void irq();
	void nmi();
	void clock();
	
public: 
	// Modos de direccionamiento
	void ABS(void); void ABX(void); void ABY(void);
	void ZP0(void); void ZPX(void); void ZPY(void);
	void IND(void); void IDX(void); void IDY(void);
	void REL(void);

	uint16_t address_absolute = 0x0000;
	uint16_t address_relative = 0x0000;
	uint16_t cycles = 0x0000;
	uint16_t clock_counter = 0x0000;

public:
	uint8_t A = 0x00;		// ACUMULADOR
	uint8_t X = 0x00;		// INDICE X
	uint8_t Y = 0x00;		// INDICE Y
	uint8_t SP = 0x00;		// STACK POINTER	// - Fijado por hardware a la p�gina $01 de la memoria, es decir la gama de direcciones $0100-$01FF (256511)
	uint8_t P = 0x00;		// STATUS
	uint16_t PC = 0x0000;	// PROGRAM COUNTER

	enum flags {
		C = 0x01 << 0,	// Carry
		Z = 0x01 << 1,	// Zero
		I = 0x01 << 2,	// Interrupt Disable
		D = 0x01 << 3,	// Decimal
		B = 0x01 << 4,	// B flag
		U = 0x01 << 5,	// No usado
		O = 0x01 << 6,	// Overflow
		N = 0x01 << 7,	// Negative
	};

	uint8_t GetFlag(flags entry);
	void SetFlag(flags entry, bool value);
	

};

