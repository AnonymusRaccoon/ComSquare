//
// Created by anonymus-raccoon on 1/27/20.
//

#include "SNES.hpp"

namespace ComSquare
{
	SNES::SNES(const std::shared_ptr<MemoryBus> &bus, const std::string &romPath) :
		cpu(new CPU::CPU(bus)),
		ppu(new PPU::PPU()),
		apu(new APU::APU()),
		cartridge(new Cartridge::Cartridge(romPath)),
		wram(new Ram::Ram(16384))
	{ }
}