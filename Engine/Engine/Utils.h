#pragma once
#include <Core.h>
#include "Physics/BoundingBox.h"

inline std::ostream& operator << (std::ostream& stream, glm::vec3 vec)
{
	stream << "(" << vec.x << ", " << vec.y << ", " << vec.z << ")";
	return stream;
}

inline std::ostream& operator << (std::ostream& stream, glm::vec2 vec)
{
	stream << "(" << vec.x << ", " << vec.y << ")";
	return stream;
}

inline glm::vec3 vec3 (const glm::vec2& vec, float z)
{
	return glm::vec3(vec.x, vec.y, z);
}

inline glm::vec2 vec2 (const glm::vec3& vec)
{
	return glm::vec2(vec.x, vec.y);
}

inline void drawBoundingBox(glm::vec2 min, glm::vec2 max, const glm::mat3x3 matrix)
{
	glm::vec2 boxMin = matrix * vec3(min, 1);
	glm::vec2 boxMax = matrix * vec3(max, 1);

	glBegin(GL_LINES);
	glColor3d(0, 1, 0);
	glVertex2f(boxMin.x, boxMin.y);
	glVertex2f(boxMin.x, boxMax.y);

	glVertex2f(boxMin.x, boxMax.y);
	glVertex2f(boxMax.x, boxMax.y);

	glVertex2f(boxMax.x, boxMax.y);
	glVertex2f(boxMax.x, boxMin.y);

	glVertex2f(boxMax.x, boxMin.y);
	glVertex2f(boxMin.x, boxMin.y);
	glEnd();
}