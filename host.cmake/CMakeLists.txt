cmake_minimum_required(VERSION 2.8)

#compiler selection must precede project according to one advice, 
#CMAKE_CXX_COMPILER 
#CMAKE_C_COMPILER 
#CMAKE_RANLIB 
#CMAKE_AR 

project(lpcbase)

#need a local dir for binaries so that we can use multiple compilers.
set(CMAKE_BINARY_DIR built)
#cutting edge language features, the point of this project is to show their worth.
set( CMAKE_C_FLAGS "-std=c99" )
set( CMAKE_CXX_FLAGS "-Wall -std=c++11 " )

#target architecture is microcontroller

# do everything in the directory, 
aux_source_directory(.. SRC_LIST)
add_executable(${PROJECT_NAME} ${SRC_LIST})

# tool paths
MESSAGE( STATUS "CMAKE_C_COMPILER: " ${CMAKE_C_COMPILER} )
MESSAGE( STATUS "CMAKE_CXX_COMPILER: " ${CMAKE_CXX_COMPILER} )
MESSAGE( STATUS "CMAKE_AR: " ${CMAKE_AR} )
MESSAGE( STATUS "CMAKE_RANLIB: " ${CMAKE_RANLIB} )
