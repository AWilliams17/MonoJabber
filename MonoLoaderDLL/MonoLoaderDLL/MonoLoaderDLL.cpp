// MonoLoaderDLL.cpp : Defines the exported functions for the DLL application.
// TODO: Ejection function

#include "stdafx.h"
#include "MonoLoaderDLL.h"
#include "MonoTypes.h"
#include <iostream>
#include <string>
#include <WinUser.h>


void InitMonoFunctions(const HMODULE &MONO_HANDLE, MonoFunctions &MONO_FUNCTIONS) {
	MONO_FUNCTIONS.mGetRootDomain = (t_mono_get_root_domain)GetProcAddress(MONO_HANDLE, "mono_get_root_domain");
	MONO_FUNCTIONS.mAssemblyOpen = (t_mono_assembly_open)GetProcAddress(MONO_HANDLE, "mono_assembly_open");
	MONO_FUNCTIONS.mThreadAttach = (t_mono_thread_attach)GetProcAddress(MONO_HANDLE, "mono_thread_attach");
	MONO_FUNCTIONS.mAssemblyGetImage = (t_mono_assembly_get_image)GetProcAddress(MONO_HANDLE, "mono_assembly_get_image");
	MONO_FUNCTIONS.mClassFromName = (t_mono_class_from_name)GetProcAddress(MONO_HANDLE, "mono_class_from_name");
	MONO_FUNCTIONS.mMethodFromName = (t_mono_class_get_method_from_name)GetProcAddress(MONO_HANDLE, "mono_class_get_method_from_name");
	MONO_FUNCTIONS.mRuntimeInvoke = (t_mono_runtime_invoke)GetProcAddress(MONO_HANDLE, "mono_runtime_invoke");
}


bool LoadAndInvokePayload(std::string &result_buffer, const LoaderArguments &LOADER_ARGS, const MonoFunctions &MONO_FUNCTIONS) {
	MonoDomain* mDomain = MONO_FUNCTIONS.mGetRootDomain();

	if (mDomain == NULL) {
		result_buffer = "Error: Failed to get the root AppDomain. Unknown why this would happen.";
		return false;
	}
	MONO_FUNCTIONS.mThreadAttach(mDomain);

	MonoAssembly* mAssembly = MONO_FUNCTIONS.mAssemblyOpen(LOADER_ARGS.DLL_PATH, NULL);
	if (mAssembly == NULL) {
		result_buffer = "Error: Failed to load the payload. Did you pass the correct path?";
		return false;
	}

	MonoImage* mImage = MONO_FUNCTIONS.mAssemblyGetImage(mAssembly);
	if (mImage == NULL) {
		result_buffer = "Error: Failed to get the payload's MonoImage.";
		return false;
	}

	MonoClass* mClass = MONO_FUNCTIONS.mClassFromName(mImage, LOADER_ARGS.LOADER_NAMESPACE, LOADER_ARGS.LOADER_CLASSNAME);
	if (mClass == NULL) {
		result_buffer=  "Error: Failed to get the payload's class from the namespace. "
			"Double check those two arguments and try again.";
		return false;
	}

	MonoMethod* mMethod = MONO_FUNCTIONS.mMethodFromName(mClass, LOADER_ARGS.LOADER_METHODNAME, 0);
	if (mMethod == NULL) {
		result_buffer = "Error: Failed to get the payload's method. Double check that argument and try again.";
		return false;
	}

	MONO_FUNCTIONS.mRuntimeInvoke(mMethod, NULL, NULL, NULL);
	result_buffer = "Injection and RuntimeInvoke were successful.";
	return true;
}


void Inject(void* loaderArguments) {
	// Setup the args struct (this assumes the caller has validated all the arguments within before calling)
	struct LoaderArguments* args = (struct LoaderArguments*)loaderArguments;
	
	// Set up result buffer
	std::string resultMessage = "";

	MonoFunctions monoFunctions;

	// TODO: Make it similar to this https://bit.ly/2YW3Cqu
	HMODULE monoModule = GetModuleHandleA("mono-2.0-bdwgc.dll");
	if (monoModule == NULL) {
		resultMessage = 
			"Error: Failed to get a handle to mono-2.0-bdwgc.dll -" 
			"This typically means that the version of mono being used in the target process is incompatible "
			"with the version that this injector supports (for now).\nLast Error Code: " + std::to_string(GetLastError()) + " - consult https://bit.ly/2DuwywP "
			"to see what that code means.";
	} else {
		InitMonoFunctions(monoModule, monoFunctions);
		LoadAndInvokePayload(resultMessage, *args, monoFunctions);
	}

	MessageBox(NULL, resultMessage.c_str(), "Operation Result", MB_OK);
}