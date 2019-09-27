#pragma once

#include <windows.h>
#include <string>

#include <vector>
#include <fstream>

using byte = unsigned char;

// Get working directory
std::string GetWorkingDirectory() // Note: Win32 specific
{
	char working_directory[MAX_PATH + 1];
	GetCurrentDirectoryA(sizeof(working_directory), working_directory);
	return working_directory;
}

// Read binary file
std::vector<byte> readFile(const char* filename)
{
	// open the file:
	std::streampos fileSize;
	std::ifstream file(filename, std::ios::binary);

	// get its size:
	file.seekg(0, std::ios::end);
	fileSize = file.tellg();
	file.seekg(0, std::ios::beg);

	// read the data:
	std::vector<byte> fileData(fileSize);
	file.read((char*)&fileData[0], fileSize);
	return fileData;
}
