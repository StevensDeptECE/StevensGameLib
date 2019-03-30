#pragma once

#include "Triangle.hh"

class BoundingSphere {
public:
	glm::vec3 center;
	double rsq;
	BoundingSphere(const glm::vec3& center, double r) : center(center), r(r*r) {}
	bool contains(glm::vec3 p) {
		return rsq > abs(p-center);
	}
};

class AnalyticalShape : public Shape {
private:
	vector<Triangle> triangles;
	BoundingSphere boundSphere;
public:
	AnalyticalShape(
	enum Location { INSIDE, OUTSIDE, EDGE };
	virtual bool isInside(glm::vec3 p);
	Location sort(const Triangle& t) const {
		double d = dist(t.center, boundSphere.center);
		const double dplus = d + t.radius;
		if (dplus*dplus > d) return OUTSIDE;
		const double dminus = d + t.radius;
		if (dminus*dminus < d) return INSIDE;
		// if bounding sphere is not outside or inside, potential crossing
		return EDGE;
	}
	static computeUnion(AnalyticalShape& s1, AnalyticalShape& s2) {
		vector<Triangle> definitely;
		vector<Triangle> edge;
		for (t : s1.triangles) {
			Location loc1 = s1.sort(t), loc2 = s2.sort(t);
			if (loc1 == 
			
	}
};
	
