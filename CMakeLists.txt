cmake_minimum_required(VERSION 3.26.4)

set(PROJECT_NAME iu5_internship)
project(${PROJECT_NAME})
set(CMAKE_CXX_STANDARD 23)

add_subdirectory(server)
add_subdirectory(client)
