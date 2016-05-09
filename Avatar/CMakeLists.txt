cmake_minimum_required(VERSION 2.6)

project(Avatar)
SET(C_MAKE_BUILD_TYPE debug)
#SET(CMAKE_BUILD_TYPE Release)

add_definitions(-DUNIX)

SET(CMAKE_MODULE_PATH ${Avatar_SOURCE_DIR}/cmake)
list(APPEND CMAKE_MODULE_PATH ${CMAKE_CYRRENT_SOURCE_DIR}/cmake)

message("Cmake module path: ${CMAKE_MODULE_PATH}\n")

find_package(SDL REQUIRED)
find_package(SDL_image REQUIRED)

SET(OPENGL_DEPENDENCIES GLU GLEW GL glut)

include_directories(
	include
	${SDL_INCLUDE_DIR}
)

link_directories(
	${Avatar_SOURCE_DIR}/lib
)

file(GLOB_RECURSE
	SOURCE_FILES
	src/*
	include/*
)

ADD_EXECUTABLE(Avatar ${SOURCE_FILES})

TARGET_LINK_LIBRARIES(Avatar ${SDL_LIBRARY} ${SDLIMAGE_LIBRARY} ${OPENGL_DEPENDENCIES} OpenNI2)