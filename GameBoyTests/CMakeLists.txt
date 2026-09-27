cmake_minimum_required(VERSION 4.2)

project(GameBoyTests)

set(CMAKE_CXX_STANDARD 23)
set(BUILD_SHARED_LIBS OFF CACHE BOOL "Don't build shared libraries." FORCE)

add_subdirectory(externals)

set(HEADER_FILES
)

set(SOURCE_FILES
        src/main.cpp
)

set(ALL_FILES
        ${HEADER_FILES}
        ${SOURCE_FILES}
)

add_executable(GameBoyTests ${ALL_FILES})

add_custom_target(copy_sst
        COMMAND ${CMAKE_COMMAND} -P ${CMAKE_CURRENT_LIST_DIR}/copy_files.cmake
)
add_dependencies(GameBoyTests copy_sst)

target_include_directories(GameBoyTests PRIVATE
        src
)

target_link_libraries(GameBoyTests PUBLIC GameBoyCore nlohmann_json::nlohmann_json)
