#pragma once

#include <vector>

#include "InstructionSet.h"

/*
	TODO LIST:
	- Load program with parameter from main()
	- Keyboard input (read until enter/return)
	- Opcode: STRCPY (copy string from another string slot)

	LONG LIST:
	- Memory (allocate and deallocate + get/set data)
	- Functions: Need a function stack
	- Functions: Parameters
	- Functions: Return value
	- File (load/save)
	- Load byte code file on the fly. This is a nice way to extend/plug-ins the program.
	- Random generator + number: E.g: Between 3 and 8
	- Stack () ? Need?
	- Time/Date
	- Timer (get delta time in ms) + (Utils: Sleep ms)
	- DataType: float
	- DataType: double
	- DataType: bool
	- Convert between data types.
	- Helper: Min(A,B)
	- Helper: Max(A,B)
	- Helper: Swap(A,B)
	- Helper: Mod(A,B)
	- Math: Pi
	- Math: Mode=Deg
	- Math: Mode=Rad
	- Math: Sin()
	- Math: Cos()
	- Math: Tan()
	- Math: InvSin()
	- Math: InvCos()
	- Math: InvTan()
	- Math: Sqr()
	- Math: Pow()
	- Math: Exp()
	- Math: Ln()
	- Math: Log()
	- Math: InvLog()
	- Optimized Opcode + Local storage (4 bytes for all types)
	- GLFW: Window
	- GLFW: Input: keyboard and mouse
	- GLFW: Events ...
	- Sprite: load & render sprite
	- Font: load & render font
	- Play sound
	- Threads
	- Network send/receive TCP/UDP
*/

using byte = unsigned char;

class VM
{
public:
	VM(const char* filename, const char* settings = "");
	~VM();

	void cpu();

private:
	void loadProgram();

	const char* filename;
	const char* settings;

	std::vector<byte> program;	// The program as byte code
	bool run = true;
	int ip = 0;					// Instruction Pointer (E.g: Current position in the code instruction)
	// int:
	int iSlotSel = 0;			// Selected slot for int (E.g: Current slot selected)
	int iSlot[256] = {0};		// Temporary working slots for int values
	// string:
	int strSlotSel = 0;			// Selected slot for string (E.g: Current slot selected)
	std::string strSlot[256]; // Temporary working slots for string values
	int strSlotSize[256] = {0};		// Size of its slot

	// Stack frame: function, parameters, local variables, return value, return function address
	struct stackFrame 
	{
		// Parameters
		std::vector<int> parameterInts;
		//std::vector<std::string> parameterStrings;
		// Local variables
		std::vector<int> localInts;
		//std::vector<std::string> localStrings;
		// Jump to function (ip) location.
		int functionIP;
		// Stack frame pointer (ip) to set when returning.
		int returnStackFramePointer;
	};
	std::vector<stackFrame> stackFrames;
};
