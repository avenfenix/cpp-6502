#include "aven6502.h"
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


	

	return 0;
}

























/*uint16_t PC = 0x0012;
	uint16_t PCL = 0x000C;
	uint16_t PCH = 0x0000;

	int8_t addr_rel= -13;

	PCL += addr_rel;

	std::cout << std::hex << PCL << "\n";*/
// ----- SUMA -----
	/*uint8_t bit7 = 0x00;
	int8_t op1 = 0b10111110;
	int8_t op2 = 0b10111111;
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

	uint8_t suma = op1 + op2 + 1;
	std::bitset<8> x(suma);
	std::cout << x << std::endl;
	if (bit7 != ((suma & 0b10000000) >> 7)) {
		std::cout << "Overflow!\n";
	}
	else {
		std::cout << "Resultado valido!\n";
	}*/

	// ----- RESTA -----

	//uint8_t carry = 0x01;

	//uint8_t bit7 = 0x00;
	//int8_t op1 = 0b00000101; // Accumulator
	//int8_t op2 = 0b00000110; // From memory

	//if (op1 >= op2) {
	//	std::cout << "Sin borrow\n";
	//}
	//else {
	//	std::cout << "Con borrow\n";
	//}

	//op2 = ~op2; // Two complement
	//
	//op2 = op2 + carry; // True Two complement
	//int16_t sum = op1 + op2;

	//std::bitset<8> Abin(op1);
	//std::bitset<8> Mbin(op2);

	//std::cout << Abin << "\n";
	//std::cout << Mbin << "\n";
	//
	//std::bitset<8> sumbin(sum);
	//std::cout << "________" << "\n";
	//std::cout << sumbin << " = " << ((((sum & 0b10000000) >> 7) == 0x01) ? ' ' : '+') << (int) sum  << "\n";