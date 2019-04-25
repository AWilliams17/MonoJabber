// MonoJabber.cpp : This file contains the 'main' function. Program execution begins and ends there.
//		-Args List-
// arg 1: Target Application Name
// arg 2: Payload DLL Path
// arg 3: Payload Namespace
// arg 4: Payload Classname
// arg 5: Payload Methodname

// TODO: Organize these
#include "pch.h"
#include "MonoJabber.h"
#include <iostream>
#include "stdafx.h"
#include <windows.h>
#include <sys/stat.h>
#include "mProcess.h"
#include "mMemory.h"
#include <Tlhelp32.h>
#include <psapi.h>

void EndApplication() {
	std::getchar();
	exit(0);
}

bool DoesDLLExist(const char **DLL_PATH) {
	struct stat st;
	return stat(*DLL_PATH, &st) == 0;
}

bool DoesTargetUseMono(HANDLE &TARGET_HANDLE) {
	return mProcessFunctions::mGetModuleAddress(TARGET_HANDLE, "mono-2.0-bdwgc.dll") != NULL;
}

std::string GetMonoLoaderDLLPath() {
	char buffer[MAX_PATH];
	GetModuleFileName(NULL, buffer, MAX_PATH);
	std::string::size_type positionToTrunc = std::string(buffer).find_last_of("\\/");
	std::string currentDirectory = std::string(buffer).substr(0, positionToTrunc);
	std::string pathToLoaderDLL = currentDirectory + "\\MonoLoaderDLL.dll";
	const char *pathArg = pathToLoaderDLL.c_str(); // Really don't like this.

	if (!DoesDLLExist(&pathArg)) {
		return "";
	}

	return pathToLoaderDLL;
}

LoaderArguments CreateArgsStruct(char* program_args[]) {
	LoaderArguments loaderArgs;
	strcpy_s(loaderArgs.DLL_PATH, program_args[2]);
	strcpy_s(loaderArgs.LOADER_NAMESPACE, program_args[3]);
	strcpy_s(loaderArgs.LOADER_CLASSNAME, program_args[4]);
	strcpy_s(loaderArgs.LOADER_METHODNAME, program_args[5]);
	return loaderArgs;
}


int main(int argc, char* argv[]) {
	printf("-MonoJabber-\n");

	if (argc != 6) {
		printf(
			"Error: Application requires five arguments: {TargetProcess} {Path to Payload DLL} "
			"{PayloadNamespace} {PayloadClassname} {PayloadMethodname}\n"
		);
		EndApplication();
	}
	
	const char *targetProcess = argv[1];
	const char *dllPath = argv[2];
	std::string monoLoaderDLLPath = GetMonoLoaderDLLPath();
	LoaderArguments lArgs = CreateArgsStruct(argv);

	int injecteePID = mProcessFunctions::mGetPID(targetProcess);
	if (injecteePID == NULL) {
		printf("Error: Failed to get target PID. Check if it's running and try again.\n" 
			"LastErrorCode: %i - see https://bit.ly/2DuwywP", GetLastError()
		);
		EndApplication();
	}

	if (!DoesDLLExist(&dllPath)) {
		printf("Error: The DLL at the specified location was not found.\n");
		EndApplication();
	}

	if (monoLoaderDLLPath == "") {
		printf("Error: MonoLoaderDLL.dll was not found in the current directory.\n");
		EndApplication();
	}

	HANDLE injecteeHandle = OpenProcess(PROCESS_CREATE_THREAD | PROCESS_QUERY_INFORMATION | PROCESS_VM_OPERATION
		| PROCESS_VM_WRITE | PROCESS_VM_READ, FALSE, injecteePID);

	bool handleIsValid = mProcessFunctions::mValidateHandle(injecteeHandle);
	if (!handleIsValid) {
		printf("Error: Failed to get a handle to the target process. Are you running as admin?"
			"LastErrorCode: %i - see https://bit.ly/2DuwywP \n", GetLastError()
		);
		EndApplication();
	}

	// Inject MonoLoaderDLL.dll into the injectee
	if (!mMemoryFunctions::mInjectDLL(targetProcess, monoLoaderDLLPath)) {
		printf("Error: Failed to inject MonoLoaderDLL.dll into the target process. Are you running as admin?"
			"LastErrorCode: %i - see https://bit.ly/2DuwywP \n", GetLastError()
		);
		EndApplication();
	}

	// Write the parameter struct to the injectee's memory
	LPVOID addressOfParams = VirtualAllocEx(injecteeHandle, NULL, sizeof(LoaderArguments), MEM_RESERVE | MEM_COMMIT, PAGE_READWRITE);
	if (!WriteProcessMemory(injecteeHandle, addressOfParams, &lArgs, sizeof(LoaderArguments), 0)) {
		printf("Error: WriteProcessMemory returned false. Are you running as admin?"
			"LastErrorCode: %i - see https://bit.ly/2DuwywP \n", GetLastError());
		EndApplication();
	}

	// Grab MonoLoaderDLL.dll's Inject method offset, add it to the injectee's base, then call it with the param struct.
	HMODULE loaderModule = LoadLibraryEx(monoLoaderDLLPath.c_str(), NULL, DONT_RESOLVE_DLL_REFERENCES);
	uintptr_t funcOffset = mProcessFunctions::mGetExportedFunctionOffset(loaderModule, "Inject");
	uintptr_t injectedLoaderBase = mProcessFunctions::mGetModuleAddress(injecteeHandle, "MonoLoaderDLL.dll");
	CreateRemoteThread(injecteeHandle, NULL, 0, (LPTHREAD_START_ROUTINE)(injectedLoaderBase + funcOffset), addressOfParams, 0, NULL);

	// Clean it up.
	FreeLibrary(loaderModule);
	CloseHandle(injecteeHandle);

	printf("\nDone. A MessageBox should have been spawned from the injected application with an error/success message.");
}