cmake_minimum_required(VERSION 3.23)
project(epitech_wolf3d_project)

set(CMAKE_CXX_STANDARD 23)

add_executable(epitech_wolf3d_project main.cpp)

#directory where the cmake files for SFML are. You will want to change this to wherever your SFML installation is
set( SFML_DIR "/usr/lib/x86_64-linux-gnu/cmake/SFML" )
find_package(SFML 2.5.1 COMPONENTS system graphics window REQUIRED)

target_link_libraries( ${PROJECT_NAME}
        PRIVATE

        sfml-system
        sfml-graphics
        sfml-window
        )

