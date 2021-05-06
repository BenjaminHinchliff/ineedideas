cmake_minimum_required(VERSION 3.8.0)

project("ineedideas")

set(CMAKE_C_STANDARD 99)
set(CMAKE_C_STANDARD_REQUIRED ON)
set(CMAKE_C_EXTENSIONS OFF)

add_executable(ineedideas "main.c")

target_compile_options(ineedideas PRIVATE -Wall -Wextra -pedantic -Werror)

configure_file(
	"${CMAKE_CURRENT_SOURCE_DIR}/words.txt"
	"${CMAKE_CURRENT_BINARY_DIR}/words.txt"
	COPYONLY
)
