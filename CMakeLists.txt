cmake_minimum_required(VERSION 3.20)

project(Havalimani_Yonetim_Sistemi1)

set(CMAKE_CXX_STANDARD 17)

set(CMAKE_PREFIX_PATH "C:/Qt/6.11.0/mingw_64")

find_package(Qt6 REQUIRED COMPONENTS Widgets)

qt_standard_project_setup()

add_executable(Havalimani_Yonetim_Sistemi1
        main.cpp


        HavayoluSistemi.cpp
        HavayoluSistemi.h

        KargoSistemi.cpp
        KuleYonetimi.cpp
        YolcuSistemi.cpp

        mainwindow.ui
        mainwindow.cpp
        mainwindow.h
)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTOUIC ON)

target_link_libraries(Havalimani_Yonetim_Sistemi1
        Qt6::Widgets
)

