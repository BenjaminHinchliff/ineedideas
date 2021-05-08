cmake_minimum_required(VERSION 3.8.0)

project("ineedideas")

set(CMAKE_C_STANDARD 99)
set(CMAKE_C_STANDARD_REQUIRED ON)
set(CMAKE_C_EXTENSIONS OFF)

if (NOT MVSC AND CMAKE_BUILD_TYPE EQUAL "DEBUG")
	target_compile_options(ineedideas PRIVATE -Wall -Wextra -pedantic -Werror)
endif()

add_subdirectory("src")
