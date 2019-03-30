#include "Triangle.hh"
using namespace glm;
using namespace std;

int main() {
	f();
	Triangle t(vec3(0,0,0), vec3(1,0,0), vec3(0.5,1,0));
	cout << t << '\n';

	Triangle t2(vec3(0,0,0), vec3(1,0,0), vec3(0,1,0));
	cout << t2 << '\n';

	Triangle t3(vec3(0,0,0), vec3(2,1,0), vec3(1,2,0));
	cout << t3 << '\n';

	Triangle t4(vec3(0,0,0), vec3(4,1,0), vec3(4,0,0));
	cout << t4 << '\n';
}
