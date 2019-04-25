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

typedef enum {
	UNKNOWN,
	WIN32,
	WIN64
} processBitness_t;


bool DoesDLLExist() {
	return false;
}

bool DoesTargetUseMono() {
	return false;
}

processBitness_t GetTargetBitness() {
	return WIN64;
}


int main(int argc, char* argv[]) {
	struct LoaderArguments {
		char DLL_PATH[250];
		char LOADER_NAMESPACE[250];
		char LOADER_CLASSNAME[250];
		char LOADER_METHODNAME[250];
	};
	LoaderArguments lArgs;

	if (argc == 4) {
		// Validate all the passed arguments
		//	also Make sure the DLL and the target application share the same bitness
	} else {
		// Error out
	}

}
