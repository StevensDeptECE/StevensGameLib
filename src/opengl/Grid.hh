#pragma once

#include "Lines2D.hh"

class Grid : public Lines2D {
private:
public:
	Grid(uint32_t m, uint32_t n) : Lines(m+n) {
		float x = 0;
		const float dx = 1.0/m;
		for (uint32_t i = 0; i < m; i++, x += dx) {
			add(x,0, x,1); // add a vertical line
			add(0,x, 1,x); // add a horizontal line
		}
	}

};		
