#include "Global.h"
#pragma comment(lib, "User32.lib")
int WINAPI WinMain(HINSTANCE hinstance, HINSTANCE prevInstance, PSTR cmdLine, int showCmd)
{
	Global::RunApp();
	return 0;
}