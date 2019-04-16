/*
 * dbkbd.hh
 *
 *  Created on: Feb 25, 2019
 *      Author: pho
 */
#pragma once

#include <vector>
#include <iostream>

class Inputs {
private:
	const static char *command;
	struct Keyboard;
	struct VKey_Array;
	struct Key;
	static std::vector<Keyboard> keyboards;	// keyboard objects
	static std::vector<VKey_Array> keys;	// key states
	static void process_chars(int id, unsigned short scan_code, bool down);
	static void *poll(void *);
	static void get_devices();	
	static void process_callbacks();

	static void (*handlers[2048])();	// callbacks
public:
	Inputs();
	bool get_key(int id, unsigned int code);
	int keyboard_count();
	void set_callback_handler(void (*f)(), unsigned int code);
	friend std::ostream& operator<<(std::ostream& s, const Inputs& inp);
};

