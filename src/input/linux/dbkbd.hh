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
		uint32_t keys[8] = {0};	// 256 keys / 32 bits/key
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

	inline static void process_chars(int id, unsigned short scan_code, bool down)
	{
		keys[id].keys[scan_code>>5] ^= 1 << (scan_code&0x1F);
	}

	// Main polling loop that calls process_chars()
	static void *poll(void *)
	{
		struct input_event event;

		int offset = 0;
		long int size_event = sizeof(struct input_event);
		while (1) {
			if (read(keyboards[offset].fh, &event, size_event) > 0
					&& event.code) {

				// EV_MAKE = pressed
				if (event.value == EV_MAKE) {
					process_chars(offset, event.code, true); 
				}

				// EV_BREAK = released
				if (event.value == EV_BREAK) {
					process_chars(offset, event.code, false); 
				}
			}
			offset = (offset == keyboards.size() - 1) ? 0 : offset + 1;
			usleep(1000); // 1000us = 1ms
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
		return keys[id].keys[code>>5] & 1 << (code&0x1F);
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
