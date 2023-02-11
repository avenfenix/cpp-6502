#include "Bus.h"
#include <memory>
#include <iostream>

Bus::Bus()
{
	memory = std::make_shared<Memory>();
	memory->SetMemory();
	CPU.ConnectOnBus(this);
}

Bus::~Bus()
{
}

void Bus::clock()
{
	CPU.clock();
	clock_counter++;
}

void Bus::reset()
{
	CPU.reset();
	clock_counter = 0;
}

uint8_t Bus::read(uint16_t address)
{
	uint8_t data = 0x00;

	if (address >= 0 && address <= 0xFFFF) {
		this->memory->read(address, data);
	}

	return data;
}

void Bus::write(uint16_t address, uint8_t data)
{
	if (address >= 0 && address <= 0xFFFF) {
		this->memory->write(address, data);
	}
}

void Bus::loadRom(std::string path)
{
	std::ifstream stream(path, std::ifstream::binary);  // Stream de archivo binario.
	stream.read((char*)&memory->memory[0x8000], 32 * 1024);
}
