cmake_minimum_required(VERSION 3.10)
project(PXCF VERSION 1.0.0 LANGUAGES C)

set(CMAKE_C_STANDARD 11)
set(CMAKE_C_STANDARD_REQUIRED ON)

# Options
option(PXCF_BUILD_CLI "Build the pxcf CLI tool" ON)
option(PXCF_BUILD_TESTS "Build the test suite" ON)
option(PXCF_BUILD_FUZZERS "Build the fuzz targets (requires clang)" OFF)

option(PXCF_BUILD_SHARED "Build a shared library" OFF)
option(PXCF_BUILD_BENCHMARKS "Build the benchmarks" OFF)
option(PXCF_ENABLE_SANITIZERS "Enable Address and UndefinedBehavior sanitizers" OFF)

if(PXCF_ENABLE_SANITIZERS)
    if(CMAKE_C_COMPILER_ID MATCHES "Clang" OR CMAKE_C_COMPILER_ID MATCHES "GNU")
        set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -fsanitize=address,undefined -g -fno-omit-frame-pointer")
        set(CMAKE_EXE_LINKER_FLAGS "${CMAKE_EXE_LINKER_FLAGS} -fsanitize=address,undefined")
        set(CMAKE_SHARED_LINKER_FLAGS "${CMAKE_SHARED_LINKER_FLAGS} -fsanitize=address,undefined")
    endif()
endif()

# Core library
set(PXCF_SOURCES
    src/lexer.c
    src/parser.c
    src/value.c
    src/serializer.c
    src/error.c
    src/pxcf.c
)

if(PXCF_BUILD_SHARED)
    add_library(pxcf SHARED ${PXCF_SOURCES})
    set_target_properties(pxcf PROPERTIES SOVERSION ${PROJECT_VERSION})
else()
    add_library(pxcf STATIC ${PXCF_SOURCES})
endif()

target_include_directories(pxcf PUBLIC
    $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>
    $<INSTALL_INTERFACE:include>
)
target_include_directories(pxcf PRIVATE ${CMAKE_CURRENT_SOURCE_DIR}/src)

# Command-line tool
if(PXCF_BUILD_CLI)
    add_executable(pxcf_cli cli/main.c cli/commands.c)
    set_target_properties(pxcf_cli PROPERTIES OUTPUT_NAME "pxcf")
    target_link_libraries(pxcf_cli PRIVATE pxcf)
endif()

# Tests
if(PXCF_BUILD_TESTS)
    enable_testing()
    add_subdirectory(tests)
endif()
include(GNUInstallDirs)
install(TARGETS pxcf
    EXPORT pxcfTargets
    LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}
    ARCHIVE DESTINATION ${CMAKE_INSTALL_LIBDIR}
    RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR}
    INCLUDES DESTINATION ${CMAKE_INSTALL_INCLUDEDIR}
)
install(DIRECTORY include/pxcf DESTINATION ${CMAKE_INSTALL_INCLUDEDIR})
if(PXCF_BUILD_CLI)
    install(TARGETS pxcf_cli RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR})
endif()