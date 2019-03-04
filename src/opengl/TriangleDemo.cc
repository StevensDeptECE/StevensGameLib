#include "GLWin.hh"
using namespace std;
class Triangle : public GLWin {
private:
	GLuint vao, vbo;
public:
	Triangle() : GLWin(1024,800, 0xFF0000, 0xFFFFFF, "triangle") {

	}
	void init() {

	}
	void render() {

	}
}
int main() {
	try {
		Triangle t;
	
		t.mainLoop();
	} catch(const char* msg) {
		cerr << msg << '\n';
	}
}
		
