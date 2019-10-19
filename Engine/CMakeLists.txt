add_library(Engine SHARED
        Engine/Core.cpp
        Engine/Core.h
        Engine/Application.cpp
        Engine/Application.h
        Engine/Entity.h
        Engine/GUI.h
        Engine/GUI.cpp
        Engine/Time.h
        Engine/Time.cpp
        Engine/VisibleEntity.h
        Engine/Enums.h)

target_compile_definitions(Engine PRIVATE ENGINE)
target_include_directories(Engine PUBLIC ${INCLUDE_DIR})
target_include_directories(Engine PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})
target_include_directories(Engine PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/Engine)

add_subdirectory(Engine/Data)
add_subdirectory(ImGUI)
add_subdirectory(Engine/Input)
add_subdirectory(Engine/Physics)
add_subdirectory(Engine/Rendering)
add_subdirectory(glad)
add_subdirectory(stb)

find_library(LIB_SDL NAMES sdl2d sdl2maind HINTS ${LIB_DIR})

target_link_libraries(Engine PUBLIC opengl32 glu32 mingw32 ${LIB_SDL} -mwindows)

add_definitions(-DSDL_MAIN_HANDLED)
set(LIBRARY_OUTPUT_PATH ${PROJECT_BINARY_DIR}/bin)

file(COPY ${BINARIES_DIR}/SDL2d.dll DESTINATION ${LIBRARY_OUTPUT_PATH})

#set_target_properties(Engine PROPERTIES COTIRE_CXX_PREFIX_HEADER_INIT "Engine/Core.h")
#cotire(Engine)
