cmake_minimum_required(VERSION 3.12)
project(GraphicsView)

set(CMAKE_CXX_STANDARD 20)

set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)
set(CMAKE_AUTOUIC ON)

if(CMAKE_VERSION VERSION_LESS "3.7.0")
set(CMAKE_INCLUDE_CURRENT_DIR ON)
endif()

set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_SOURCE_DIR}/bin)

# 请在环境变量中提供QT路径，如"D:/Qt/Qt5.14.2/5.14.2/msvc2017_64/bin"
find_package(Qt5 COMPONENTS Widgets Core OpenGL REQUIRED)

# boost
set(USING_BOOST_BOOLEAN ON)

if(USING_BOOST_BOOLEAN)
set(Boost_INCLUDE_DIR "../boost_1_79_0")
set(Boost_LIBRARY_DIRS "../boost_1_79_0/stage/lib")
find_package(Boost REQUIRED)
include_directories(${Boost_INCLUDE_DIR})
link_directories(${Boost_LIBRARY_DIRS})
set(Boost_USE_STATIC_LIBS        OFF)
set(Boost_USE_MULTITHREADED      ON)
set(Boost_USE_STATIC_RUNTIME     OFF)
set(BOOST_ALL_DYN_LINK           ON)
add_compile_definitions(USING_BOOST_BOOLEAN)
endif()

# 工程文件
file(GLOB SRCS
       ${PROJECT_SOURCE_DIR}/*.cc
       ${PROJECT_SOURCE_DIR}/control/*.cc
       ${PROJECT_SOURCE_DIR}/data/*.cc
       ${PROJECT_SOURCE_DIR}/graphics/*.cc
       ${PROJECT_SOURCE_DIR}/utility/*.cc
       ${PROJECT_SOURCE_DIR}/view/*.cc)

add_executable(${PROJECT_NAME} ${SRCS})

target_include_directories(${PROJECT_NAME} PRIVATE ./iguana)
target_link_libraries(${PROJECT_NAME} Qt5::Widgets Qt5::Core Qt5::OpenGL)

if(USING_BOOST_BOOLEAN)
target_link_libraries(${PROJECT_NAME} ${Boost_LIBRARIES})
endif()

#打印SRC中列表信息
message(STATUS "SRCS = ${SRCS}")
