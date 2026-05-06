cmake_minimum_required(VERSION 4.0)

project(TimerApp)

add_executable(TimerApp)

target_sources(TimerApp
  PRIVATE
    main.cpp
    timer.cpp
  
  PUBLIC
    FILE_SET HEADERS
    FILES
      timer.h
)
