
# 查找源文件
# file(GLOB_RECURSE MODULE1_SOURCES "module1/*.cpp")
# file(GLOB_RECURSE MODULE2_SOURCES "module2/*.cpp")
# -----------------------------------------------------------
# 手动列出源文件
# set(SOURCES
#     network.cpp
# )
# -----------------------------------------------------------

include_directories(/usr/include)
# link_directories(/usr/lib/x86_64-linux-gnu)
# find_package(MySQL REQUIRED)

# 只在当前目录查找源文件
file(GLOB SOURCES "*.cpp")


#添加库
add_library(database ${SOURCES})

target_link_libraries(database mysqlcppconn hiredis)
