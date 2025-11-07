#项目基本设置
cmake_minimum_required(VERSION 3.14)
project(ExecutorProject)
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# [修复 LNK2038]
# 强制所有目标 (包括 GTest) 都使用 动态/DLL 运行时库
# [清理] 这是解决该问题的唯一且正确的部分
if (MSVC)
  set(CMAKE_MSVC_RUNTIME_LIBRARY "MultiThreaded$<$<CONFIG:Debug>:Debug>DLL"
      CACHE STRING "MSVC runtime" FORCE)
endif()

#从本地 "third_party" 文件夹加载 GTest
add_subdirectory(third_party/googletest-1.14.0)


#编译
add_library(executor_lib
  executor.cc
)
target_include_directories(executor_lib
  PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}
)

add_executable(main
  main.cc
)
target_link_libraries(main PRIVATE executor_lib)


# [新] 将 pose_test_utils.cc 添加到 run_tests 目标
add_executable(run_tests
  executor_test.cc
  pose_test_utils.cc 
  executor_fast_test.cc  # [新] 添加这个新文件
)


#把 GTest和你的核心代码链接到测试程序上
target_link_libraries(run_tests
  PRIVATE
  executor_lib      
  GTest::gtest_main 
)