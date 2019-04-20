#pragma once
#include "mono_types.h"
#ifdef _EXPORTS
#define EXPORT extern "C" __declspec(dllexport)
#else  
#define EXPORT extern "C" __declspec(dllimport)
#endif


struct LoaderArguments {
	char DLL_PATH[250];
	char LOADER_NAMESPACE[250];
	char LOADER_CLASSNAME[250];
	char LOADER_METHODNAME[250];
};

typedef struct MonoFunctions {
	t_mono_thread_attach				mThreadAttach;
	t_mono_get_root_domain				mGetRootDomain;
	t_mono_assembly_open				mAssemblyOpen;
	t_mono_assembly_get_image			mAssemblyGetImage;
	t_mono_class_from_name				mClassFromName;
	t_mono_class_get_method_from_name	mMethodFromName;
	t_mono_runtime_invoke				mRuntimeInvoke;
} MonoFunctions;

EXPORT void InjectLoader(void* LoaderArguments);