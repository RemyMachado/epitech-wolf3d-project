cmake_minimum_required(VERSION 3.23)
project(epitech_wolf3d_project)

set(CMAKE_CXX_STANDARD 23)
set(CMAKE_CXX_FLAGS_RELEASE "-O3")

add_executable(epitech_wolf3d_project src/main.cpp src/game_loop.cpp src/game_loop.hpp src/lines.cpp src/lines.hpp src/my_math.cpp src/my_math.hpp src/raycasting.cpp src/raycasting.hpp src/file_parser.cpp src/file_parser.hpp src/Grid.hpp src/player.cpp src/player.hpp src/draw.cpp src/draw.hpp)

#directory where the cmake files for SFML are. You will want to change this to wherever your SFML installation is
set(SFML_DIR "/usr/lib/x86_64-linux-gnu/cmake/SFML")
find_package(SFML 2.5.1 COMPONENTS system graphics window REQUIRED)

target_link_libraries(${PROJECT_NAME}
        PRIVATE

        sfml-system
        sfml-graphics
        sfml-window
        )

