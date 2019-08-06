#pragma once
BOOL Is64Bit(void) {
	#if defined(_WIN64)
		return TRUE;   // 64-bit program
	return FALSE;
	#endif
}

struct LoaderArguments {
	char DLL_PATH[250];
	char LOADER_NAMESPACE[250];
	char LOADER_CLASSNAME[250];
	char LOADER_METHODNAME[250];
	char MLPIPENAME[250];
};