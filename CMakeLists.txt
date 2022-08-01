cmake_minimum_required(VERSION 3.24)
project(vulkan_project LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

if(APPLE)
    find_package(vulkan REQUIRED FATAL_ERROR)
else()
    find_package(Vulkan REQUIRED FATAL_ERROR)
endif()

find_package(glfw3 REQUIRED FATAL_ERROR)
find_package(glm REQUIRED FATAL_ERROR)

set(EXEC_SOURCES
	main.cpp
)

add_executable(${PROJECT_NAME} ${EXEC_SOURCES})
target_link_libraries(${PROJECT_NAME} Vulkan::Vulkan glfw)
