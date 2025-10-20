cmake_minimum_required(VERSION 3.15...4.0)

project(Assembler VERSION 0.1 LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

set(SOURCES
	Source.cpp
	Parser.cpp
	Lexer.cpp
	utils/Radix.cpp
)

set(HEADERS
	Parser.h
	Lexer.h
	utils/Radix.h
	Tokens.h
)

add_executable(Assembler ${HEADERS} ${SOURCES})

target_include_directories(Assembler PRIVATE ../libsLocal)