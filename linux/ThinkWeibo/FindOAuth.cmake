
MESSAGE(STATUS "Using FindOAuth.cmake...")

FIND_PATH(
    LIBOAUTH_INCLUDE_DIR
    oauth.h 
    /usr/include/ 
    /usr/local/include/ 
    )

FIND_LIBRARY(
    LIBOAUTH_LIBRARIES NAMES oauth
    PATHS /usr/lib/ /usr/local/lib/
    )
