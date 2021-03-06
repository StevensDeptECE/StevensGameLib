/*
 * winkbd.hh
 *
 *  Created on: Feb 15, 2019
 *      Author: pho
 */

#ifdef __MINGW32__
#   define _WIN32_WINNT 0x0501
#endif // __MINGW32__
#include <windows.h>
#include <winuser.h>
#include <iostream>
#include <stdio.h>
#include <stdlib.h>
#include <vector>
#include <string>
#include <process.h>

using namespace std;


LRESULT CALLBACK WndProc(HWND, UINT, WPARAM, LPARAM);
void create_event_window(void *);

static void (*keyboard_handler)(int id, unsigned char key);

class Inputs 
{
private:
	
	struct Keyboard 
	{
		HANDLE fh;
		string name;
		Keyboard(const string& name) : name(name) {}
		
		Keyboard(const HANDLE fh) : fh(fh)
		{
			cout << "Added keyboard: " << fh << endl;
		}
	};
	
	struct VKey_Array
	{
		bool keys[256] = {0};
		VKey_Array() {}
	};
	
	
	static vector<Keyboard> keyboards;
	static vector<VKey_Array> keys;
	
	void getDevices()
	{
		/*
		UINT GetRawInputDeviceList(
			PRAWINPUTDEVICELIST pRawInputDeviceList,
			PUINT               puiNumDevices,
			UINT                cbSize
		);
		*/
		
		/*
		typedef struct tagRAWINPUTDEVICELIST {
			HANDLE hDevice;
			DWORD  dwType;  // RIM_TYPEMOUSE = 0, RIM_TYPEKEYBOARD = 1, RIM_TYPEHID = 2
		}	RAWINPUTDEVICELIST, *PRAWINPUTDEVICELIST;
		*/
		
		UINT nDevices;
		PRAWINPUTDEVICELIST pRawInputDeviceList;
		
		// Check # of devices
		if (GetRawInputDeviceList(NULL, &nDevices, sizeof(RAWINPUTDEVICELIST)) != 0) {
			printf("Error: GetRawInputDeviceList()\n");
		}
		// Allocate memory for pointer
		if ((pRawInputDeviceList = (PRAWINPUTDEVICELIST)malloc(sizeof(RAWINPUTDEVICELIST) * nDevices)) == NULL) {
			printf("Error: pRawInputDeviceList malloc()\n");
		}
		// Get structures
		if (GetRawInputDeviceList(pRawInputDeviceList, &nDevices, sizeof(RAWINPUTDEVICELIST)) == (-1)) {
			printf("Error: GetRawInputDeviceList()\n");
		}
		
		cout << "Number of devices: " << nDevices << endl;
		for (int i = 0; i < nDevices; ++i) {
			// print the device ids
			if ((pRawInputDeviceList+i)->dwType == RIM_TYPEKEYBOARD) {
				Inputs::keyboards.push_back(Keyboard((pRawInputDeviceList+i)->hDevice));
				keys.push_back(VKey_Array());
			}
		}
		free(pRawInputDeviceList);
	}
	
	static bool is_down(int n, unsigned short vkey)
	{
		return keys[n].keys[vkey];
	}
	
	static void poll(void *)
	{
		while (1) {
			for (int i = 0; i < keyboards.size(); ++i) {
				for (int j = 0; j < 256; ++j) {
					if (is_down(i, j)) {
						(*keyboard_handler)(i, j);
					}
				}
			}
			Sleep(5);
		}
	}
	
public:
	Inputs()
	{
		getDevices();
		// create hidden window to handle events,
		// non-blocking
		_beginthread(create_event_window, 0, NULL);
	}
	
	static void register_handler(void (*handler)(int, unsigned char))
	{
		keyboard_handler = handler;
		_beginthread(poll, 0, NULL);
	}
	
	static void set_key(HANDLE handle, unsigned short vkey, bool is_down)
	{
		for (int i = 0; i < keyboards.size(); ++i) {
			if (keyboards[i].fh == handle) {
					keys[i].keys[vkey] = is_down;
			}
		}
	}
	
	int keyboard_count()
	{
		return keyboards.size();
	}
	
	friend ostream& operator <<(ostream& s, const Inputs& inp)
	{
		for (auto k : inp.keyboards) {
			cout << k.name << '\n';
		}
		return s;
	}
};
vector<Inputs::Keyboard> Inputs::keyboards;
vector<Inputs::VKey_Array> Inputs::keys;



void process_keyboard_input(HWND hwnd, LPARAM lParam)
{
	// safety check
	UINT dwSize;
	if (-1 == GetRawInputData((HRAWINPUT)lParam, RID_INPUT, NULL, &dwSize, sizeof(RAWINPUTHEADER)))
		printf("Error: GetRawInputData()\n");
	
	
	RAWINPUT *input = (RAWINPUT *)malloc(dwSize);
	GetRawInputData((HRAWINPUT)lParam, RID_INPUT, input, &dwSize, sizeof(RAWINPUTHEADER));

	if (input->header.dwType == RIM_TYPEKEYBOARD) {
		if (input->data.keyboard.Message == WM_KEYUP) {
			Inputs::set_key(input->header.hDevice, input->data.keyboard.VKey, FALSE); 
		}
		else if (input->data.keyboard.Message == WM_KEYDOWN) {
			Inputs::set_key(input->header.hDevice, input->data.keyboard.VKey, TRUE);
		}
	}
	
	delete input;
	
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
}


void create_event_window(void *)
{
	HINSTANCE hInstance = (HINSTANCE)GetModuleHandle(NULL);
	
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
	WS_OVERLAPPEDWINDOW, // | WS_VISIBLE,
	0, 0, dwWidth, dwHeight, NULL, NULL, hInstance, NULL);
	
	HWND hwnd;
	hwnd = FindWindow(NULL, szTitle);
	
	RAWINPUTDEVICE dev;
	dev.usUsagePage = 1;
	dev.usUsage = 6;
	dev.dwFlags = RIDEV_INPUTSINK;	// Enable input when not focused
	dev.hwndTarget = hwnd;			// Must be specified for dwFlags
	RegisterRawInputDevices(&dev, 1, sizeof(dev));
	
	while (GetMessage(&msg, NULL, 0, 0)) {
		TranslateMessage(&msg);
		DispatchMessage(&msg);
	}
	
	return (void) msg.wParam;
}



LRESULT CALLBACK WndProc(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam)
{	
	HDC hdc;
	PAINTSTRUCT ps;

	switch(msg) {
	case WM_INPUT:
		process_keyboard_input(hwnd, lParam);
		break;
	case WM_DESTROY:
		PostQuitMessage(0);
		return 0;
	}

	return DefWindowProcW(hwnd, msg, wParam, lParam);
}