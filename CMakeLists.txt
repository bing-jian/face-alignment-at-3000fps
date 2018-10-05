project(FaceAlignment)
cmake_minimum_required(VERSION 2.8)

set(CMAKE_POSITION_INDEPENDENT_CODE ON)

find_package(OpenCV REQUIRED)

include(lib3000fps.cmake)

set(SRC src/main.cpp
        src/prepare.cpp
        src/test.cpp
        src/train.cpp)

add_executable(${PROJECT_NAME} ${SRC})
target_link_libraries(${PROJECT_NAME} 3000fps ${OpenCV_LIBS})
