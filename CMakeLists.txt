# 项目基本设置
cmake_minimum_required(VERSION 3.14)
project(ExecutorProject)
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# [关键设置] 强制 MSVC 使用动态运行时库 (/MDd)
if (MSVC)
  set(CMAKE_MSVC_RUNTIME_LIBRARY "MultiThreaded$<$<CONFIG:Debug>:Debug>DLL" CACHE STRING "MSVC runtime" FORCE)
endif()

# [关键修复] 强制 GTest 也使用共享运行时，解决 LNK2038 错误
set(gtest_force_shared_crt ON CACHE BOOL "" FORCE)

# --- 头文件路径 ---
include_directories(include)
include_directories(src)
include_directories(tests)

# --- GTest ---
add_subdirectory(third_party/googletest-1.14.0)

# --- 核心库 ---
add_library(executor_lib
  src/ExecutorImpl.cpp
  src/PoseHandler.cpp
  src/ActionGroup.cpp
  # [注意] src/CmderFactory.cpp 已被移除，因为它是模板类，实现都在 .hpp 中
)

# --- 主程序 ---
add_executable(main
  src/main.cc
)
target_link_libraries(main PRIVATE executor_lib)

# --- 测试程序 ---
add_executable(run_tests
  tests/ExecutorTest.cpp
  tests/ExecutorFastTest.cpp
  tests/ExecutorReverseTest.cpp
  tests/ExecutorTurnRoundTest.cpp
  tests/ExecutorVehicleTest.cpp   # [新] 添加车辆类型测试
  tests/PoseEq.cpp
)

target_link_libraries(run_tests
  PRIVATE
  executor_lib
  GTest::gtest_main
)