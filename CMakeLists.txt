cmake_minimum_required(VERSION 3.30)
project(Server)

set(CMAKE_CXX_STANDARD 20)

# GoogleTest 패키지 찾기 (Homebrew 설치 경로 명시)
set(CMAKE_PREFIX_PATH "/opt/homebrew/Cellar/googletest/1.16.0/lib/cmake")
find_package(GTest REQUIRED)

# 기본 실행 파일 추가
add_executable(Server
        main.cpp
        GameServer.cpp
        GameServer.h
        Player.cpp
        Player.h
        GameServerTest.cpp)

# GoogleTest와 pthread를 링크
target_link_libraries(Server PRIVATE GTest::gtest GTest::gtest_main pthread)

# GameServerTest 실행 파일 추가
add_executable(GameServerTest GameServerTest.cpp)

# GoogleTest와 pthread를 링크
target_link_libraries(GameServerTest PRIVATE GTest::gtest GTest::gtest_main pthread)