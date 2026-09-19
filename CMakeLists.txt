cmake_minimum_required(VERSION 4.2)
project(GameBoyEm)

set(CMAKE_CXX_STANDARD 20)
set(BUILD_SHARED_LIBS OFF CACHE BOOL "Don't build shared libraries." FORCE)

add_subdirectory(externals)

add_executable(GameBoyEm src/main.cpp
        src/CPU/CPU.hpp
        src/MemoryBus/MemoryBus.hpp
        src/DMG.hpp
        src/Utils/Log.hpp
        src/Utils/Log.cpp
        src/CPU/ControlUnit.hpp
        src/CPU/ControlUnit.cpp
        src/DMG.cpp
        src/CPU/CPU.cpp
        src/Utils/Math.hpp
        src/CPU/Registers/ByteRegister.hpp
        src/CPU/Registers/ByteRegisterPair.hpp
        src/CPU/Registers/TwoByteRegister.hpp
        src/CPU/Registers/ATwoByteRegister.hpp
        src/CPU/Registers/FlagRegister.hpp
        src/CPU/Registers/AByteRegister.hpp
        src/CPU/Registers/Address.hpp
)

target_include_directories(GameBoyEm PRIVATE src/)

# target_compile_definitions(GameBoyEm PRIVATE SPDLOG_COMPILED_LIB)

target_link_libraries(GameBoyEm PRIVATE spdlog::spdlog)