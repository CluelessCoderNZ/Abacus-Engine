/*
	============================================
	Project:	Abacus Engine
	File: 		main.cpp
	Author: 	Connor Macdonald
	--------------------------------------------
	Entry point for the engine.
	============================================
*/

#include <iostream>

#ifdef OS_LINUX
#include "linux_platform.h"
#endif

using namespace std;

// Entry point for the program
int main()
{
	return StartPlatformEngine();
}