cmake_minimum_required(VERSION 3.21)

project(SubtitleStudio LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

find_package(Qt6 REQUIRED COMPONENTS Widgets)

qt_standard_project_setup()

qt_add_executable(SubtitleStudio
    src/Main.cpp

    src/app/MainWindow.cpp
    src/app/MainWindow.h
)

target_link_libraries(SubtitleStudio PRIVATE
    Qt6::Widgets
)

find_program(WINDEPLOYQT_EXECUTABLE
    NAMES windeployqt
    HINTS "${CMAKE_PREFIX_PATH}/bin"
)

if (WIN32)
    find_program(WINDEPLOYQT_EXECUTABLE
        NAMES windeployqt
        HINTS "${QT_HOST_PATH}/bin"
        REQUIRED
    )

    add_custom_command(TARGET SubtitleStudio POST_BUILD
        COMMAND "${WINDEPLOYQT_EXECUTABLE}"
                --no-translations
                "$<TARGET_FILE:SubtitleStudio>"
        COMMENT "Deploying Qt runtime..."
    )
endif()