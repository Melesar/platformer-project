#pragma once
#include <Core.h>

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