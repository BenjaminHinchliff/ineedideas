cmake_minimum_required(VERSION 3.8.0)

project("ineedideas")

set(CMAKE_C_STANDARD 99)
set(CMAKE_C_STANDARD_REQUIRED ON)
set(CMAKE_C_EXTENSIONS OFF)

add_executable(ineedideas "main.c" "words.c")

if (NOT MVSC)
	target_compile_options(ineedideas PRIVATE -Wall -Wextra -pedantic -Werror)
endif()
