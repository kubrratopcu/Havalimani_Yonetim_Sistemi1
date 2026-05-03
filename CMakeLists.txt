cmake_minimum_required(VERSION 3.20)
project(Havalimani_Yonetim_Sistemi1)

set(CMAKE_CXX_STANDARD 20)

add_executable(Havalimani_Yonetim_Sistemi1
        main.cpp
        Modeller.h
        KuleYonetimi.cpp
        YolcuSistemi.cpp
        KargoSistemi.cpp
)