#include "emulationGUI.h"
#include <QtWidgets/QApplication>

#include "SDL.h"


int main(int argc, char *argv[])
{

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
	// Fixes: Debug RAM * IMPLIED PC Behaviour * JMP implementacion. 
	// Add: xxx

	// 10-02-2023
	// Carga de binario implementado.

	// 11-02-2023
	// Cambio de interfaz grafica y debugger realizado con Qt.
	// Falta manejar todas las interrupciones
    QApplication a(argc, argv);
    emulationGUI w;
    w.show();


    return a.exec();
}
