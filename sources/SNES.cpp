//
// Created by anonymus-raccoon on 1/27/20.
//

#include <ios>
#include <iostream>
#include "SNES.hpp"
#ifdef DEBUGGER_ENABLED
	#include "Debugger/CPUDebug.hpp"
#endif

namespace ComSquare
{
	SNES::SNES(const std::shared_ptr<Memory::MemoryBus> &bus, const std::string &romPath, Renderer::IRenderer &renderer) :
		cartridge(new Cartridge::Cartridge(romPath)),
		cpu(new CPU::CPU(bus, cartridge->header)),
		ppu(new PPU::PPU(bus, renderer)),
		apu(new APU::APU()),
		wram(new Ram::Ram(16384)),
		sram(new Ram::Ram(this->cartridge->header.sramSize))
	{
		bus->mapComponents(*this);
	}

	void SNES::enableCPUDebugging()
	{
		#ifdef DEBUGGER_ENABLED
			this->cpu = std::make_shared<Debugger::CPUDebug>(*this->cpu, *this);
		#else
			std::cerr << "Debugging features are not enabled. You can't enable the debugger." << std::endl;
		#endif
	}

	void SNES::disableCPUDebugging()
	{
		this->cpu = std::make_shared<CPU::CPU>(*this->cpu);
	}

	void SNES::enableRamViewer()
	{
		#ifdef DEBUGGER_ENABLED
			this->_ramViewer = std::make_shared<Debugger::MemoryViewer>(*this);
		#endif
	}

	void SNES::disableRamDebugging()
	{
		#ifdef DEBUGGER_ENABLED
			this->_ramViewer = nullptr;
		#endif
	}

	void SNES::update()
	{
		unsigned cycleCount = this->cpu->update();
		this->ppu->update(cycleCount);
		this->apu->update(cycleCount);
	}
}
