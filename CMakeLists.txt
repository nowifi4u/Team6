cmake_minimum_required(VERSION 3.19)
project(Team6)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -pthread")

# include all source code directory
include_directories(src)

set(JSON_BuildTests OFF CACHE INTERNAL "")
add_subdirectory(libs/json)

add_subdirectory(libs/SFML)

# adding boost
include( ExternalProject )

set(boost_SOURCE_DIR ${PROJECT_SOURCE_DIR}/libs/boost)
set(boost_INSTALL ${CMAKE_CURRENT_BINARY_DIR}/libs/boost)
set(boost_INCLUDE_DIR ${boost_INSTALL}/include)
set(boost_LIB_DIR ${boost_INSTALL}/lib)

message(STATUS "Boost source dir: " ${boost_SOURCE_DIR})
message(STATUS "Boost install dir: " ${boost_INSTALL})

ExternalProject_Add( boost
        PREFIX boost
        SOURCE_DIR ${boost_SOURCE_DIR}
        BUILD_IN_SOURCE 1
        CONFIGURE_COMMAND
        ./bootstrap.sh
        --with-libraries=graph
        --with-libraries=thread
        --prefix=<INSTALL_DIR>
        BUILD_COMMAND
        ./b2 install link=static variant=release threading=multi runtime-link=static
        INSTALL_COMMAND ""
        INSTALL_DIR ${boost_INSTALL} )

set(Boost_LIBRARIES
        ${boost_LIB_DIR}/libboost_graph.a
        ${boost_LIB_DIR}/libboost_thread.a )

message( STATUS "Boost static libs: " ${Boost_LIBRARIES} )

add_executable(Team6
        src/game/data/event.h
        src/game/data/player.h
        src/game/data/post.h
        src/game/data/train.h
        src/game/data.h
        src/game/solver.h
        src/graph/CoordsCalculator.h
        src/graph/CoordsHolder.h
        src/graph/graph.h
        src/graph/KKSCoordsCalculator.h
        src/render/drawer.h
        src/utils/network/server_connector.h
        src/utils/network/tcp_connector.h
        src/utils/bincharstream.h
        src/utils/binstream.h
        src/utils/ClassDefines.h
        src/utils/Logging.h
        src/game.h
        src/Main.cpp
        src/Types.h)

target_link_libraries(Team6 sfml-graphics nlohmann_json ${Boost_LIBRARIES})
