cmake_minimum_required(VERSION 3.10)

project(OS2Chat)

set(CMAKE_CXX_STANDARD 11)

add_executable(ServerChat main.cpp chat_log.cpp)
add_executable(ChatClient client.cpp)
