cmake_minimum_required(VERSION 3.17)
project(ProjetRF)

set(CMAKE_CXX_STANDARD 14)

add_executable(ProjetRF main.cpp Data/Data.cpp Data/Data.h Data/Core.cpp Data/Core.h Data/Knn.cpp Data/Knn.h Data/Kmeans.cpp Data/Kmeans.h Data/Tester.cpp Data/Tester.h Data/KmeansEM.cpp Data/KmeansEM.h)
