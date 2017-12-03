cmake_minimum_required(VERSION 3.7)
project(ToyDBMS)

set(CMAKE_CXX_STANDARD 14)
#for better times
#set(CMAKE_CXX_FLAGS "-Wall -Werror")


set(SOURCE_FILES
        dbmse/engine/demo/pgetnextnode.cpp
        dbmse/engine/demo/pgetnextnode.h
        dbmse/engine/demo/join/pjoinnode.cpp
        dbmse/engine/demo/join/pjoinnode.h
        dbmse/engine/demo/select/pselectnode.cpp
        dbmse/engine/demo/select/pselectnode.h
        dbmse/engine/demo/cross_product/pcrossproductnode.cpp
        dbmse/engine/demo/cross_product/pcrossproductnode.h
        dbmse/engine/demo/projection/pprojectnode.cpp
        dbmse/engine/demo/projection/pprojectnode.h
        dbmse/engine/interface/basics.h
        dbmse/engine/interface/interface.cpp
        dbmse/engine/interface/interface.h
        dbmse/engine/interface/lprojectnode.cpp
        dbmse/engine/utils/bd_utils.h
        dbmse/engine/utils/utils.h
        dbmse/engine/demo/query_api/query_api.h

        )

add_library(ToyDB ${SOURCE_FILES})

add_executable(demo dbmse/engine/demo/demo.cpp)
target_link_libraries(demo ToyDB)

add_executable(my_demo dbmse/engine/advanced_demo/advanced_demo.cpp)
target_link_libraries(my_demo ToyDB)