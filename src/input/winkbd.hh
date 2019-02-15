#ifdef __MINGW32__
#   define _WIN32_WINNT 0x0501
#endif // __MINGW32__
#include <windows.h>
#include <winuser.h>
#include <stdio.h>

LRESULT CALLBACK WndProc(HWND, UINT, WPARAM, LPARAM);
void OnInput(HWND hwnd, WPARAM code, HRAWINPUT hRawInput);

int APIENTRY WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int NCmdShow){

	DWORD dwWidth = GetSystemMetrics(SM_CXSCREEN) / 4;
	DWORD dwHeight = GetSystemMetrics(SM_CYSCREEN) / 4;
	
	MSG  msg;
	WNDCLASSW wc = {0};

	wc.style = CS_HREDRAW | CS_VREDRAW;
	wc.lpszClassName = L"KeyboardM";
	wc.hInstance     = hInstance;
	wc.hbrBackground = GetSysColorBrush(COLOR_3DFACE);
	wc.lpfnWndProc   = WndProc;
	wc.hCursor       = LoadCursor(0, IDC_ARROW);
	char szTitle[]	 = "KeyboardM";

	RegisterClassW(&wc);
	CreateWindowW(wc.lpszClassName, L"KeyboardM",
	WS_OVERLAPPEDWINDOW | WS_VISIBLE,
	0, 0, dwWidth, dwHeight, NULL, NULL, hInstance, NULL);
	

	
	HWND hwnd;
	hwnd = FindWindow(NULL, szTitle);
	ShowWindow(hwnd, SW_SHOW);
	
	HWND hWndKbd1 = CreateWindow("STATIC", "", WS_VISIBLE | WS_CHILD | ES_LEFT, 10,10,100,100, hwnd, NULL, hInstance, NULL);
	SetWindowText(hWndKbd1, TEXT("Keyboard 1: "));
	HWND hWndKbd2 = CreateWindow("STATIC", "", WS_VISIBLE | WS_CHILD | ES_LEFT, 10,50,140,100, hwnd, NULL, hInstance, NULL);
	SetWindowText(hWndKbd2, TEXT("Keyboafasfard 2:"));
	
	const char *text = "Keyboard 2:";
	SetWindowText(hWndKbd2, (LPCTSTR)text);
	
	RAWINPUTDEVICE dev;
	dev.usUsagePage = 1;
	dev.usUsage = 6;
	dev.dwFlags = 0;
	dev.hwndTarget = hwnd;
	RegisterRawInputDevices(&dev, 1, sizeof(dev));
	
	
	while (GetMessage(&msg, NULL, 0, 0)) {
		TranslateMessage(&msg);
		DispatchMessage(&msg);
	}
	
	return (int) msg.wParam;
}

void process_keyboard_input(HWND hwnd, LPARAM lParam) {
	UINT dwSize;
	GetRawInputData((HRAWINPUT)lParam, RID_INPUT, NULL, &dwSize, sizeof(RAWINPUTHEADER));
	
	
	RAWINPUT *input = (RAWINPUT *)malloc(dwSize);
	GetRawInputData((HRAWINPUT)lParam, RID_INPUT, input, &dwSize, sizeof(RAWINPUTHEADER));
	
	/*
	typedef struct tagRAWINPUT {
		RAWINPUTHEADER header;
		union {
			RAWMOUSE    mouse;
			RAWKEYBOARD keyboard;
			RAWHID      hid;
		} data;
	} RAWINPUT, *PRAWINPUT, *LPRAWINPUT;
	*/
	
	/*
	typedef struct tagRAWINPUTHEADER {
		DWORD  dwType;  // RIM_TYPEMOUSE = 0, RIM_TYPEKEYBOARD = 1, RIM_TYPEHID = 2
		DWORD  dwSize;  // The size, in bytes, of the entire input packet of data.
		HANDLE hDevice;
		WPARAM wParam;
	} RAWINPUTHEADER, *PRAWINPUTHEADER, *LPRAWINPUTHEADER;
	*/
	
	/*
	typedef struct tagRAWKEYBOARD {
		USHORT MakeCode;
		USHORT Flags;
		USHORT Reserved;
		USHORT VKey;
		UINT   Message;
		ULONG  ExtraInformation;
	} RAWKEYBOARD, *PRAWKEYBOARD, *LPRAWKEYBOARD;
	*/
	if (input->header.dwType == RIM_TYPEKEYBOARD && !input->data.keyboard.Flags) {
		printf("%p: %d\n", input->header.hDevice, input->data.keyboard.VKey);
	}
}

LRESULT CALLBACK WndProc(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam) {
	
	HDC hdc;
	PAINTSTRUCT ps;

	switch(msg) {
	case WM_INPUT:
		process_keyboard_input(hwnd, lParam);
		break;
	case WM_PAINT:

		hdc = BeginPaint(hwnd, &ps);
		EndPaint(hwnd, &ps);
		break;

	case WM_CREATE:
		break;
	case WM_DESTROY:
		PostQuitMessage(0);
		return 0;
	}

	return DefWindowProcW(hwnd, msg, wParam, lParam);
}