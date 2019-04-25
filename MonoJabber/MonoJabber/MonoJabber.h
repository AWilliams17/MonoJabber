#pragma once
typedef enum {
	UNKNOWN,
	WIN32,
	WIN64
} processBitness_t;


struct LoaderArguments {
	char DLL_PATH[250];
	char LOADER_NAMESPACE[250];
	char LOADER_CLASSNAME[250];
	char LOADER_METHODNAME[250];
};
LoaderArguments lArgs;