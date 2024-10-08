cmake_minimum_required(VERSION 3.10)

project(JimsCommon)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED True)
find_package(jsoncpp REQUIRED)

# Find Boost libs.


SET(Boost_USE_STATIC_LIBS ON)
FIND_PACKAGE( Boost 1.74.0 COMPONENTS program_options REQUIRED )
FIND_PACKAGE( Boost 1.74.0 COMPONENTS thread log log_setup REQUIRED )
find_package(Threads)
INCLUDE_DIRECTORIES( ${Boost_INCLUDE_DIR} )
#file(GLOB sources ./source/connection.cpp)
#add_library(JimsCommon ${sources}) # STATIC by default
#target_include_directories(JimsCommon PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})
#target_link_libraries(JimsCommon jsoncpp)
INCLUDE_DIRECTORIES( ./include )

add_executable(sinMqttTest
../JimsCommon/source/logger.cpp
../JimsCommon/source/loggerSingleton.cpp
./source/randomSingleton.cpp
./source/connection.cpp
./source/mqtt.cpp
./source/mqttMessage.cpp
./test/mqttSinTest.cpp
)
target_link_libraries(sinMqttTest jsoncpp paho-mqtt3a ${Boost_LIBRARIES})

add_executable(mqttSubSinQueueTest
../JimsCommon/source/logger.cpp
../JimsCommon/source/loggerSingleton.cpp
./source/randomSingleton.cpp
./source/connection.cpp
./source/mqtt.cpp
./source/mqttMessage.cpp
./test/mqttSubSinQueueTest.cpp
)
target_link_libraries(mqttSubSinQueueTest jsoncpp paho-mqtt3a ${Boost_LIBRARIES})

add_executable(mqttSubSinFuncTest
../JimsCommon/source/logger.cpp
../JimsCommon/source/loggerSingleton.cpp
./source/randomSingleton.cpp
./source/connection.cpp
./source/mqtt.cpp
./source/mqttMessage.cpp
./test/mqttSubSinFuncTest.cpp
)
target_link_libraries(mqttSubSinFuncTest jsoncpp paho-mqtt3a ${Boost_LIBRARIES})

add_executable(sinMqttSingletonTest
../JimsCommon/source/logger.cpp
../JimsCommon/source/loggerSingleton.cpp
./source/randomSingleton.cpp
./source/connection.cpp
./source/mqtt.cpp
./source/mqttMessage.cpp
./source/mqttSingleton.cpp
./test/mqttSingletonTest.cpp
)
target_link_libraries(sinMqttSingletonTest jsoncpp paho-mqtt3a ${Boost_LIBRARIES})

add_executable(mySqlTest
../JimsCommon/source/logger.cpp
../JimsCommon/source/loggerSingleton.cpp
./source/randomSingleton.cpp
./source/connection.cpp
./source/mySqlDB.cpp
./test/mySqlTest.cpp
)
target_link_libraries(mySqlTest jsoncpp ${Boost_LIBRARIES} mysqlcppconn)

add_executable(mySqlSingletonTest
../JimsCommon/source/logger.cpp
../JimsCommon/source/loggerSingleton.cpp
./source/randomSingleton.cpp
./source/connection.cpp
./source/mySqlDB.cpp
./source/mySqlDBSingleton.cpp
./test/mySqlTest.cpp
)
target_link_libraries(mySqlSingletonTest jsoncpp ${Boost_LIBRARIES} mysqlcppconn)

add_executable(propertyTest
../JimsCommon/source/logger.cpp
../JimsCommon/source/loggerSingleton.cpp
../JimsCommon/source/propertyFile.cpp
./test/propertyTest.cpp
)
target_link_libraries(propertyTest ${Boost_LIBRARIES})

add_executable(randomTest
../JimsCommon/source/logger.cpp
../JimsCommon/source/loggerSingleton.cpp
./source/randomSingleton.cpp
./test/randomTest.cpp
)
target_link_libraries(randomTest ${Boost_LIBRARIES})

add_executable(stopWatchTest
../JimsCommon/source/logger.cpp
../JimsCommon/source/loggerSingleton.cpp
./source/stopWatch.cpp
./test/stopWatchTest.cpp
)
target_link_libraries(stopWatchTest ${Boost_LIBRARIES})

add_executable(webSocketSimple
../JimsCommon/source/logger.cpp
../JimsCommon/source/loggerSingleton.cpp
../JimsCommon/source/randomSingleton.cpp
./test/webSocketSimple.cpp
)
target_link_libraries(webSocketSimple jsoncpp paho-mqtt3a ${Boost_LIBRARIES})

add_executable(logTestBoost
./test/logTestBoost.cpp
)
target_link_libraries(logTestBoost ${Boost_LIBRARIES})

add_executable(logTest
../JimsCommon/source/logger.cpp
./test/logTest.cpp
)
target_link_libraries(logTest ${Boost_LIBRARIES})

add_executable(logTestSingleton
../JimsCommon/source/logger.cpp
../JimsCommon/source/loggerSingleton.cpp
./test/logTestSingleton.cpp
)
target_link_libraries(logTestSingleton ${Boost_LIBRARIES})

add_executable(mqttPubTest
./source/logger.cpp
./source/loggerSingleton.cpp
./source/stopWatch.cpp
./source/randomSingleton.cpp
./source/connection.cpp
./source/mqtt.cpp
./source/mqttMessage.cpp
./test/mqttPubTest.cpp
)
target_link_libraries(mqttPubTest jsoncpp paho-mqtt3a ${Boost_LIBRARIES})

add_executable(mqttPubAutoTest
./source/logger.cpp
./source/loggerSingleton.cpp
./source/stopWatch.cpp
./source/randomSingleton.cpp
./source/connection.cpp
./source/mqtt.cpp
./source/mqttMessage.cpp
./test/mqttPubAutoTest.cpp
)
target_link_libraries(mqttPubAutoTest jsoncpp paho-mqtt3a ${Boost_LIBRARIES})

add_executable(mqttSubTest
./source/logger.cpp
./source/loggerSingleton.cpp
./source/stopWatch.cpp
./source/randomSingleton.cpp
./source/connection.cpp
./source/mqtt.cpp
./source/mqttMessage.cpp
./test/mqttSubTest.cpp
)
target_link_libraries(mqttSubTest jsoncpp paho-mqtt3a ${Boost_LIBRARIES})

