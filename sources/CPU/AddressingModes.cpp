//
// Created by anonymus-raccoon on 3/20/20.
//

#include "../Models/Int24.hpp"
#include "CPU.hpp"

namespace ComSquare::CPU
{
	uint24_t CPU::_getImmediateAddrForA()
	{
		uint24_t effective = this->_registers.pac++;
		if (!this->_registers.p.m)
			this->_registers.pac++;
		return effective;
	}

	uint24_t CPU::_getImmediateAddrForX()
	{
		uint24_t effective = this->_registers.pac++;
		if (!this->_registers.p.x_b)
			this->_registers.pac++;
		return effective;
	}

	uint24_t CPU::_getDirectAddr()
	{
		uint8_t addr = this->_bus->read(this->_registers.pac++);
		return this->_registers.d + addr;
	}

	uint24_t CPU::_getAbsoluteAddr()
	{
		uint24_t addr = this->_registers.dbr << 16u;
		addr += this->_bus->read(this->_registers.pac++);
		addr += this->_bus->read(this->_registers.pac++) << 8u;
		return addr;
	}

	uint24_t CPU::_getAbsoluteLongAddr()
	{
		uint24_t addr = this->_bus->read(this->_registers.pac++);
		addr += this->_bus->read(this->_registers.pac++) << 8u;
		addr += this->_bus->read(this->_registers.pac++) << 16u;
		return addr;
	}

	uint24_t CPU::_getDirectIndirectIndexedYAddr()
	{
		uint16_t dp = this->_bus->read(this->_registers.pac++) + this->_registers.d;
		uint24_t base = this->_bus->read(dp);
		base += this->_bus->read(dp + 1) << 8u;
		base += this->_registers.dbr << 16u;
		if ((base & 0x80000000u) == (((base + this->_registers.y) & 0x80000000u)))
			this->_hasIndexCrossedPageBoundary = true;
		return base + this->_registers.y;
	}

	uint24_t CPU::_getDirectIndirectIndexedYLongAddr()
	{
		uint16_t dp = this->_bus->read(this->_registers.pac++) + this->_registers.d;
		uint24_t base = this->_bus->read(dp);
		base += this->_bus->read(dp + 1) << 8u;
		base += this->_bus->read(dp + 2) << 16u;
		return base;
	}

	uint24_t CPU::_getDirectIndirectIndexedXAddr()
	{
		uint16_t dp = this->_bus->read(this->_registers.pac++) + this->_registers.d;
		dp += this->_registers.x;
		uint24_t base = this->_bus->read(dp);
		base += this->_bus->read(dp + 1) << 8u;
		base += this->_registers.dbr << 16u;
		return base;
	}

	uint24_t CPU::_getDirectIndexedByXAddr()
	{
		uint16_t dp = this->_bus->read(this->_registers.pac++) + this->_registers.d;
		dp += this->_registers.x;
		return dp;
	}

	uint24_t CPU::_getDirectIndexedByYAddr()
	{
		uint16_t dp = this->_bus->read(this->_registers.pac++) + this->_registers.d;
		dp += this->_registers.y;
		return dp;
	}

	uint24_t CPU::_getAbsoluteIndexedByXAddr()
	{
		uint16_t abs = this->_bus->read(this->_registers.pac++);
		abs += this->_bus->read(this->_registers.pac++) << 8u;
		uint24_t effective = abs + (this->_registers.dbr << 16u);
		if ((effective & 0x80000000u) == (((effective + this->_registers.x) & 0x80000000u)))
			this->_hasIndexCrossedPageBoundary = true;
		return effective + this->_registers.x;
	}

	uint24_t CPU::_getAbsoluteIndexedByYAddr()
	{
		uint16_t abs = this->_bus->read(this->_registers.pac++);
		abs += this->_bus->read(this->_registers.pac++) << 8u;
		uint24_t effective = abs + (this->_registers.dbr << 16u);
		if ((effective & 0x80000000u) == (((effective + this->_registers.y) & 0x80000000u)))
			this->_hasIndexCrossedPageBoundary = true;
		return effective + this->_registers.y;
	}

	uint24_t CPU::_getAbsoluteIndexedByXLongAddr()
	{
		uint24_t lng = this->_bus->read(this->_registers.pac++);
		lng += this->_bus->read(this->_registers.pac++) << 8u;
		lng += this->_bus->read(this->_registers.pac++) << 16u;
		return lng + this->_registers.x;
	}

	uint24_t CPU::_getProgramCounterRelativeAddr()
	{
		uint24_t pc = this->_registers.pac;
		int8_t mod = this->_bus->read(this->_registers.pac++);
		return pc + mod;
	}

	uint24_t CPU::_getProgramCounterRelativeLongAddr()
	{
		uint24_t pc = this->_registers.pac;
		uint8_t val1 = this->_bus->read(this->_registers.pac++);
		uint8_t val2 = this->_bus->read(this->_registers.pac++);
		int16_t mod = val2 > 0x7F ? (static_cast<char>(val2) * 256 - val1) : (val1 | val2 << 8u);
		return pc + mod;
	}

	uint24_t CPU::_getAbsoluteIndirectAddr()
	{
		uint16_t abs = this->_bus->read(this->_registers.pac++);
		abs += this->_bus->read(this->_registers.pac++) << 8u;
		uint24_t effective = this->_bus->read(abs);
		effective += this->_bus->read(abs + 1) << 8u;
		return effective;
	}

	uint24_t CPU::_getAbsoluteIndirectIndexedByXAddr()
	{
		uint24_t abs = this->_bus->read(this->_registers.pac++);
		abs += this->_bus->read(this->_registers.pac++) << 8u;
		abs += this->_registers.x;
		uint24_t effective = this->_bus->read(abs);
		effective += this->_bus->read(abs + 1) << 8u;
		return effective;
	}

	uint24_t CPU::_getDirectIndirectAddr()
	{
		uint16_t dp = this->_bus->read(this->_registers.pac++) + this->_registers.d;
		uint24_t effective = this->_bus->read(dp);
		effective += this->_bus->read(dp + 1) << 8u;
		effective += this->_registers.dbr << 16u;
		return effective;
	}

	uint24_t CPU::_getDirectIndirectLongAddr()
	{
		uint16_t dp = this->_bus->read(this->_registers.pac++) + this->_registers.d;
		uint24_t effective = this->_bus->read(dp);
		effective += this->_bus->read(++dp) << 8u;
		effective += this->_bus->read(++dp) << 16u;
		return effective;
	}

	uint24_t CPU::_getStackRelativeAddr()
	{
		return this->_bus->read(this->_registers.pac++) + this->_registers.s;
	}

	uint24_t CPU::_getStackRelativeIndirectIndexedYAddr()
	{
		uint24_t base = this->_bus->read(this->_registers.pac++) + this->_registers.s;
		base += this->_registers.dbr << 16u;
		return base + this->_registers.y;
	}
}