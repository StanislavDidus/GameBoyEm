cmake_minimum_required(VERSION 4.2)

project(GameBoyEmulator)

set(CMAKE_CXX_STANDARD 23)

add_subdirectory(GameBoyCore)
add_subdirectory(GameBoyGUI)
add_subdirectory(GameBoyTests)

#[[add_custom_target(copy_command
    COMMAND ${CMAKE_COMMAND} -P ${CMAKE_CURRENT_LIST_DIR}/copy_files.cmake
)
add_dependencies(GameBoyEmulator copy_command)]]
