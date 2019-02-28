/*
 * dbkbd.hh
 *
 *  Created on: Feb 25, 2019
 *      Author: pho
 */
#ifndef _DBKBD_H 
#define _DBKBD_H

#include <iostream>
#include <stdio.h>
#include <string>
#include <vector>
#include <unistd.h>
#include <linux/input.h>
#include <fcntl.h>
#include <pthread.h>

#define EV_MAKE 1
#define EV_BREAK 0
#define EV_REPEAT 2
using namespace std;

class Inputs {
private:
	// grab USB keyboard devices
	const static char *command;
	struct Keyboard
	{
		int fh;
		string name;
		Keyboard(const string& name) : name(name)
		{
			fh = open(name.c_str(), O_RDONLY);
			if (fh < 0) {
				throw "keyboard failed to open";
			}
			int flags = fcntl(fh, F_GETFL, 0); // save current flags
			fcntl(fh, F_SETFL, flags | O_NONBLOCK);  // OR in the nonblocking bit
			keys.push_back(VKey_Array());
		}
	};

	struct VKey_Array
	{
		bool keys[256] = {0};
		VKey_Array() {}
	};

	static vector<Keyboard> keyboards;	// keyboard objects
	static vector<VKey_Array> keys;		// key states

	static void getDevices()
	{
		FILE *grep_devices_p = popen(command, "r");
		char keyboardNameBuf[16];
		const string prefix = "/dev/input/";
		for (int i = 0; NULL != (fgets(keyboardNameBuf, 9, grep_devices_p)) && i < 10; ++i) {
			try {
				int j;
				for (j = 0; keyboardNameBuf[j] != '\n'; j++)
					;
				keyboardNameBuf[j] = '\0'; // remove the newline at the end
				keyboards.push_back(Keyboard(prefix + keyboardNameBuf));
			} catch (const char* msg) {
				cerr << msg << '\n';
				exit(1);
			}
		}
		pclose(grep_devices_p);
	}

	// Store into array of keys
	static void process_chars(int id, unsigned short scan_code, bool down)
	{
		keys[id].keys[scan_code] = down;
	}

	// Main polling loop that calls process_chars()
	static void *poll(void *)
	{
		unsigned short scan_code = 0;
		struct input_event event;

		// How do I do this in C++ with new?
		// May just toss this anyway
		bool *shift = (bool*)malloc(sizeof(bool) * keyboards.size());
		bool *ctrl = (bool*)malloc(sizeof(bool) * keyboards.size());
		if (shift == NULL || ctrl == NULL) {
			printf("Could not allocate memory for either shift or ctrl\n");
			exit(6);
		}

		for (int i = 0; i < keyboards.size(); ++i) {
			shift[i] = false;
			ctrl[i] = false;
		}
		int offset = 0;
		while (1) {
			if (read(keyboards[offset].fh, &event, sizeof(struct input_event))
					> 0) {
				if (event.type != EV_KEY) // always ev_key
					continue;

				scan_code = event.code;

				// EV_MAKE = pressed
				if (event.value == EV_MAKE) {
					if (scan_code == KEY_LEFTSHIFT || scan_code == KEY_RIGHTSHIFT)
						shift[offset] = true;
					else if (scan_code == KEY_LEFTCTRL || scan_code == KEY_RIGHTCTRL)
						ctrl[offset] = true;
					process_chars(offset, scan_code, true); 
				}

				// EV_BREAK = released
				if (event.value == EV_BREAK) {
					if (scan_code == KEY_LEFTSHIFT || scan_code == KEY_RIGHTSHIFT)
						shift[offset] = false;
					else if (scan_code == KEY_LEFTCTRL || scan_code == KEY_RIGHTCTRL)
						ctrl[offset] = false;
					process_chars(offset, scan_code, false); 
				}

				// Not really necessary but w/e
				if (event.value == EV_REPEAT)
					process_chars(offset, scan_code, true);
				
			}
			offset = (offset == keyboards.size() - 1) ? 0 : offset + 1;
			usleep(10000); // 10000us = 10ms
			fflush(stdout);
		}
	}


public:
	Inputs()
	{
		getDevices();
		// Polling thread
		pthread_t myThread;
		pthread_create(&myThread, NULL, &poll, NULL);
	}

	bool get_key(int id, unsigned int code)
	{
		return keys[id].keys[code];
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

const char *Inputs::command =
			"grep -E 'Handlers|EV=' /proc/bus/input/devices | grep -B1 'EV=120013' | grep -Eo 'event[0-9]+'";

#endif
