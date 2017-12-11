cmake_minimum_required(VERSION 3.7)
project(ToyDBMS)

set(CMAKE_CXX_STANDARD 17)
#for better times
set(CMAKE_CXX_FLAGS "-Wall -Werror -Wextra -Winit-self -Wold-style-cast -Woverloaded-virtual -Wuninitialized -Winit-self -pedantic-errors")


set(SOURCE_FILES
        dbmse/engine/demo/pgetnextnode.cpp
        dbmse/engine/demo/pgetnextnode.h
        dbmse/engine/demo/joins/pnestedloopjoinnode.cpp
        dbmse/engine/demo/joins/pnestedloopjoinnode.h
        dbmse/engine/demo/select/pselectnode.cpp
        dbmse/engine/demo/select/pselectnode.h
        dbmse/engine/demo/cross_product/pcrossproductnode.cpp
        dbmse/engine/demo/cross_product/pcrossproductnode.h
        dbmse/engine/demo/projection/pprojectnode.cpp
        dbmse/engine/demo/projection/pprojectnode.h
        dbmse/engine/demo/query_api/query_api.h
        dbmse/engine/utils/bd_utils.h
        dbmse/engine/utils/utils.h
        dbmse/engine/interface/basics.h
        dbmse/engine/interface/interface.cpp
        dbmse/engine/interface/interface.h
        dbmse/engine/interface/projection/lprojectnode.h
        dbmse/engine/interface/projection/lprojectnode.cpp
        dbmse/engine/interface/select/lselectnode.cpp
        dbmse/engine/interface/select/lselectnode.h
        dbmse/engine/interface/joins/lnestedloopjoinnode.cpp
        dbmse/engine/interface/joins/lnestedloopjoinnode.h
        dbmse/engine/interface/crossproduct/lcrossproductnode.cpp
        dbmse/engine/interface/crossproduct/lcrossproductnode.h
        dbmse/engine/interface/joins/lsortmergejoin.cpp
        dbmse/engine/interface/joins/lsortmergejoin.h
        dbmse/engine/utils/bd_utils.cpp
        dbmse/engine/demo/joins/psortmergejoinnode.cpp
        dbmse/engine/demo/joins/psortmergejoinnode.h
        dbmse/engine/demo/joins/joinutils.h
        dbmse/engine/interface/joins/labstractjoinnode.cpp
        dbmse/engine/interface/joins/labstractjoinnode.h
        dbmse/engine/interface/joins/lhashjoinnode.cpp
        dbmse/engine/interface/joins/lhashjoinnode.h
        dbmse/engine/demo/joins/phashjoinnode.cpp
        dbmse/engine/demo/joins/phashjoinnode.h)

add_library(ToyDB ${SOURCE_FILES})

add_executable(demo dbmse/engine/demo/demo.cpp)
target_link_libraries(demo ToyDB)

add_executable(my_demo dbmse/engine/advanced_demo/advanced_demo.cpp)
target_link_libraries(my_demo ToyDB)