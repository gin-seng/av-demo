cmake_minimum_required(VERSION 2.8)

project(demo)

aux_source_directory(./c_cpp/kkav DIR_SRCS)

add_executable(demo ${DIR_SRCS}) 
