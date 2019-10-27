target_sources(Engine PRIVATE
        Entity.h
        Manager.h
        System.h
        SystemsQueue.h
        ECS.h)

target_include_directories(Engine PRIVATE ${CMAKE_CURRENT_SOURCE_DIR})