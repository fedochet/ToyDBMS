cmake_minimum_required(VERSION 3.7)
project(ToyDBMS)

set(CMAKE_CXX_STANDARD 11)
#set(CMAKE_CXX_FLAGS "-Wall -Werror")

set(SOURCE_FILES
        dbmse/engine/demo/demo.cpp
        dbmse/engine/demo/pgetnextnode.cpp
        dbmse/engine/demo/pgetnextnode.h
        dbmse/engine/demo/pjoinnode.cpp
        dbmse/engine/demo/pjoinnode.h
        dbmse/engine/demo/pselectnode.cpp
        dbmse/engine/demo/pselectnode.h
        dbmse/engine/demo/pcrossproductnode.cpp
        dbmse/engine/demo/pcrossproductnode.h
        dbmse/engine/demo/pprojectnode.cpp
        dbmse/engine/demo/pprojectnode.h
        dbmse/engine/interface/basics.h
        dbmse/engine/interface/interface.cpp
        dbmse/engine/interface/interface.h
        dbmse/engine/interface/lprojectnode.cpp
        )

add_executable(ToyDBMS ${SOURCE_FILES})