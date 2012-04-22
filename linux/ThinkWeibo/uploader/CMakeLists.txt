
project(uploader)
#EXECUTE_PROCESS(COMMAND javac Uploader.java -classpath weibo4j.jar 
#    WORKING_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR})

MESSAGE(STATUS "*****************copy Uploader.jar******************" )
EXECUTE_PROCESS(COMMAND cp ${CMAKE_CURRENT_SOURCE_DIR}/Uploader.jar ${uploader_BINARY_DIR})
