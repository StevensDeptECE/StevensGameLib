#include "GLWin.hh"
#include "GL/gl.h"

using namespace std;
class Triangle : public GLWin {
private:
public:
	Triangle() : GLWin(1024,800, 0xFF0000, 0xFFFFFF, "triangle") {

	}
	void init() {

	}
	void render() {
	}
};

int main() {
	try {
		Triangle t;
	
		t.mainLoop();
	} catch(const char* msg) {
		cerr << msg << '\n';
	}
}
		
