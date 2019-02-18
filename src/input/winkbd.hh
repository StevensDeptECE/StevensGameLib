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

struct mainInfo {
	HINSTANCE h;
	HINSTANCE p;
	LPSTR l;
	int n;
};

LRESULT CALLBACK WndProc(HWND, UINT, WPARAM, LPARAM);
void OnInput(HWND hwnd, WPARAM code, HRAWINPUT hRawInput);
void poop(void *data);

class Inputs {
private:
	struct Keyboard {
		HANDLE fh;
		string name;
		Keyboard(const string& name) : name(name) {
		}
		
		Keyboard(const HANDLE fh) : fh(fh) {
			printf("Added keyboard: %p\n", fh);
		}
	};

		
	static vector<Inputs::Keyboard> keyboards;
	static vector<unsigned short> vkey_current;
	
	void getDevices() {
		/*
		UINT GetRawInputDeviceList(
		  PRAWINPUTDEVICELIST pRawInputDeviceList,
		  PUINT               puiNumDevices,
		  UINT                cbSize
		);
		*/
		
		/*typedef struct tagRAWINPUTDEVICELIST {
		  HANDLE hDevice;  // RIM_TYPEMOUSE = 0, RIM_TYPEKEYBOARD = 1, RIM_TYPEHID = 2
		  DWORD  dwType;
		} RAWINPUTDEVICELIST, *PRAWINPUTDEVICELIST;
		*/
		
		UINT nDevices;
		PRAWINPUTDEVICELIST pRawInputDeviceList;
		if (GetRawInputDeviceList(NULL, &nDevices, sizeof(RAWINPUTDEVICELIST)) != 0) { printf("Error\n");}
		if ((pRawInputDeviceList = (PRAWINPUTDEVICELIST)malloc(sizeof(RAWINPUTDEVICELIST) * nDevices)) == NULL) {printf("Error\n");}
		if (GetRawInputDeviceList(pRawInputDeviceList, &nDevices, sizeof(RAWINPUTDEVICELIST)) == (-1)) {printf("Error\n");}
		// do the job...
		cout << "Number of devices: " << nDevices << endl;
		for (int i = 0; i < nDevices; ++i) {
			// print the device ids
			fflush(stdout);
			if ((pRawInputDeviceList+i)->dwType == RIM_TYPEKEYBOARD) {
				Inputs::keyboards.push_back(Keyboard((pRawInputDeviceList+i)->hDevice));
				vkey_current.push_back(-1);
				fflush(stdout);
			}
		}
		
		
		// after the job, free the RAWINPUTDEVICELIST
		//free(pRawInputDeviceList);
	}
public:
	Inputs(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int NCmdShow) {
		getDevices();
		mainInfo *info = new mainInfo;
		info->h = hInstance;
		info->p = hPrevInstance;
		info->l = lpCmdLine;
		info->n = NCmdShow;
		_beginthread(poop, 0, info);
	}
	unsigned short read_device(int n) {
		return vkey_current[n];
		
	}
	
	 static void set_key(HANDLE handle, unsigned short vkey) {
		for (int i = 0; i < keyboards.size(); ++i) {
			if (keyboards[i].fh == handle)
				vkey_current[i] = vkey;
		}
				
	}
	
	friend ostream& operator <<(ostream& s, const Inputs& inp) {
		for (auto k : inp.keyboards) {
			cout << k.name << '\n';
		}
		return s;
	}
};

vector<Inputs::Keyboard> Inputs::keyboards;
vector<unsigned short> Inputs::vkey_current;



void process_keyboard_input(HWND hwnd, LPARAM lParam) {
	UINT dwSize;
	GetRawInputData((HRAWINPUT)lParam, RID_INPUT, NULL, &dwSize, sizeof(RAWINPUTHEADER));
	
	
	RAWINPUT *input = (RAWINPUT *)malloc(dwSize);
	GetRawInputData((HRAWINPUT)lParam, RID_INPUT, input, &dwSize, sizeof(RAWINPUTHEADER));
	
	
	if (input->header.dwType == RIM_TYPEKEYBOARD) {
		//printf("%p: %d\n", input->header.hDevice, input->data.keyboard.VKey);
		if (!input->data.keyboard.Flags) {
			Inputs::set_key(input->header.hDevice, input->data.keyboard.VKey);
		} else if (input->data.keyboard.Flags == 1) {
			Inputs::set_key(input->header.hDevice, -1);
		}
	}
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



void poop(void *data){
	
	struct mainInfo *info = (struct mainInfo *)data;
	
	HINSTANCE hInstance = info->h;
	HINSTANCE hPrevInstance = info->p;
	LPSTR lpCmdLine = info->l;
	int NCmdShow = info->n;
	

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
	
	return (void) msg.wParam;
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