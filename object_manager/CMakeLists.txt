project("object manager")
cmake_minimum_required(VERSION 3.16)

set(CMAKE_CXX_FLAGS "-std=c++17 -ggdb3" )

add_executable(object_manager
    main.cpp
    entity.cpp
    object_manager.cpp
    entity_actions_implementation.cpp)

