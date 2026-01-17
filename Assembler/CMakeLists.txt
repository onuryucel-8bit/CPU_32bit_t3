cmake_minimum_required(VERSION 3.15...4.0)

project(Assembler VERSION 0.1 LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

set(LIBS_LOCAL_DIR ${CMAKE_CURRENT_SOURCE_DIR}/../libsLocal)

set(SOURCES
	Source.cpp
	Parser.cpp
	Lexer.cpp
	LexerPCH.cpp
	LogisimPrinter.cpp	
)

set(HEADERS
	Parser.h
	Lexer.h
	LexerPCH.h
	LogisimPrinter.h
	Tokens.h
	Timer.h
	Common.h
)

add_executable(Assembler ${SOURCES} ${LIBS_LOCAL_DIR}/utils/Radix.h)

target_include_directories(Assembler PRIVATE 
	${LIBS_LOCAL_DIR}
)

target_sources(Assembler PRIVATE
    ${HEADERS}
    ${LIBS_LOCAL_DIR}/utils/Radix.h
)