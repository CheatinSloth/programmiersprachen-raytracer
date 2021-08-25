#include "camera.hpp"
using std::sin;
using std::sqrt;
using std::pow;

// Calculates dist from cam to visual plane
float Camera::dist() const
{
	float radianAngle = angle * ((2.f * M_PI) / 360.f);
	float sideLen = ((1.f / sin(radianAngle / 2.f)) / 2.f);

	return (sqrt((4.f * pow(sideLen, 2.f) - 1))) / 2.f;
}
