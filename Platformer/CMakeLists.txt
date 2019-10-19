
add_executable(Platformer
        main.cpp
        Application.h
        Application.cpp
        Bullet.h
        CharacterController.cpp
        CharacterController.h)

add_subdirectory(AI)
add_subdirectory(Player)
add_subdirectory(UI)

target_include_directories(Platformer PRIVATE ${CMAKE_CURRENT_SOURCE_DIR})

target_link_libraries(Platformer Engine)

set(EXECUTABLE_OUTPUT_PATH  ${PROJECT_BINARY_DIR}/bin)
set(RESOURCES_OUTPUT_PATH ${EXECUTABLE_OUTPUT_PATH}/res)

add_custom_command(TARGET Platformer POST_BUILD
        COMMAND ${CMAKE_COMMAND} -E make_directory
        ${EXECUTABLE_OUTPUT_PATH}/res)
add_custom_command(TARGET Platformer POST_BUILD
        COMMAND ${CMAKE_COMMAND} -E copy_directory
        ${CMAKE_SOURCE_DIR}/res/ ${RESOURCES_OUTPUT_PATH})