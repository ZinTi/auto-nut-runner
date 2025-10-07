cmake_minimum_required(VERSION 3.16)

project(auto_nut_runner_ui VERSION 0.1 LANGUAGES CXX)

set(CMAKE_AUTOUIC ON)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

set(CMAKE_PREFIX_PATH
        # "A:/app/Qt/6.8.1/mingw_64"
        "A:/applib/qt-6.9.0-win64-mingw64-static"    # Qt 6.9.0 mingw64 static (for release)
)

find_package(QT NAMES Qt6 Qt5 REQUIRED COMPONENTS Widgets SerialPort)
find_package(Qt${QT_VERSION_MAJOR} REQUIRED COMPONENTS Widgets SerialPort)

include_directories(
        include
        include/common

)

set(PROJECT_SOURCES
        src/main.cpp
        src/main_window.cpp

        src/common/text_converter.cpp
        src/common/serial_combo_box.cpp
        src/common/hex_text_edit.cpp
        src/common/read_config.cpp

        src/serial_port_config.cpp
        src/encoding_config.cpp
        src/microstep_config.cpp
        src/general_setting.cpp
        src/detail_display.cpp
        src/batch_command.cpp
        src/stepper_ctl.cpp
        src/sound_light_alert.cpp
        src/dc_gear_motor.cpp
        src/fetch_config.cpp
        # 添加对应的头文件
        include/main_window.h

        include/common/text_converter.h
        include/common/serial_combo_box.h
        include/common/hex_text_edit.h
        include/common/read_config.h

        include/serial_port_config.h
        include/encoding_config.h
        include/microstep_config.h
        include/general_setting.h
        include/detail_display.h
        include/batch_command.h
        include/stepper_ctl.h
        include/sound_light_alert.h
        include/dc_gear_motor.h
        include/fetch_config.h
        # 资源文件
        res/auto_nut_runner.rc
)

if(${QT_VERSION_MAJOR} GREATER_EQUAL 6)
    add_executable(${PROJECT_NAME}
        # MANUAL_FINALIZATION
        ${PROJECT_SOURCES}

    )
else()
    if(ANDROID)
        add_library(${PROJECT_NAME} SHARED
            ${PROJECT_SOURCES}
        )
    else()
        add_executable(${PROJECT_NAME}
            ${PROJECT_SOURCES}
        )
    endif()
endif()

# Windows 特定设置
if(WIN32)
    # 设置 Windows 入口点
    set(CMAKE_EXE_LINKER_FLAGS "${CMAKE_EXE_LINKER_FLAGS} -mwindows")

    # 添加 Windows 库
    target_link_libraries(${PROJECT_NAME}
        PRIVATE
            kernel32
            user32
            shell32
            Qt${QT_VERSION_MAJOR}::Widgets
            Qt${QT_VERSION_MAJOR}::SerialPort
           # Qt${QT_VERSION_MAJOR}::TextCodec
    )
else()
    target_link_libraries(${PROJECT_NAME}
        PRIVATE
            Qt${QT_VERSION_MAJOR}::Widgets
            Qt${QT_VERSION_MAJOR}::SerialPort
           # Qt${QT_VERSION_MAJOR}::TextCodec
    )
endif()

if(${QT_VERSION} VERSION_LESS 6.1.0)
  set(BUNDLE_ID_OPTION MACOSX_BUNDLE_GUI_IDENTIFIER com.example.${PROJECT_NAME})
endif()
set_target_properties(${PROJECT_NAME} PROPERTIES
    ${BUNDLE_ID_OPTION}
    MACOSX_BUNDLE_BUNDLE_VERSION ${PROJECT_VERSION}
    MACOSX_BUNDLE_SHORT_VERSION_STRING ${PROJECT_VERSION_MAJOR}.${PROJECT_VERSION_MINOR}
    MACOSX_BUNDLE TRUE
    WIN32_EXECUTABLE TRUE
)

include(GNUInstallDirs)
install(TARGETS ${PROJECT_NAME}
    BUNDLE DESTINATION .
    LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}
    RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR}
)

if(QT_VERSION_MAJOR EQUAL 6)
    qt_finalize_executable(${PROJECT_NAME})
endif()
