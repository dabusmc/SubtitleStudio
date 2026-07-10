cmake_minimum_required(VERSION 3.21)

project(SubtitleStudio LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

find_package(Qt6 REQUIRED COMPONENTS
    Widgets
    Multimedia
    MultimediaWidgets
)

qt_standard_project_setup()

qt_add_executable(SubtitleStudio
    src/Main.cpp

    src/app/Application.cpp

    src/util/Timecode.cpp

    src/subtitle/Subtitle.cpp
    src/subtitle/SubtitleTrack.cpp
    src/subtitle/SRTParser.cpp

    src/video/VideoPlayer.cpp

    src/ui/MainWindow.cpp

    src/ui/widgets/PlaybackControlsWidget.cpp
    src/ui/widgets/TimelineWidget.cpp
    src/ui/widgets/VideoWidget.cpp
)

target_link_libraries(SubtitleStudio PRIVATE
    Qt6::Widgets
    Qt6::Multimedia
    Qt6::MultimediaWidgets
)

if (MSVC)
    target_compile_options(SubtitleStudio PRIVATE /MP)
endif()

target_include_directories(SubtitleStudio PRIVATE
    ${CMAKE_CURRENT_SOURCE_DIR}/src
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