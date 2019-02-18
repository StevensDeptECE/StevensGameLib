#include "winkbd.hh"

int APIENTRY WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int NCmdShow)
{
	Inputs i(hInstance, hPrevInstance, lpCmdLine, NCmdShow);
	
	unsigned short vkey = 0;
	while(1) {
		printf("%d\n", i.read_device(1));
		fflush(stdout);
		Sleep(1000);
	}
	
	
	return 0;
}