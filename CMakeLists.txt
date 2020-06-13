cmake_minimum_required(VERSION 3.15)
project(OdooCpp)

set(CMAKE_CXX_STANDARD 11)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wconversion -Wall -Wextra -pedantic -std=c++11")


file(MAKE_DIRECTORY builds)
# https://cmake.org/cmake/help/latest/variable/CMAKE_SOURCE_DIR.html#variable:CMAKE_SOURCE_DIR
set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY ${CMAKE_SOURCE_DIR}/lib)
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${CMAKE_SOURCE_DIR}/lib)
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_SOURCE_DIR}/bin)


if(GPROF)
  SET(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -pg")
  SET(CMAKE_EXE_LINKER_FLAGS "${CMAKE_EXE_LINKER_FLAGS} -pg")
  SET(CMAKE_SHARED_LINKER_FLAGS "${CMAKE_SHARED_LINKER_FLAGS} -pg")
  unset(GPROF CACHE)
endif()

add_subdirectory(external/OdooRPC)

ADD_LIBRARY(odooclient
  src/client.h src/client.cpp
  src/model.h src/model.cpp
  src/exception.h src/exception.cpp
  src/json.h
)
target_link_libraries(odooclient odoorpc)


add_executable(
    odoo
    main.cpp
)
target_link_libraries(odoo odooclient)

add_executable(
    test_model
    tests/test_model.cpp
)
target_link_libraries(test_model odooclient)