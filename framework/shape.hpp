#pragma once

class Shape {
public: 
	virtual double area() = 0;
	virtual double volume() = 0;
};

class Sphere : public Shape {
public:
};

class Box : public Shape {
public:
};