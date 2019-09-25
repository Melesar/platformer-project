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