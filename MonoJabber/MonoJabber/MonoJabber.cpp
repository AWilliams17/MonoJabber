// MonoJabber.cpp : This file contains the 'main' function. Program execution begins and ends there.
//		-Args List-
// arg 1: Target Application Name
// arg 2: Loader DLL Path
// arg 3: Loader Namespace
// arg 4: Loader Classname
// arg 5: Loader Methodname
// If any of these args are either
//   A: Invalid
//		-or-
//   B: Not supplied
// Then any valid/passed arguments should be preserved and
// the program should ask the user to supply the rest.
// Additionally, do a sanity check to make sure 'MJL.dll' is present & correct.
//	(Might instead be better to just embed MJL.dll inside the program itself and deploy it that way)
// Also determine if the bitness of the target matches the bitness of the injector.

#include "pch.h"
#include <iostream>


bool DLLExists() {
	return false;
}

bool TargetUsesMono() {
	return false;
}


int main(int argc, char* argv[]) {
	
}
