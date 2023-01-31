#pragma once
#include <inttypes.h>
#include <string>

class aven6502
{
public:
	aven6502();
	~aven6502();
	
	void fetch();
	void decode();
	void execute();

	uint8_t read(uint16_t address);
	void write(uint16_t address, uint8_t data);


public: 
	// Eventos
	void reset();
	void irq();
	void nmi();
	void clock();


public:
	struct instruction {
		std::string nombre;
		uint8_t (aven6502::*execute)(void) = nullptr; // Puntero a ejecutor de operacion
		uint8_t (aven6502::*address)(void) = nullptr; // Puntero al modo de addressing
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
	// Modos de direccionamiento - Retorna los ciclos necesarios para obtener la direccion.
	// Una instrucion toma tantos ciclos como ciclos donde hay accesos a la memoria + ciclos de calculo de direccion hechos por la naturaleza de los 8-bits
	
	uint8_t REL(); uint8_t IMM(); uint8_t IMP();
	uint8_t ABS(); uint8_t ABX(); uint8_t ABY();
	uint8_t ZP0(); uint8_t ZPX(); uint8_t ZPY();
	uint8_t IND(); uint8_t IDX(); uint8_t IDY();
	
	//	Note: En mi emulacion, el Program Counter cambiara cuando un addressing mode es llamado. 
	// O en la instruccion en caso de un branch o una de modo Implied. 

	uint16_t address_absolute = 0x0000;
	uint8_t address_relative = 0x00;
	uint16_t cycles = 0x0000;
	uint16_t clock_counter = 0x0000;


public:
	uint8_t A = 0x00;		// ACUMULADOR
	uint8_t X = 0x00;		// INDICE X
	uint8_t Y = 0x00;		// INDICE Y
	uint8_t SP = 0x00;		// STACK POINTER	// - Fijado por hardware a la página $01 de la memoria, es decir la gama de direcciones $0100-$01FF (256511)
	uint8_t P = 0x00;		// STATUS
	uint16_t PC = 0x0000;	// PROGRAM COUNTER

	enum flags {
		C = 0x01 << 0,	// Carry
		Z = 0x01 << 1,	// Zero
		I = 0x01 << 2,	// Interrupt Disable
		D = 0x01 << 3,	// Decimal
		B = 0x01 << 4,	// B flag
		U = 0x01 << 5,	// No usado
		V = 0x01 << 6,	// Overflow
		N = 0x01 << 7,	// Negative
	};

	uint8_t GetFlag(flags entry);
	void SetFlag(flags entry, bool value);


public: // utilidades
	enum Utils {
		LEFT,
		RIGTH
	};

	void isNegative(uint8_t byte);
	void isZero(uint8_t byte);
	void isCarry(uint8_t byte, Utils direction);
	void isOverflow(uint8_t byte);

};

