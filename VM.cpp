#include "VM.h"

#include "Timer.h"
#include "FileHelper.h"

VM::VM(const char* filename, const char* settings)
{
	this->filename = filename;
	this->settings = settings;

	for (int i = 0; i < 256; i++)
		strSlot[i] = "";

	loadProgram();
}

VM::~VM()
{
}

void VM::loadProgram()
{
	std::string filepath = GetWorkingDirectory() + "/" + filename;
	program = readFile(filepath.c_str());

	program.push_back((byte)Opcode::HALT);	// Need an extra byte. Not a part of the program. E.g: Not really executed
}

void VM::cpu()
{
	printf("Started..\n");

	Timer timer;
	timer.StartTimer();

	while (run)
	{
		int opcode = program[ip++]; // Fetch instruction

		switch (opcode)
		{
			case Opcode::JMP:
				ip = static_cast<int>((program[ip++] << 24) | (program[ip++] << 16) | (program[ip++] << 8) | program[ip]);
				ip--;
				break;
			case Opcode::CALL:
				{
					stackFrame sf;
					int len = (int)program[ip++];
					for (int i = 0; i < len; i++) // <----- Number of parameters
					{
						// Fetch parameter
						iSlotSel = (int)program[ip++]; // Select int slot
						sf.parameterInts.push_back(iSlot[iSlotSel]); // add parameter from iSlot[i]
					}

					sf.functionIP = static_cast<int>((program[ip++] << 24) | (program[ip++] << 16) | (program[ip++] << 8) | program[ip++]);
					sf.functionIP--;

					sf.returnStackFramePointer = static_cast<int>((program[ip++] << 24) | (program[ip++] << 16) | (program[ip++] << 8) | program[ip]);
					
					ip = sf.functionIP;

					stackFrames.push_back(sf);
				}
				break;
			case Opcode::RET:
				// NOTE: Return values should already be stored at iSlot[i]
				ip = stackFrames[(int)stackFrames.size()-1].returnStackFramePointer;
				stackFrames.pop_back();
				break;
			case Opcode::INC:
				iSlotSel = (int)program[ip++]; // Select int slot
				iSlot[iSlotSel]++;
				break;
			case Opcode::DEC:
				iSlotSel = (int)program[ip++]; // Select int slot
				iSlot[iSlotSel]--;
				break;
			case Opcode::IADD:
				iSlotSel = (int)program[ip++];
				iSlot[iSlotSel] = iSlot[(int)program[ip++]] + iSlot[(int)program[ip++]];
				break;
			case Opcode::ISUB:
				iSlotSel = (int)program[ip++];
				iSlot[iSlotSel] = iSlot[(int)program[ip++]] - iSlot[(int)program[ip++]];
				break;
			case Opcode::IMUL:
				iSlotSel = (int)program[ip++];
				iSlot[iSlotSel] = iSlot[(int)program[ip++]] * iSlot[(int)program[ip++]];
				break;
			case Opcode::IDIV:
				iSlotSel = (int)program[ip++];
				iSlot[iSlotSel] = iSlot[(int)program[ip++]] / iSlot[(int)program[ip++]];
				break;
			case Opcode::IVAR:
				iSlotSel = (int)program[ip++]; // Select int slot
				iSlot[iSlotSel] = static_cast<int>((program[ip++] << 24) | (program[ip++] << 16) | (program[ip++] << 8) | program[ip++]);
				break;
			case Opcode::ICPY:
				iSlotSel = (int)program[ip++]; // Select int slot (copy to)
				iSlot[iSlotSel] = iSlot[(int)program[ip++]]; // (copy from)
				break;
			case Opcode::IJMPT:
				iSlotSel = (int)program[ip++];
				if (iSlot[iSlotSel] != 0) // true if not false
				{
					ip = static_cast<int>((program[ip++] << 24) | (program[ip++] << 16) | (program[ip++] << 8) | program[ip]);
					ip--;
				}
				else
					ip += 4;
				break;
			case Opcode::IJMPF:
				iSlotSel = (int)program[ip++];
				if (iSlot[iSlotSel] == 0) // true if false
				{
					ip = static_cast<int>((program[ip++] << 24) | (program[ip++] << 16) | (program[ip++] << 8) | program[ip]);
					ip--;
				}
				else
					ip += 4;
				break;
			case Opcode::IJMPGT:
				if (iSlot[(int)program[ip++]] >= iSlot[(int)program[ip++]]) // (A >= B) is it true or false ?
				{
					ip = static_cast<int>((program[ip++] << 24) | (program[ip++] << 16) | (program[ip++] << 8) | program[ip]);
					ip--;
				}
				else
					ip += 5;
				break;
			case Opcode::IJMPG:
				if (iSlot[(int)program[ip++]] > iSlot[(int)program[ip++]]) // (A > B) is it true or false ?
				{
					ip = static_cast<int>((program[ip++] << 24) | (program[ip++] << 16) | (program[ip++] << 8) | program[ip]);
					ip--;
				}
				else
					ip += 5;
				break;
			case Opcode::IJMPLT:
				if (iSlot[(int)program[ip++]] <= iSlot[(int)program[ip++]]) // (A <= B) is it true or false ?
				{
					ip = static_cast<int>((program[ip++] << 24) | (program[ip++] << 16) | (program[ip++] << 8) | program[ip]);
					ip--;
				}
				else
					ip += 5;
				break;
			case Opcode::IJMPL:
				if (iSlot[(int)program[ip++]] < iSlot[(int)program[ip++]]) // (A < B) is it true or false ?
				{
					ip = static_cast<int>((program[ip++] << 24) | (program[ip++] << 16) | (program[ip++] << 8) | program[ip]);
					ip--;
				}
				else
					ip += 5;
				break;
			case Opcode::IJEQU:
				if (iSlot[(int)program[ip++]] == iSlot[(int)program[ip++]]) // (A == B) is it true or false ?
				{
					ip = static_cast<int>((program[ip++] << 24) | (program[ip++] << 16) | (program[ip++] << 8) | program[ip]);
					ip--;
				}
				else
					ip += 5;
				break;
			case Opcode::IJEQUN:
				if (iSlot[(int)program[ip++]] != iSlot[(int)program[ip++]]) // (A != B) is it true or false ?
				{
					ip = static_cast<int>((program[ip++] << 24) | (program[ip++] << 16) | (program[ip++] << 8) | program[ip]);
					ip--;
				}
				else
					ip += 5;
				break;
			case Opcode::STRVAR:
				strSlotSel = (int)program[ip++]; // Select string slot
				strSlotSize[strSlotSel] = (int)program[ip++]; // String length
				{
					for (int i = 0; i < strSlotSize[strSlotSel]; i++)
						strSlot[strSlotSel] += program[ip++];
				}
				break;
			case Opcode::IPRINT:
				iSlotSel = (int)program[ip++];
				printf("%d", iSlot[iSlotSel]);
				break;
			case Opcode::STRPRINT:
				strSlotSel = (int)program[ip++]; // Select string slot
				printf("%s", strSlot[strSlotSel].c_str());
				break;
			case Opcode::PRINTLN:
				printf("\n");
				break;
			case Opcode::HALT:
				run = false; // quit program
				ip = 0;
				break;
		}
	}

	printf("Elapsed time: %f ms\n", timer.StopTimer());
}


/*
// INT32 to BYTE4
unsigned char bytes[4];
unsigned long n = 175;

bytes[0] = (n >> 24) & 0xFF;
bytes[1] = (n >> 16) & 0xFF;
bytes[2] = (n >> 8) & 0xFF;
bytes[3] = n & 0xFF;
*/

/*
// BYTE4 to INT32
uint32_t var2 = (v[3] << 24) | (v[2] << 16) | (v[1] << 8) | v[0];
*/
