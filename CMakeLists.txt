cmake_minimum_required(VERSION 3.10)


set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

project(MyWxApp)

# กำหนดไดเรกทอรีที่เก็บไฟล์ .cpp และ .hpp
include_directories( /opt/homebrew/include ./include)

# กำหนดไลบรารีที่ต้องการ
link_directories(/opt/homebrew/lib)

# ค้นหา wxWidgets
find_package(wxWidgets REQUIRED COMPONENTS core base)

# เปิดใช้ wxWidgets
include(${wxWidgets_USE_FILE})

# เพิ่มไฟล์ต้นฉบับ
add_executable(MyWxApp main.cpp src/modbus_utils.cpp src/serial_utils.cpp src/comport_setting.cpp src/mainwindow_properties.cpp)

# ลิงก์ wxWidgets
target_link_libraries(MyWxApp ${wxWidgets_LIBRARIES} boost_system modbus)


# คัดลอกไฟล์จากไดเรกทอรี media ไปยังโฟลเดอร์ build
file(COPY ./media/ DESTINATION ${CMAKE_BINARY_DIR}/media)