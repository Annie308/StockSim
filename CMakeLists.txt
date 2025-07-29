cmake_minimum_required(VERSION 3.15)

# Tell CMake this is your project
project(StocksSim)

# Use C++17
set(CMAKE_CXX_STANDARD 17)

# Vcpkg toolchain
set(CMAKE_TOOLCHAIN_FILE "C:/Users/annie/Documents/CppProjects/StocksSim/vcpkg/scripts/buildsystems/vcpkg.cmake"
    CACHE STRING "Vcpkg toolchain file")

# Find CPR via vcpkg
find_package(cpr CONFIG REQUIRED)

# Your main file
add_executable(StocksSim src/main.cpp src/portfolio.cpp src/balance.cpp src/input.cpp src/utils.cpp src/stock_data.cpp)


# Tell CMake where to find headers
target_include_directories(StocksSim PRIVATE include)

# Link CPR
target_link_libraries(StocksSim PRIVATE cpr::cpr)

