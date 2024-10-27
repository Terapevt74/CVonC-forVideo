cmake_minimum_required(VERSION 3.10)
project(OpenCV_Project)

# Находим OpenCV
find_package(OpenCV REQUIRED)
include_directories(${OpenCV_INCLUDE_DIRS})

# Добавляем исполняемый файл и подключаем библиотеки
add_executable(OpenCV_Project main.cpp)
target_link_libraries(OpenCV_Project ${OpenCV_LIBS})
