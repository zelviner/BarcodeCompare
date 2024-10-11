# 寻找Qt5库
find_package(Qt5 COMPONENTS Core Gui REQUIRED)

# 查找源文件
file(GLOB_RECURSE SOURCES ${CMAKE_CURRENT_LIST_DIR}/*.cpp ${CMAKE_CURRENT_LIST_DIR}/*.hpp)

# 查找头文件
file(GLOB_RECURSE HEADERS ${CMAKE_CURRENT_LIST_DIR}/*.h)

# 编译为静态库
add_library(Data STATIC  ${SOURCES} ${RESOURCES})
# 为target添加头文件
target_include_directories(Data
PUBLIC
    ${CMAKE_CURRENT_LIST_DIR}
)
# 为target添加需要链接的共享库
TARGET_LINK_LIBRARIES(Data
PRIVATE
    Qt5::Core
    Qt5::Gui
)
