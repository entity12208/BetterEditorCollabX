cmake_minimum_required(VERSION 3.10)

project(BetterEditorCollab VERSION 1.0)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED True)

add_library(BetterEditorCollab SHARED EditorCollab.cpp BetterEditorCollab.cpp)

find_package(Boost REQUIRED)
find_package(WebSocketPP REQUIRED)
find_package(JsonCpp REQUIRED)
find_package(WebRTC REQUIRED)

include_directories(${Boost_INCLUDE_DIRS} ${WebSocketPP_INCLUDE_DIRS} ${JsonCpp_INCLUDE_DIRS} ${WebRTC_INCLUDE_DIRS})

target_link_libraries(BetterEditorCollab PRIVATE ${Boost_LIBRARIES} ${WebSocketPP_LIBRARIES} ${JsonCpp_LIBRARIES} ${WebRTC_LIBRARIES})

install(TARGETS BetterEditorCollab DESTINATION bin)
