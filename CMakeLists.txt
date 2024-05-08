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

add_executable(sinMqttTest
./source/connection.cpp
./source/mqtt.cpp
./source/mqttMessage.cpp
./test/mqttSinTest.cpp
)
target_link_libraries(sinMqttTest jsoncpp paho-mqtt3a ${Boost_LIBRARIES})

add_executable(sinMqttSingletonTest
./source/connection.cpp
./source/mqtt.cpp
./source/mqttMessage.cpp
./source/mqttSingleton.cpp
./test/mqttSingletonTest.cpp
)
target_link_libraries(sinMqttSingletonTest jsoncpp paho-mqtt3a ${Boost_LIBRARIES})

add_executable(mySqlTest
./source/connection.cpp
./source/mySqlDB.cpp
./test/mySqlTest.cpp
)
target_link_libraries(mySqlTest jsoncpp ${Boost_LIBRARIES} mysqlcppconn)

add_executable(mySqlSingletonTest
./source/connection.cpp
./source/mySqlDB.cpp
./source/mySqlDBSingleton.cpp
./test/mySqlTest.cpp
)
target_link_libraries(mySqlSingletonTest jsoncpp ${Boost_LIBRARIES} mysqlcppconn)

add_executable(randomTest
./source/randomSingleton.cpp
./test/randomTest.cpp
)

add_executable(stopWatchTest
./source/stopWatch.cpp
./test/stopWatchTest.cpp
)

