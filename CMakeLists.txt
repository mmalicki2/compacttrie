cmake_minimum_required (VERSION 2.6) 
project (TRIE) 

#set(CMAKE_CXX_FLAGS "-O0 -static -std=c++11 -g -DDEBUG")
set(CMAKE_CXX_FLAGS "-O2 -static -std=c++11 -DNDEBUG")
#set(CMAKE_CXX_FLAGS "-O2 -static -std=c++11 -g -DNDEBUG")

include_directories(libutils)
include_directories(libmemory)
include_directories(libsimpletrie)

add_subdirectory (libutils)
add_subdirectory (libcompacttrie) 
add_subdirectory (libsimpletrie) 
add_subdirectory (libmemory) 
add_subdirectory (libmeasure_resource)
add_subdirectory (bin)
