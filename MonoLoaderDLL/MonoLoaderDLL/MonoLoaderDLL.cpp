// MonoLoaderDLL.cpp : Defines the exported functions for the DLL application.
//

#include "stdafx.h"
#include "MonoLoaderDLL.h"
#include <WinUser.h>
#include "mono_types.h"


void initMonoFunctions(const HMODULE &MONO_HANDLE, MonoFunctions &MONO_FUNCTIONS) {
	MONO_FUNCTIONS.mGetRootDomain = (t_mono_get_root_domain)GetProcAddress(MONO_HANDLE, "mono_get_root_domain");
	MONO_FUNCTIONS.mAssemblyOpen = (t_mono_assembly_open)GetProcAddress(MONO_HANDLE, "mono_assembly_open");
	MONO_FUNCTIONS.mThreadAttach = (t_mono_thread_attach)GetProcAddress(MONO_HANDLE, "mono_thread_attach");
	MONO_FUNCTIONS.mAssemblyGetImage = (t_mono_assembly_get_image)GetProcAddress(MONO_HANDLE, "mono_assembly_get_image");
	MONO_FUNCTIONS.mClassFromName = (t_mono_class_from_name)GetProcAddress(MONO_HANDLE, "mono_class_from_name");
	MONO_FUNCTIONS.mMethodFromName = (t_mono_class_get_method_from_name)GetProcAddress(MONO_HANDLE, "mono_class_get_method_from_name");
	MONO_FUNCTIONS.mRuntimeInvoke = (t_mono_runtime_invoke)GetProcAddress(MONO_HANDLE, "mono_runtime_invoke");
}


void InjectLoader(void* loaderArguments) {
	struct LoaderArguments* args = (struct LoaderArguments*)loaderArguments;
	const char* payloadPath = args->DLL_PATH;
	const char* payloadNamespace = args->LOADER_NAMESPACE;
	const char* payloadClassname = args->LOADER_CLASSNAME;
	const char* payloadMethodname = args->LOADER_METHODNAME;

	MonoFunctions monoFunctions;

	// TODO: Make it similar to this https://bit.ly/2YW3Cqu
	HMODULE monoModule = GetModuleHandleA("mono-2.0-bdwgc.dll");

	initMonoFunctions(monoModule, monoFunctions);

	MonoDomain* mDomain = monoFunctions.mGetRootDomain();
	monoFunctions.mThreadAttach(mDomain);
	MonoAssembly* mAssembly = monoFunctions.mAssemblyOpen(payloadPath, NULL);
	MonoImage* mImage = monoFunctions.mAssemblyGetImage(mAssembly);
	MonoClass* mClass = monoFunctions.mClassFromName(mImage, payloadNamespace, payloadClassname);
	MonoMethod* mMethod = monoFunctions.mMethodFromName(mClass, payloadMethodname, 0);
	monoFunctions.mRuntimeInvoke(mMethod, NULL, NULL, NULL);

	// TODO: FreeLibrary.
}