cmake_minimum_required(VERSION 3.7)
project(ToyDBMS)

set(CMAKE_CXX_STANDARD 11)
#for better times
#set(CMAKE_CXX_FLAGS "-Wall -Werror")

set(SOURCE_FILES
        dbmse/engine/demo/demo.cpp
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
        )

add_executable(ToyDBMS ${SOURCE_FILES})