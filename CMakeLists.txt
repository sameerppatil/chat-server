cmake_minimum_required(VERSION 3.6)

project(chat-app)




set (CMAKE_CXX_STANDARD 17)
# include_directories(../libdatachannel/include)

# add_library(libdatachannel SHARED IMPORTED)

# set_property(TARGET libdatachannel PROPERTY IMPORTED_LOCATION /home/sameerwsl/garage/remote/garage/webrtc/echo_server/libdatachannel.so)
find_package(Curses REQUIRED)
include_directories(${CURSES_INCLUDE_DIR})
include_directories(${PROJECT_SOURCE_DIR}/dependencies/plog/include)

add_executable(chat-service
            src/main.cpp
            src/server.cpp
            src/client.cpp
            src/interface.cpp
            src/ncurses.cpp)
target_link_libraries(chat-service ${CURSES_LIBRARIES})