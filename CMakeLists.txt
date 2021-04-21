#cmake min version
cmake_minimum_required(VERSION 2.6)

#project name
project(a.out) 

#set output apth
set (EXECUTABLE_OUTPUT_PATH ${PROJECT_SOURCE_DIR}/bin)

#header files
include_directories(/usr/local/include)

#dubug
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11 -lcurl -pthread")


#link
link_directories(/usr/local/lib)

#source file
aux_source_directory(. SRC_LIST)
add_executable(a.out ${SRC_LIST})


target_link_libraries (a.out libcrypto.so  libcurl.so  libssl.so)
