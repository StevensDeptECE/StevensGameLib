#include <glm/vec3.hpp>

#include <iostream>
#include <cmath>

double dist(const glm::vec3& a, const glm::vec3& b) {
	double dx = a.x - b.x, dy = a.y - b.y, dz = a.z - b.z;
	return sqrt(dx*dx + dy*dy + dz*dz);
}

std::ostream& operator <<(std::ostream& s, const glm::vec3& v) {
	return s << v.x << ", " << v.y << ", " << v.z;
}

double max(double a, double b, double c) {
	return a > b ? (a > c : a : c) : (b > c ? b : c);
}

class Triangle {
private:
	static constexpr double THIRD = 1.0/3;
	glm::vec3 a, b, c;
	glm::vec3 center;
	float boundingRadius; // radius of bounding sphere
public:
	Triangle(const glm::vec3& a, const glm::vec3& b, const glm::vec3& c) :
		a(a), b(b), c(c)
		//		center((a.x+b.x+c.x) * THIRD, (a.y+b.y+c.y) * THIRD,(a.z+b.z+c.z) * THIRD),
		//		boundingRadius(max(dist(center, a), dist(center,b), dist(center,c))) {
		//faster, approx center
	{
		computeCenter();
	}
	friend std::ostream& operator <<(std::ostream& s, const Triangle& t) {
		return s << t.a << '\t' << t.b << '\t' << t.c <<
			" center: " << t.center << " radius=" << t.boundingRadius;
	}
	void computeCenter() {
		double m1x = (a.x + b.x) * 0.5; // x midpoint edge 1
		double m1y = (a.y + b.y) * 0.5; // y midpoint edge 1
		double m1z = (a.z + b.z) * 0.5; // z midpoint edge 1
		double m2x = (b.x + c.x) * 0.5; // same for edge2
		double m2y = (b.y + c.y) * 0.5; // same for edge2
		double m2z = (b.z + c.z) * 0.5; // same for edge2

    double a1 = b.y - a.y, b1 = b.x - a.x, c1 = b1 * m1x + a1 * m1y;
    double a2 = c.y - b.y, b2 = c.x - b.x, c2 = b2 * m2x + a2 * m2y;

		
    double determinant = a2*b1 - a1*b2; 
    if (determinant == 0) {
			std::cerr <<"can't happen, degenerate triangle?";
    } else {
			determinant = 1/determinant;
			center.y = (b1*c2 - b2*c1)*determinant; 
			center.x = (a2*c1 - a1*c2)*determinant;
			center.z = 0; //TODO: Full 3d code
			boundingRadius = dist(center, a);
			//			std::cout << "Center(correct)=" << x << ',' << y << '\n';
			//			glm::vec3 center(x,y,0);
			//			std::cout << dist(center,a) << '\t' << dist(center,b) << '\t' << dist(center,c) << '\n';
    } 
	}
};
