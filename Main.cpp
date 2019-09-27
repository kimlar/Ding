#include "VM.h"

int main(int argc, char* argv[])
{
	if (argc != 2)
	{
		printf("Ding: Expected a ding executable (*.dng)\n");
		return 1;
	}

	VM vm(argv[1]);
	vm.cpu();

	printf("Press any key to continue");
	system("pause>nul");
	return 0;
}
