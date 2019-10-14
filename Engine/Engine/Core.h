#pragma once

#ifdef ENGINE
#define ENGINE_API _declspec(dllexport)
#else
#define ENGINE_API _declspec(dllimport)
#endif

#include <SDL/SDL.h>
#include <SDL/SDL_image.h>

#include <GL/glew.h>

#include <glm/glm.hpp>
#include <glm/vec2.hpp>
#include <glm/vec3.hpp>
#include <glm/gtx/transform.hpp>

#include "imgui.h"
#include "imgui_impl_sdl.h"
#include "imgui_impl_opengl3.h"

#include <iostream>
#include <sstream>
#include <fstream>

#include <vector>
#include <set>
#include <list>
#include <map>
#include <queue>
#include <unordered_map>
#include <stack>

#include <memory>
#include <algorithm>
#include <string>

#include "Utils.h"
