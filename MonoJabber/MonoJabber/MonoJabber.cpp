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
#include "MonoJabber.h"
#include <iostream>
#include "stdafx.h"
#include <windows.h>
#include <sys/stat.h>
#include "mProcess.h"
#include "mMemory.h"

int EndApplication() {
	std::getchar();
	return 0;
}


bool DoesDLLExist(const char **DLL_PATH) {
	struct stat st;
	return stat(*DLL_PATH, &st) == 0;
}

bool DoesTargetUseMono(HANDLE &TARGET_HANDLE) {
	return mProcessFunctions::mGetModuleAddress(TARGET_HANDLE, "mono-2.0-bdwgc.dll") != NULL;
}

processBitness_t GetTargetBitness() {
	return WIN64;
}


int main(int argc, char* argv[]) {
	printf("-MonoJabber-");
	/*
	const char *targetProcess, *dllPath, *payloadNamespace, *payloadClassname, *payloadMethodname = NULL;
	if (argc == 4) {
		targetProcess =		argv[1];
		dllPath =			argv[2];
		payloadNamespace =	argv[3];
		payloadClassname =	argv[4];
		payloadMethodname =	argv[5];
	} else {
		printf(
			"Error: Application requires five arguments: {TargetProcess} {Path to Payload DLL} "
			"{PayloadNamespace} {PayloadClassname} {PayloadMethodname}\n"
		);
		EndApplication();
	}

	int injecteePID = mProcessFunctions::mGetPID(targetProcess);

	if (injecteePID == NULL) {
		printf("Error: Failed to get target PID. Check if it's running and try again.\n" 
			"LastErrorCode: %i - see https://bit.ly/2DuwywP", GetLastError()
		);
		EndApplication();
	}

	HANDLE injecteeHandle = OpenProcess(PROCESS_CREATE_THREAD | PROCESS_QUERY_INFORMATION | PROCESS_VM_OPERATION
		| PROCESS_VM_WRITE | PROCESS_VM_READ, FALSE, injecteePID);

	bool handleIsValid = mProcessFunctions::mValidateHandle(injecteeHandle);
	if (!handleIsValid) {
		printf("Error: Failed to get a handle to the target process. Are you running as admin?"
			"LastErrorCode: %i - see https://bit.ly/2DuwywP", GetLastError()
		);
		EndApplication();
	}



	printf("");
	*/
}
