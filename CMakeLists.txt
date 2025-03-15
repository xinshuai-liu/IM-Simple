#要求最低cnake的版本
cmake_minimum_required(VERSION 3.10)

#项目名称
project(IMServer VERSION 1.0.0)

set(CMAKE_BUILD_PARALLEL_LEVEL 4)
SET(CMAKE_BUILD_TYPE "Debug")  # 定义编译类型
SET(CMAKE_CXX_FLAGS_DEBUG "$ENV{CXXFLAGS} -O0 -Wall -g2 -ggdb") # 定义Debug编译参数
SET(CMAKE_CXX_FLAGS_RELEASE "$ENV{CXXFLAGS} -O3 -Wall") # 定义Release编译参数

#设置c++标准
set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

#添加子目录
add_subdirectory(common)
add_subdirectory(network)
add_subdirectory(database)
# add_subdirectory(src/data lib/data)
# add_subdirectory(src/logic lib/logic)
# add_subdirectory(src/network lib/network)

#添加可执行文件
add_executable(${PROJECT_NAME} main.cpp)

#链接子目录
target_link_libraries(${PROJECT_NAME} common network database)

# 使用MinGW编译
# cmake .. -G "MinGW Makefiles" -DCMAKE_MAKE_PROGRAM="D:/Qt/Tools/mingw1120_64/bin/mingw32-make.exe"
# cmake --build .


#-----------------------------------------------------------------使用第三方库

# cmake_minimum_required(VERSION 3.10)
# project(MyProject)

# # 添加 Boost
# find_package(Boost 1.70 REQUIRED)

# # 添加子目录   第三方库源代码
# add_subdirectory(third_party/SomeLibrary)

# # 使用 FetchContent 添加另一个库
# include(FetchContent)
# FetchContent_Declare(
#     another_library
#     GIT_REPOSITORY https://github.com/another/repo.git
#     GIT_TAG        main
# )
# FetchContent_MakeAvailable(another_library)

# # 创建可执行文件
# add_executable(MyProject main.cpp)

# # 链接库    库是预编译的（如 .lib、.a、.so 或 .dll 文件），可以直接指定路径
# target_link_libraries(MyProject 
#     Boost::Boost 
#     SomeLibrary 
#     another_library
#     "path/to/library.lib"    
# )
