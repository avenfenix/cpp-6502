#pragma once
#include <inttypes.h>
#include <string>
#include <vector>
#include <memory>

using std::vector;

class Bus;

class aven6502
{
public:
	aven6502();
	~aven6502();

	uint8_t fetch();
	void decode();
	void execute();

	uint8_t read(uint16_t address);
	void write(uint16_t address, uint8_t data);

	// La cpu tiene que leer y escribir en el bus, este bus esta configurado de la manera que nosotros queramos. Seria bueno
	// tener una clase Bus que nos permita conectar distintos dispositivos a este bus como la cpu, ram, almacenamiento, etc. 
	// de manera sencilla.
	// * Para probar utilizare solo un arreglo de bytes que contenga todo el rango de direcciones posible.

public:
	// Dispositivos
	std::shared_ptr<Bus> bus;
	void ConnectOnBus(Bus* bus);

public:
	// Eventos
	void reset();
	void irq();
	void nmi();
	void clock();

public:
	struct instruction {
		std::string nombre;
		uint8_t(aven6502::* execute)(void) = nullptr; // Puntero a ejecutor de operacion
		uint8_t(aven6502::* address)(void) = nullptr; // Puntero al modo de addressing
		uint8_t cycles = 0;
	};
	
	vector<instruction> lista;

	// Estado del procesador
	uint8_t SEC(); uint8_t CLC(); uint8_t CLV();
	uint8_t SEI(); uint8_t CLI(); uint8_t SED();
	uint8_t CLD();

	// Transferencia de bytes
	uint8_t LDA(); uint8_t LDX(); uint8_t LDY();
	uint8_t STA(); uint8_t STX(); uint8_t STY();
	uint8_t TAX(); uint8_t TAY(); uint8_t TXA();
	uint8_t TXS(); uint8_t TYA(); uint8_t TSX();

	// Operacion matematica basica
	uint8_t ADC(); uint8_t SBC();

	// Operaciones bit a bit
	uint8_t AND(); uint8_t ORA(); uint8_t EOR();

	// Operaciones sobre registros
	uint8_t INX(void); uint8_t INY(void);
	uint8_t DEY(void);	uint8_t DEX(void);

	// Operaciones sobre la memoria
	uint8_t INC();	uint8_t DEC();

	// Comparacion de bytes
	uint8_t CMP(); uint8_t CPX();	uint8_t CPY();

	// Para testear bits
	uint8_t BIT();

	// Operaciones para shiftear bits
	uint8_t LSR(); uint8_t ASL();
	uint8_t ROR(); uint8_t ROL();

	// Salto - JUMP
	uint8_t JMP();

	// Branch
	uint8_t BMI(void); uint8_t BVC(void);
	uint8_t BPL(void); uint8_t BCS(void);
	uint8_t BVS(void); uint8_t BCC(void);
	uint8_t BEQ(void); uint8_t BNE(void);

	// Operaciones para manejar subrutinas y pila de llamadas
	uint8_t RTS(void); uint8_t JSR(void);
	uint8_t PHA(void); uint8_t PHP(void);
	uint8_t PLA(void); uint8_t PLP(void);


	// Operacion para manejar interrupciones
	uint8_t RTI(); uint8_t BRK();

	// Otras
	uint8_t NOP();

	uint8_t XXX();


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
