# lib3000fps

find_package(OpenCV REQUIRED)
find_package(OpenMP REQUIRED)

if(OPENMP_FOUND)
    message("OPENMP FOUND")
    set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} ${OpenMP_C_FLAGS}")
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} ${OpenMP_C_FLAGS}")
    set(CMAKE_EXE_LINKER_FLAGS "${CMAKE_EXE_LINKER_FLAGS} ${OpenMP_EXE_LINKER_FLAGS}")
endif()

include_directories(${CMAKE_CURRENT_LIST_DIR}/3rdparty)
include_directories(${CMAKE_CURRENT_LIST_DIR}/include)

include(${CMAKE_CURRENT_LIST_DIR}/3rdparty/liblinear/liblinear.cmake)

file(GLOB SRC ${CMAKE_CURRENT_LIST_DIR}/src/lbf/*.cpp)

add_library(3000fps STATIC ${SRC})
target_link_libraries(3000fps linear ${OpenCV_LIBS})

SET(PUBLIC_HEADER ${CMAKE_CURRENT_LIST_DIR}/src/lbf/public.hpp)
set_target_properties(3000fps PROPERTIES PUBLIC_HEADER ${PUBLIC_HEADER})
INSTALL(TARGETS 3000fps
        ARCHIVE DESTINATION "lib"
        LIBRARY DESTINATION "lib"
        PUBLIC_HEADER DESTINATION "include/lbf")
