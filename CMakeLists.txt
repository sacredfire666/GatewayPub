cmake_minimum_required (VERSION 2.8)
 
project (GatewayPub)
add_definitions(-std=c++11)

SET(SRC_LIST main.cpp Publisher.cpp MdSpi.cpp DataTool.cpp DataTool.h Proxy.cpp Proxy.h)
file(COPY config/login.json DESTINATION ${CMAKE_CURRENT_BINARY_DIR})
file(COPY config/instruments.json DESTINATION ${CMAKE_CURRENT_BINARY_DIR})

include_directories(./api)
link_directories(${CMAKE_CURRENT_SOURCE_DIR}/api)

# Find required protobuf package
find_package(Protobuf REQUIRED)
if(PROTOBUF_FOUND)
   message(STATUS "protobuf library found")
else()
    message(FATAL_ERROR "protobuf library is needed but cant be found")
endif()
include_directories(${PROTOBUF_INCLUDE_DIRS})
include_directories(${CMAKE_CURRENT_BINARY_DIR})
PROTOBUF_GENERATE_CPP(MARKETPROTO_SRCS MARKETPROTO_HDRS protobuf/MarketSnapshot.proto)

find_package(PkgConfig REQUIRED)
pkg_check_modules(PC_ZeroMQ libzmq)
include_directories(${PC_ZeroMQ_INCLUDE_DIRS})
message(${PC_ZeroMQ_INCLUDE_DIRS})
link_directories(${PC_ZeroMQ_LIBRARY_DIRS})

ADD_EXECUTABLE(main ${SRC_LIST} ${MARKETPROTO_HDRS} ${MARKETPROTO_SRCS})
ADD_EXECUTABLE(client client.cpp Proxy.cpp ${MARKETPROTO_HDRS} ${MARKETPROTO_SRCS} Subscriber.cpp Subscriber.h)

target_link_libraries(main ${PROTOBUF_LIBRARIES} zmq thostmduserapi_se)
target_link_libraries(client ${PROTOBUF_LIBRARIES} zmq)


