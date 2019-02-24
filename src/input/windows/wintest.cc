/*
 * wintest.cc
 *
 *  Created on: Feb 15, 2019
 *      Author: pho
 */

#include "winkbd.hh"
#include <time.h>

using namespace std;

void create_test_window(void *);

struct tagRectangle
{
	RECT r;
	int offsetX;
	int offsetY;
	int id;
	HBRUSH color;
	struct tagRectangle *next;
} *rectangles;

LRESULT CALLBACK WindowProc(HWND, UINT, WPARAM, LPARAM);
void refresh_window(void *win);

bool done = false;

int main()
{
	srand(time(NULL));
	
	Inputs i;
	rectangles = new tagRectangle;
	struct tagRectangle *head = rectangles;
	for (int j = 0; j < i.keyboard_count(); ++j) {
		head->offsetX = rand() % (GetSystemMetrics(SM_CXSCREEN) / 2 - 50);
		head->offsetY = rand() % (GetSystemMetrics(SM_CYSCREEN) / 2 - 50);
		head->r.left = head->offsetX;
		head->r.right = head->offsetX + 50;
		head->r.top = head->offsetY;
		head->r.bottom = head->offsetY + 50;
		head->id = j;
		head->color = CreateSolidBrush(RGB(rand()%255, rand()%255, rand()%255));
		head->next = new tagRectangle;
		if (j == i.keyboard_count() - 1)
			head->next = NULL;
		head = head->next;
	}
	
	_beginthread(create_test_window, 0, NULL);	// create game window
	
	while (!done) {
		head = rectangles;
		while (head) {
			unsigned short vkey = 0;
			if (i.is_down(head->id, VK_LEFT))
				head->offsetX -= 1;
			if (i.is_down(head->id, VK_RIGHT))
				head->offsetX += 1;
			if (i.is_down(head->id, VK_UP))
				head->offsetY -= 1;
			if (i.is_down(head->id, VK_DOWN))
				head->offsetY += 1;
			head->r.left = head->offsetX;
			head->r.top = head->offsetY;
			head->r.right = head->offsetX + 50;
			head->r.bottom = head->offsetY + 50;
			head = head->next;
		}
		Sleep(10);
	}
	return 0;
}

void create_test_window(void *)
{
	HINSTANCE hInstance = (HINSTANCE)GetModuleHandle(NULL);
	
	DWORD dwWidth = GetSystemMetrics(SM_CXSCREEN) / 2;
	DWORD dwHeight = GetSystemMetrics(SM_CYSCREEN) / 2;
	
	MSG  msg;
	WNDCLASSW wc = {0};

	wc.style = CS_HREDRAW | CS_VREDRAW;
	wc.lpszClassName = L"KeyboardDemoM";
	wc.hInstance     = hInstance;
	wc.hbrBackground = GetSysColorBrush(COLOR_3DFACE);
	wc.lpfnWndProc   = WindowProc;
	wc.hCursor       = LoadCursor(0, IDC_ARROW);
	char szTitle[]	 = "KeyboardDemoM";

	RegisterClassW(&wc);
	CreateWindowW(wc.lpszClassName, L"KeyboardDemoM",
	WS_OVERLAPPEDWINDOW | WS_VISIBLE,
	0, 0, dwWidth, dwHeight, NULL, NULL, hInstance, NULL);
	
	HWND hwnd;
	hwnd = FindWindow(NULL, szTitle);
	
	_beginthread(refresh_window, 0, hwnd);
	
	while (GetMessage(&msg, NULL, 0, 0)) {
		TranslateMessage(&msg);
		DispatchMessage(&msg);
	}
	return (void) msg.wParam;
}

void refresh_window(void *win)
{
	struct tagRectangle *head;
	HWND hwnd = (HWND)win;
	RECT rc;
	HDC hdc;
	PAINTSTRUCT ps;
	while (!done) {
		hdc = GetDC(hwnd);
		head = rectangles;
		GetWindowRect(hwnd, &rc);
		FillRect(hdc, &rc, (HBRUSH)(COLOR_WINDOW));
		while (head) {
			FillRect(hdc, &(head->r), head->color);
			head = head->next;
		}
		ReleaseDC(hwnd, hdc);
		Sleep(10);
	}
}

LRESULT CALLBACK WindowProc(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam)
{		
	switch(msg) {
	case WM_DESTROY:
		done = 1;
		PostQuitMessage(0);
		return 0;
	}
	return DefWindowProcW(hwnd, msg, wParam, lParam);
}