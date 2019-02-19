/*
 * wintest.cc
 *
 *  Created on: Feb 15, 2019
 *      Author: pho
 */

#include "winkbd.hh"

int main()
{
	Inputs i;
	
	unsigned short vkey = 0;
	while(1) {
		printf("%d\n", i.read_device(1));
		fflush(stdout);
		Sleep(10);
	}
	
	
	return 0;
}