set(HDRS
    "${PROJECT_SOURCE_DIR}/include/words.h")

set(SRCS
    "words.c"
    "main.c")

add_executable(ineedideas ${SRCS} ${HDRS})

target_include_directories(ineedideas PUBLIC "${PROJECT_SOURCE_DIR}/include")
