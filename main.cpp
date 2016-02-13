#include "SoftRApp.h"

/*int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE prevInstance,
	PSTR cmdLine, int showCmd)
	*/
int main()
{
	// Enable run-time memory check for debug builds.
#if defined(DEBUG) | defined(_DEBUG)
	_CrtSetDbgFlag(_CRTDBG_ALLOC_MEM_DF | _CRTDBG_LEAK_CHECK_DF);
#endif

	HINSTANCE hInstance = GetModuleHandle(0);

	//���̴߳������������̡߳�


	SoftRApp theApp(hInstance);

	if (!theApp.Init())
		return 0;

	return theApp.Run();
}
