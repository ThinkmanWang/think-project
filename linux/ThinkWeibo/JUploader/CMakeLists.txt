
cmake_minimum_required(VERSION 2.8.6)
find_package(Java)
include(UseJava)
project(Uploader Java)
set(SRC
    src/com/thinkman/uploader/Uploader.java
    )
MESSAGE(STATUS ${Uploader_SOURCE_DIR} )
set(LIBS 
    ${Uploader_SOURCE_DIR}/lib/weibo4j.jar 
    ${Uploader_SOURCE_DIR}/lib/commons-codec.jar 
    ${Uploader_SOURCE_DIR}/lib/commons-httpclient-3.1.jar 
    ${Uploader_SOURCE_DIR}/lib/commons-logging-1.1.jar 
    )
set(CMAKE_JAVA_INCLUDE_PATH ${LIBS})

INCLUDE_DIRECTORIES(${Uploader_SOURCE_DIR}/lib/weibo4j.jar)
INCLUDE_DIRECTORIES(${Uploader_SOURCE_DIR}/lib/commons-codec.jar)
INCLUDE_DIRECTORIES(${Uploader_SOURCE_DIR}/lib/commons-httpclient-3.1.jar)
INCLUDE_DIRECTORIES(${Uploader_SOURCE_DIR}/lib/commons-logging-1.1.jar)

set(CMAKE_JAVA_TARGET_OUTPUT_NAME Uploader.jar)
add_jar(Uploader ${SRC} ${LIBS})
set(CMAKE_JAVA_MANIFEST MANIFEST.MF)
set(CMAKE_JAVA_EXECUTABLE src/com/thinkman/uploader/Uploader)
