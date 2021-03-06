//
// Created by anonymus-raccoon on 3/25/20.
//

#ifndef COMSQUARE_INSTRUCTION_HPP
#define COMSQUARE_INSTRUCTION_HPP

#include <string>
#include "../Models/Int24.hpp"

namespace ComSquare::CPU
{
	class CPU;

	//! @brief Different addressing modes that instructions can use for the main CPU.
	enum AddressingMode {
		Implied,

		Immediate8bits,
		Immediate16bits,
		ImmediateForA,
		ImmediateForX,

		Absolute,
		AbsoluteLong,

		DirectPage,
		DirectPageIndirect,
		DirectPageIndirectLong,

		AbsoluteIndexedByX,
		AbsoluteIndexedByXLong,
		AbsoluteIndexedByY,

		DirectPageIndexedByX,
		DirectPageIndexedByY,
		DirectPageIndirectIndexedByX,
		DirectPageIndirectIndexedByY,
		DirectPageIndirectIndexedByYLong,

		StackRelative,
		StackRelativeIndirectIndexedByY,


		AbsoluteIndirect,
		AbsoluteIndirectIndexedByX,

		AbsoluteIndirectLong
	};

	//! @brief Struct containing basic information about instructions.
	struct Instruction {
		int (CPU::*call)(uint24_t valueAddr, AddressingMode mode) = nullptr;
		int cycleCount = 0;
		std::string name = "";
		AddressingMode addressingMode = Implied;
		int size = 0;
	};
}
#endif //COMSQUARE_INSTRUCTION_HPP
