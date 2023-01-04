cmake_minimum_required(VERSION 3.23.2)
project(vulkan_project LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE__CXX_FLAGS "-Wall -O2")

if(APPLE)
    find_package(vulkan REQUIRE)
else()
    find_package(Vulkan REQUIRED)
endif()

find_package(glfw3 REQUIRED)
find_package(glm REQUIRED)

set(EXEC_SOURCES
    src/main.cpp
    src/VulkanApp.cpp
    src/VulkanUtils.cpp
)

add_executable(${PROJECT_NAME} ${EXEC_SOURCES})
target_link_libraries(${PROJECT_NAME} Vulkan::Vulkan glfw)
include_directories("./lib")

# SHADERS
set(GLSL_VALIDATOR "/usr/bin/glslangValidator")

file(GLOB_RECURSE GLSL_SOURCE_FILES
    "shaders/*.frag"
    "shaders/*.vert"
)

foreach(GLSL ${GLSL_SOURCE_FILES})
    get_filename_component(FILE_NAME ${GLSL} NAME)
    set(SPIRV "${PROJECT_BINARY_DIR}/shaders/${FILE_NAME}.spv")
    add_custom_command(
        OUTPUT ${SPIRV}
        COMMAND ${CMAKE_COMMAND} -E make_directory "${PROJECT_BINARY_DIR}/shaders/"
        COMMAND ${GLSL_VALIDATOR} -V ${GLSL} -o ${SPIRV}
        DEPENDS ${GLSL}
    )
    list(APPEND SPIRV_BINARY_FILES ${SPIRV})
endforeach(GLSL)

add_custom_target(
    Shaders
    DEPENDS ${SPIRV_BINARY_FILES}
)

add_dependencies(vulkan_project Shaders)

add_custom_command(TARGET ${PROJECT_NAME} POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E make_directory "$<TARGET_FILE_DIR:vulkan_project>/shaders/"
    COMMAND ${CMAKE_COMMAND} -E copy_directory "${PROJECT_BINARY_DIR}/shaders" "$<TARGET_FILE_DIR:vulkan_project>/shaders"
)

add_custom_command(TARGET ${PROJECT_NAME} POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E make_directory "$<TARGET_FILE_DIR:vulkan_project>/textures/"
    COMMAND ${CMAKE_COMMAND} -E copy_directory "${PROJECT_SOURCE_DIR}/textures" "$<TARGET_FILE_DIR:vulkan_project>/textures"
)

add_custom_command(TARGET ${PROJECT_NAME} POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E make_directory "$<TARGET_FILE_DIR:vulkan_project>/models/"
    COMMAND ${CMAKE_COMMAND} -E copy_directory "${PROJECT_SOURCE_DIR}/models" "$<TARGET_FILE_DIR:vulkan_project>/models/"
)

