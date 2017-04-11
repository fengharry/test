cmake_minimum_required(VERSION 2.8.11)

project(dyninst_test)

if(NOT DEFINED ENV{DYNINST_ROOT})
message(FATAL_ERROR "DYNINST_ROOT not defined!!!")
else()
message(DYNINST_ROOT=$ENV{DYNINST_ROOT})
endif()

set(CMAKE_VERBOSE_MAKEFILE on)
set(CMAKE_CXX_COMPILER "g++")
set(CMAKE_CXX_FLAGS "-Wall -g -O0 -std=c++11")

include_directories($ENV{DYNINST_ROOT})
include_directories($ENV{DYNINST_ROOT}/common/src)

add_executable(test_app test_app.cpp)

add_library(lib1 SHARED lib1.cpp)

add_custom_target(install 
			COMMAND sudo cp liblib1.so /usr/local/lib/
			COMMAND sudo ldconfig)
add_dependencies(install lib1)

add_executable(insn_decode insn_decode.cpp)
target_link_libraries(insn_decode instructionAPI)

add_executable(ParseAPI_test ParseAPI_test.cpp)
target_link_libraries(ParseAPI_test dyninstAPI parseAPI symtabAPI instructionAPI)

add_executable(ParseAPI_test2 ParseAPI_test2.cpp)
target_link_libraries(ParseAPI_test2 dyninstAPI parseAPI patchAPI symtabAPI instructionAPI)

add_executable(BPatch_test BPatch_test.cpp)
target_link_libraries(BPatch_test dyninstAPI instructionAPI)

add_executable(BPatch_test2 BPatch_test2.cpp)
target_link_libraries(BPatch_test2 dyninstAPI instructionAPI patchAPI)

add_executable(asm_test asm_test.cpp)

add_executable(test_chrono test_chrono.cpp)

add_executable(aesenc aesenc.cpp)

add_executable(test_gettimeofday test_gettimeofday.cpp)

add_executable(test_mutex test_mutex.cpp)
target_link_libraries(test_mutex pthread)
