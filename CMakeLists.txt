cmake_minimum_required(VERSION 3.10)


set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

project(MyWxApp)

# ค้นหา wxWidgets
find_package(wxWidgets REQUIRED COMPONENTS core base)

# เปิดใช้ wxWidgets
include(${wxWidgets_USE_FILE})

# เพิ่มไฟล์ต้นฉบับ
add_executable(MyWxApp src/main.cpp)

# ลิงก์ wxWidgets
target_link_libraries(MyWxApp ${wxWidgets_LIBRARIES})
