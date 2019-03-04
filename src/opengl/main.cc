#include "GLWin.hh"
using namespace std;
int main() {
	try {
		GLWin w(1024, 800, 0xFF0000, 0xFFFFFF, "test");
	
		w.mainLoop();
	} catch(const char* msg) {
		cerr << msg << '\n';
	}
}
		
