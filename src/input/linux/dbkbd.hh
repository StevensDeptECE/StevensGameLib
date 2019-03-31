/*
 * dbkbd.hh
 *
 *  Created on: Feb 25, 2019
 *      Author: pho
 */
#ifndef _DBKBD_H 
#define _DBKBD_H

#include <vector>
#include <iostream>

class Inputs {
private:
	const static char *command;
	struct Keyboard;
	struct VKey_Array;
	static std::vector<Keyboard> keyboards;	// keyboard objects
	static std::vector<VKey_Array> keys;	// key states
	static void process_chars(int id, unsigned short scan_code, bool down);
	static void *poll(void *);
	static void getDevices();	
public:
	Inputs();
	bool get_key(int id, unsigned int code);
	int keyboard_count();
	friend std::ostream& operator <<(std::ostream& s, const Inputs& inp);
};

#endif
