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

add_library(test SHARED libtest.cpp)

add_custom_target(install 
			COMMAND sudo cp libtest.so /usr/local/lib/
			COMMAND sudo ldconfig)
add_dependencies(install test)

add_executable(decode_insn dyninst/decode_instruction.cpp)
target_link_libraries(decode_insn instructionAPI)

add_executable(SymtabAPI_disassemble dyninst/SymtabAPI_disassemble.cpp)
target_link_libraries(SymtabAPI_disassemble dyninstAPI parseAPI symtabAPI instructionAPI)

add_executable(CodeObject_disassemble dyninst/CodeObject_disassemble.cpp)
target_link_libraries(CodeObject_disassemble dyninstAPI parseAPI patchAPI symtabAPI instructionAPI)

add_executable(BPatch_disassemble dyninst/BPatch_disassemble.cpp)
target_link_libraries(BPatch_disassemble dyninstAPI instructionAPI)

add_executable(insert_1function dyninst/insert_1function.cpp)
target_link_libraries(insert_1function dyninstAPI instructionAPI patchAPI)

add_executable(insert_printf_all dyninst/insert_printf_all.cpp)
target_link_libraries(insert_printf_all dyninstAPI patchAPI)

add_executable(asm_test asm_test.cpp)
add_executable(aesenc aesenc.cpp)

add_executable(chrono testAPI/chrono.cpp)
add_executable(gettimeofday testAPI/gettimeofday.cpp)

add_executable(mutex testAPI/mutex.cpp)
target_link_libraries(mutex pthread)

add_executable(openssl testAPI/openssl.cpp)
target_link_libraries(openssl crypto)
