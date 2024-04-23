cmake_minimum_required(VERSION 3.21)
project(untitled1)

set(CMAKE_CXX_STANDARD 17)
set(EasyX_Dir D:/easyxLibrary)#此句要放到add_executable前
add_executable(untitled1 main.cpp header.h)
#文件包路径，例如C:/Users/Simon/Desktop/easyX
set(CMAKE_EXE_LINKER_FLAGS -static)
#下面三句要放到add_executable后
target_include_directories(${PROJECT_NAME} PRIVATE  ${EasyX_Dir})
target_link_libraries(${PROJECT_NAME} ${EasyX_Dir}/libEasyX.a)
target_link_directories(${PROJECT_NAME} PRIVATE ${EasyX_Dir})
