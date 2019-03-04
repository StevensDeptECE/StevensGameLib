#include "GLwin.hh"
using namespace std;
int main() {
	try {
		GLWin w(1024, 800, 0xFF0000, 0x000000, "test window");


		
		w.mainLoop();
	} catch(const char* msg) {
		cerr << msg << '\n';
	}
}
		
