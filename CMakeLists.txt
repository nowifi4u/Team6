cmake_minimum_required(VERSION 3.19)
project(Team6)

# Enable debug by default
if(NOT CMAKE_BUILD_TYPE)
    set(CMAKE_BUILD_TYPE Debug CACHE STRING "Choose the type of build (Debug or Release)" FORCE)
endif()

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -pthread")

# include all source code directory
include_directories(src)

set(JSON_BuildTests OFF CACHE INTERNAL "")
add_subdirectory(libs/json)

add_subdirectory(libs/SFML)
add_subdirectory(libs/spdlog)

# adding boost
include( ExternalProject )

set(boost_SOURCE_DIR ${PROJECT_SOURCE_DIR}/libs/boost)
set(boost_INSTALL ${CMAKE_CURRENT_BINARY_DIR}/libs/boost)
set(boost_INCLUDE_DIR ${boost_INSTALL}/include)
set(boost_LIB_DIR ${boost_INSTALL}/lib)

message(STATUS "Boost source dir: " ${boost_SOURCE_DIR})
message(STATUS "Boost install dir: " ${boost_INSTALL})

if(WIN32)
    set(boost_BOOTSTRAP ./bootstrap.bat)
    set(LIB_EXTENSION .lib)
    message(STATUS "Using WIN32 specific boost compilation")
elseif(UNIX)
    set(boost_BOOTSTRAP ./bootstrap.sh)
    set(LIB_EXTENSION .a)
    message(STATUS "Using UNIX specific boost compilation")
elseif(TRUE)
    message(FATAL_ERROR "At this moment, support only WIN32 & UNIX boost compilation")
endif()

ExternalProject_Add( boost
        PREFIX boost
        SOURCE_DIR ${boost_SOURCE_DIR}
        BUILD_IN_SOURCE 1
        CONFIGURE_COMMAND
        ${boost_BOOTSTRAP}
        --with-libraries=graph
        --with-libraries=thread
        --with-libraries=filesystem
        --prefix=<INSTALL_DIR>
        BUILD_COMMAND
        ./b2 install link=static variant=release threading=multi runtime-link=static
        INSTALL_COMMAND ""
        INSTALL_DIR ${boost_INSTALL} )

include_directories(${boost_INCLUDE_DIR})

set(Boost_LIBRARIES
        ${boost_LIB_DIR}/libboost_graph${LIB_EXTENSION}
        ${boost_LIB_DIR}/libboost_filesystem${LIB_EXTENSION}
        ${boost_LIB_DIR}/libboost_thread${LIB_EXTENSION} )

message( STATUS "Boost static libs: " ${Boost_LIBRARIES} )

add_executable(Team6
        src/game/data/event.h
        src/game/data/player.h
        src/game/data/post.h
        src/game/data/train.h
        src/game/solver/RouteManager.h
        src/game/solver/executor/executor.cpp
        src/game/data.h
        src/game/solver.h
        src/game/controller.h
        src/graph/CoordsCalculator.h
        src/graph/CoordsHolder.h
        src/graph/graph.h
        src/graph/KKSCoordsCalculator.h
        src/render/game_drawer.h
        src/render/ResourceManager.h
        src/render/SpriteUtils.h
        src/render/TextureManager.h
        src/utils/network/server_connector.h
        src/utils/network/tcp_connector.h
        src/utils/bincharstream.h
        src/utils/binstream.h
        src/utils/ClassDefines.h
        src/game.h
        src/Main.cpp
        src/Types.h )

add_dependencies(Team6 boost)
target_link_libraries(Team6 sfml-graphics nlohmann_json spdlog::spdlog ${Boost_LIBRARIES} -ldl)
