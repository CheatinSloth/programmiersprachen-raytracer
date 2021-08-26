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

glm::mat4 Camera::transform() const 
{
	glm::mat4 result;
	glm::vec3 normDir = glm::normalize(direction);
	glm::vec3 normUp = glm::cross(normDir, up);
	glm::vec3 normVec = glm::normalize(glm::cross(normUp, normDir));

	/*result[0] = { normUp.x, normUp.y, normUp.z, 0.f };
	result[1] = { normVec.x, normVec.y, normVec.z, 0.f };
	result[2] = { -normDir.x, -normDir.y, -normDir.z, 0.f };
	result[3] = { position.x, position.y, position.z, 0.f };*/

	return glm::mat4();
}
