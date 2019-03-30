#pragma once

class Shape {
private:
public:
	virtual void draw() {} // TODO: make this pure virtual and implement later
	virtual double surfaceArea() const = 0;
	virtual double volume() const = 0;
};
