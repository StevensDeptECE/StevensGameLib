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

		
	static vector<Keyboard> keyboards;
	static vector<unsigned short> vkey_current;
	
	static bool control_keys[3];
	
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
				vkey_current.push_back(-1);
			}
		}
		free(pRawInputDeviceList);
	}
	
public:
	Inputs()
	{
		getDevices();
		for (int i = 0; i < sizeof(control_keys); ++i) {
			control_keys[i] = FALSE;
		}
		// create hidden window to handle events,
		// non-blocking
		_beginthread(create_event_window, 0, NULL);
	}
	unsigned short read_device(int n)
	{
		if (n >= vkey_current.size())
			return 65535;
		return vkey_current[n];
	}
	
	string get_key_string(int n, unsigned char c)
	{
		string s = "";
		if (n >= vkey_current.size())
			return "Error";
		s += control_keys[0] ? "shift+" : "";
		s += control_keys[1] ? "ctrl+" : "";
		s += control_keys[2] ? "alt+" : "";
		
		s += vkey_current[n];
		
		return s;
		
	}
	
	static void set_key(HANDLE handle, unsigned short vkey, bool is_down)
	{
		for (int i = 0; i < keyboards.size(); ++i) {
			if (keyboards[i].fh == handle) {
				if (vkey >= VK_SHIFT && vkey <= VK_MENU) {
					// 10 = shift, 11 = ctrl, 12 = alt
					control_keys[vkey - VK_SHIFT] = is_down;
					cout << "Control key" << endl;
				} else {
					vkey_current[i] = is_down ? vkey : 65535;
				}
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
vector<unsigned short> Inputs::vkey_current;
bool Inputs::control_keys[3];



void process_keyboard_input(HWND hwnd, LPARAM lParam)
{
	// safety check
	UINT dwSize;
	if (-1 == GetRawInputData((HRAWINPUT)lParam, RID_INPUT, NULL, &dwSize, sizeof(RAWINPUTHEADER)))
		printf("Error: GetRawInputData()\n");
	
	
	RAWINPUT *input = (RAWINPUT *)malloc(dwSize);
	GetRawInputData((HRAWINPUT)lParam, RID_INPUT, input, &dwSize, sizeof(RAWINPUTHEADER));
	
	// If key_up then set key to -1 (may want to change keystate handling)
	// May want to have an array with every key, and
	// API call is getKeyState('A') for example
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