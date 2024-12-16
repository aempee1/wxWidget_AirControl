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
add_executable(MyWxApp main.cpp src/modbus_utils.cpp src/serial_utils.cpp src/comport_setting.cpp src/mainwindow_properties.cpp src/manual_calibrate.cpp )

# ลิงก์ wxWidgets
target_link_libraries(MyWxApp ${wxWidgets_LIBRARIES} boost_system modbus)

# คัดลอกไฟล์จากไดเรกทอรี media ไปยังโฟลเดอร์ build
file(COPY ./media/ DESTINATION ${CMAKE_BINARY_DIR}/media)

# กำหนดให้ CMake สร้าง .app สำหรับ macOS
set(MY_APP_BUNDLE_NAME "MyWxApp.app")

# ตั้งค่าต่างๆ สำหรับ macOS Bundle
set(MACOSX_BUNDLE_INFO_STRING "My wxWidgets Application")
set(MACOSX_BUNDLE_GUI_IDENTIFIER com.mycompany.mywxapp)
set(MACOSX_BUNDLE_BUNDLE_VERSION 1.0.0)
set(MACOSX_BUNDLE_ICON_FILE "MyWxApp.icns")

# เพิ่มการตั้งค่าที่เกี่ยวข้องกับ Bundle
set_target_properties(MyWxApp PROPERTIES
    MACOSX_BUNDLE TRUE
    MACOSX_BUNDLE_GUI_IDENTIFIER ${MACOSX_BUNDLE_GUI_IDENTIFIER}
    MACOSX_BUNDLE_INFO_STRING ${MACOSX_BUNDLE_INFO_STRING}
    MACOSX_BUNDLE_BUNDLE_VERSION ${MACOSX_BUNDLE_BUNDLE_VERSION}
    MACOSX_BUNDLE_ICON_FILE ${MACOSX_BUNDLE_ICON_FILE}
)

# กำหนดให้ CPack ใช้ในการสร้างไฟล์ติดตั้ง
include(CPack)

set(CPACK_GENERATOR "DragNDrop")  # หรือ "DMG"
set(CPACK_PACKAGE_VERSION "1.0.0")
set(CPACK_PACKAGE_FILE_NAME "MyWxApp-${CPACK_PACKAGE_VERSION}")
set(CPACK_RESOURCE_FILE_LICENSE "${CMAKE_CURRENT_SOURCE_DIR}/LICENSE")  # ถ้ามีไฟล์ LICENSE

# กำหนดไอคอนสำหรับไฟล์ติดตั้ง
set(CPACK_PACKAGE_ICON "${CMAKE_CURRENT_SOURCE_DIR}/media/appIcons.icns")

# เพิ่มข้อมูลติดต่อสำหรับแพ็คเกจ
set(CPACK_PACKAGE_CONTACT "your-email@example.com")



