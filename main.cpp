#include "aven6502.h"
#include "Bus.h"
#include <iostream>
#include <string>
#include <iostream>
#include <bitset>

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

	struct memformat {
		uint16_t address;
		uint8_t byte;
	};

	std::vector<memformat> rom = 
	{
		{0xFFFC, 0x00},
		{0xFFFD, 0x80},
		{0x8000, 0xA9},
		{0x8001, 0xFF},
		{0x8002, 0xEA},

		{0x8003, 0xEA},
	};


	Bus DataBus = *(new Bus());

	for (int i = 0; i < rom.size(); i++)
	{
		DataBus.write(rom[i].address, rom[i].byte);
	}

	DataBus.reset();
	while (true) {
		DataBus.clock();
	}


	return 0;
}



