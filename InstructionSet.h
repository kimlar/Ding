#pragma once

enum Opcode {
	JMP = 0x01,	// Jump unconditionally
	CALL,		// Call function. Store current stack frame and create a new stack frame.
	RET,		// Return from function
	// int stuff
	INC,		// 'int' increment: X++
	DEC,		// 'int' decrement: X--
	IADD,		// 'int' addition: X=A+B
	ISUB,		// 'int' subtraction: X=A-B
	IMUL,		// 'int' multiply: X=A*B
	IDIV,		// 'int' division: X=A/B
	IVAR,		// Store 'int' variable to a slot (temporary working slots for int values).
	ICPY,		// Change the value of an 'int' variable by copy from another integer slot.
	IJMPT,		// Jump if true.			A==true
	IJMPF,		// Jump if false.			A==false
	IJMPGT,		// Jump if greater than.	A>=B
	IJMPG,		// Jump if greater			A>B
	IJMPLT,		// Jump if less then		A<=B
	IJMPL,		// Jump if less				A<B
	IJEQU,		// Jump if equal			A==B
	IJEQUN,		// Jump if not equal		A!=B
	// string stuff
	STRVAR,		// Store 'string' variable to a slot (temporary working slots for strings).
	// print stuff
	IPRINT,		// Print from selected int slot.
	STRPRINT,	// Print from selected string slot.
	PRINTLN,	// Print new line
	// other stuff
	HALT,		// Stop the program execution. E.g: quit the program
};
