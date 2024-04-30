cmake_minimum_required(VERSION 3.10)

project(JimsCommon)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED True)
find_package(jsoncpp REQUIRED)

file(GLOB sources ./source/connection.cpp)
add_library(JimsCommon ${sources}) # STATIC by default
target_include_directories(JimsCommon PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})
target_link_libraries(JimsCommon jsoncpp)
INCLUDE_DIRECTORIES( ./include )

add_executable(SinMQTTTest
./source/connection.cpp
./source/mqtt.cpp
./test/mqttSinTest.cpp
)
target_link_libraries(SinMQTTTest jsoncpp paho-mqtt3a ${Boost_LIBRARIES} mysqlcppconn)

add_executable(mySQLTest
./source/connection.cpp
./source/mySqlDatabase.cpp
./test/mySqlTest.cpp
)
target_link_libraries(mySQLTest jsoncpp paho-mqtt3a ${Boost_LIBRARIES} mysqlcppconn)

add_executable(randomTest
./source/randomSingleton.cpp
./test/randomTest.cpp
)

