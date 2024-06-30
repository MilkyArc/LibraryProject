cmake_minimum_required(VERSION 3.22)
project(OOP_Library_Project)

set(CMAKE_CXX_STANDARD 14)

add_executable(OOP_Library_Project main.cpp Admin.cpp Article.cpp Book.cpp Date.cpp PeriodicRelease.cpp Reader.cpp User.cpp crypto/sha256.cpp Functionality.cpp Functionality.h)
