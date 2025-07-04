cmake_minimum_required(VERSION 3.16)
project(KTUImageProcessing LANGUAGES CXX)

# ——————————————————————————————————
# Compiler & Qt setup
# ——————————————————————————————————

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTOUIC ON)
set(CMAKE_AUTORCC OFF)

find_package(Qt6 REQUIRED COMPONENTS
    Widgets Core Gui Charts Qml Concurrent
)

# ——————————————————————————————————
# Source files
# ——————————————————————————————————

set(SRC_FILES
    src/main.cpp
    src/gui/MainWindow.cpp
    src/gui/MainWindow.h
    src/core/ImageProcessor.cpp
    src/core/ImageProcessor.h
    src/core/QMat.h
)

file(GLOB CORE_FILTERS 
    CONFIGURE_DEPENDS
    src/core/filters/*.cpp
    src/core/filters/*.h
)

set(ALL_SOURCES ${SRC_FILES} ${CORE_FILTERS})

# ——————————————————————————————————
# Executable configuration
# ——————————————————————————————————

if(WIN32)
    add_executable(${PROJECT_NAME} WIN32 ${ALL_SOURCES})
else()
    add_executable(${PROJECT_NAME} ${ALL_SOURCES})
endif()


# ——————————————————————————————————
# Resources
# ——————————————————————————————————

qt_add_resources(${PROJECT_NAME} "app_resources"
    PREFIX "/"
    FILES
    res/gif/spinner.gif
    res/gif/1493.gif
    res/icons/contrast.png
    res/icons/gaussian.png
    res/icons/histogram.png
    res/icons/mean.png
    res/icons/median.png
    res/icons/mirror.png
    res/icons/prewitt.png
    res/icons/rotate.png
    res/icons/sharpen.png
    res/icons/sobel.png
    res/icons/shift.png
    res/icons/dilation.png
    res/icons/erosion.png
    res/icons/skeletonization.png
    res/icons/centroid.png
    res/icons/manual.png
    res/icons/kapur.png
    res/icons/otsu.png
    res/logos/ktu.png
    res/logos/logo.ico
)

# ——————————————————————————————————
# Include directories
# ——————————————————————————————————

target_include_directories(${PROJECT_NAME} PRIVATE
    ${CMAKE_CURRENT_SOURCE_DIR}/src
    ${CMAKE_CURRENT_SOURCE_DIR}/src/core
    ${CMAKE_CURRENT_SOURCE_DIR}/src/gui
    ${CMAKE_CURRENT_SOURCE_DIR}/src/core/filters
)

# ——————————————————————————————————
# Dependencies
# ——————————————————————————————————

target_link_libraries(${PROJECT_NAME} PRIVATE
    Qt6::Widgets Qt6::Core Qt6::Gui Qt6::Charts Qt6::Qml Qt6::Concurrent
)
