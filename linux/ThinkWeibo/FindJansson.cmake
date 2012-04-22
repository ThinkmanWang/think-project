
MESSAGE(STATUS "Using FindJansson.cmake...")

FIND_PATH(
    LIBOJANSSON_INCLUDE_DIR
    jansson.h 
    /usr/include/
    /usr/local/include/ )

FIND_LIBRARY(
    LIBJANSSON_LIBRARIES NAMES jansson
    PATHS /usr/lib /usr/local/lib/
    )
