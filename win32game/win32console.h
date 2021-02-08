#include <Windows.h>
#include <stdio.h>

// call "FreeConsole()" once before the program closes!
void createWin32Console()
{
	AllocConsole();
	AttachConsole(GetCurrentProcessId());
#ifdef _CRT_INSECURE_DEPRECATE
	FILE * oldFilePointer;
	freopen_s(&oldFilePointer, "CON","w",stdout);
#else
	freopen("CON","w",stdout);
#endif
}
