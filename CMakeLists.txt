cmake_minimum_required(VERSION 3.16)
cmake_policy(SET CMP0048 NEW)

project(oled-driver LANGUAGES CXX C ASM)

add_library(${PROJECT_NAME} STATIC
        src/Display.cxx
        src/Image.cxx
        src/Renderer.cxx
        fonts/mono.cxx
        )

target_include_directories(${PROJECT_NAME} PUBLIC
        include
        .
        )

target_link_libraries(${PROJECT_NAME})
