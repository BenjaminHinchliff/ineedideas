set(HDRS
    "${PROJECT_SOURCE_DIR}/include/words.h")

set(SRCS
    "words.c"
    "main.c")

add_executable(ineedideas ${SRCS} ${HDRS})

set_property(TARGET ineedideas PROPERTY C_STANDARD 99)
set_property(TARGET ineedideas PROPERTY C_STANDARD_REQUIRED ON)
set_property(TARGET ineedideas PROPERTY C_EXTENSIONS OFF)

target_include_directories(ineedideas PUBLIC "${PROJECT_SOURCE_DIR}/include")

if (NOT MVSC AND CMAKE_BUILD_TYPE STREQUAL "Debug")
	target_compile_options(ineedideas PRIVATE -Wall -Wextra -pedantic -Werror)
endif()
