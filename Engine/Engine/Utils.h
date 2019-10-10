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

inline std::ostream& operator << (std::ostream& stream, Engine::BoundingBox box)
{
	stream << "Min: " << box.min << ", max: " << box.max;
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

inline bool isClose (glm::vec2 p1, glm::vec2 p2)
{
	return glm::distance(p1, p2) < 0.5f;
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

inline void drawQuad(glm::vec2 center, glm::vec2 extents, const glm::mat3x3& matrix, const glm::vec3& color)
{
	glBegin(GL_QUADS);
	glColor3d(color.r, color.g, color.b);

	glm::vec2 pMin = matrix * vec3(center + extents, 1);
	glm::vec2 pMax = matrix * vec3(center - extents, 1);

	glVertex2f(pMin.x, pMin.y);
	glVertex2f(pMin.x, pMax.y);

	glVertex2f(pMin.x, pMax.y);
	glVertex2f(pMax.x, pMax.y);

	glVertex2f(pMax.x, pMax.y);
	glVertex2f(pMax.x, pMin.y);

	glVertex2f(pMax.x, pMin.y);
	glVertex2f(pMin.x, pMin.y);
	glEnd();
}

inline void drawLine(glm::vec2 start, glm::vec2 end, const glm::mat3x3& matrix, const glm::vec3& color)
{
	glBegin(GL_LINES);
	glColor3d(color.r, color.g, color.b);

	glm::vec2 pMin = matrix * vec3(start, 1);
	glm::vec2 pMax = matrix * vec3(end, 1);

	glVertex2f(pMin.x, pMin.y);
	glVertex2f(pMin.x, pMax.y);
	
	glEnd();
}