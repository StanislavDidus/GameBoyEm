cmake_minimum_required(VERSION 4.2)
project(GameBoyEm)

set(CMAKE_CXX_STANDARD 20)
set(BUILD_SHARED_LIBS OFF CACHE BOOL "Don't build shared libraries." FORCE)

add_subdirectory(externals)

set(HEADER_FILES
        src/CPU/CPU.hpp
        src/MemoryBus/MemoryBus.hpp
        src/DMG.hpp
        src/Utils/Log.hpp
        src/CPU/ControlUnit.hpp
        src/Utils/Math.hpp
        src/CPU/Registers/ByteRegister.hpp
        src/CPU/Registers/ByteRegisterPair.hpp
        src/CPU/Registers/TwoByteRegister.hpp
        src/CPU/Registers/ATwoByteRegister.hpp
        src/CPU/Registers/FlagRegister.hpp
        src/CPU/Registers/AByteRegister.hpp
        src/CPU/Registers/Address.hpp
        src/Graphics/Graphics.hpp
        src/UI/UI.hpp
)

set(SOURCE_FILES
        src/main.cpp
        src/Utils/Log.cpp
        src/CPU/ControlUnit.cpp
        src/DMG.cpp
        src/CPU/CPU.cpp
)

set(GLAD_FILES
        externals/glad/src/glad.c
)

set(ALL_FILES
        ${HEADER_FILES}
        ${SOURCE_FILES}
        ${GLAD_FILES}
)

add_executable(GameBoyEm ${ALL_FILES})


add_custom_target(copy_command
    COMMAND ${CMAKE_COMMAND} -P ${CMAKE_CURRENT_LIST_DIR}/copy_files.cmake
)
add_dependencies(GameBoyEm copy_command)

target_include_directories(GameBoyEm PRIVATE
        src
        externals/glad/include
)

target_link_libraries(GameBoyEm PRIVATE spdlog::spdlog glfw imgui)