cmake_minimum_required(VERSION 3.10)


set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

project(MyWxApp)

# กำหนดไดเรกทอรีที่เก็บไฟล์ .cpp และ .hpp
include_directories(./Utils /opt/homebrew/include)

# กำหนดไลบรารีที่ต้องการ
link_directories(/opt/homebrew/lib)

# ค้นหา wxWidgets
find_package(wxWidgets REQUIRED COMPONENTS core base)

# เปิดใช้ wxWidgets
include(${wxWidgets_USE_FILE})

# เพิ่มไฟล์ต้นฉบับ
add_executable(MyWxApp src/main.cpp Utils/modbus_utils.cpp Utils/serial_utils.cpp )

# ลิงก์ wxWidgets
target_link_libraries(MyWxApp ${wxWidgets_LIBRARIES} boost_system modbus)


# คัดลอกไฟล์จากไดเรกทอรี media ไปยังโฟลเดอร์ build
file(COPY ./media/ DESTINATION ${CMAKE_BINARY_DIR}/media)