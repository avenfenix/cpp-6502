#pragma once
#include <inttypes.h>
#include <string>
#include <vector>
#include "aven6502.h"



struct Memory {
	int memorySize = 64 * 1024;
	std::vector<uint8_t> memory;
	void write(uint16_t address, uint8_t data) {
		memory[address] = data;
	}
	void read(uint16_t address, uint8_t& data) {
		data = memory[address];
	}
	void SetMemory() {
		memory.resize(memorySize, (uint8_t)0x00);
	}
};

class Bus
{
public:
	Bus();
	~Bus();

public:
	void reset();
	void clock();
	

public:
	uint8_t read(uint16_t address);
	void write(uint16_t address, uint8_t data);

public:
	std::shared_ptr<Memory> memory;
public:
	aven6502 CPU;
};

