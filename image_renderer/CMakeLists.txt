cmake_minimum_required(VERSION 3.14)

project(test_transponde LANGUAGES CXX)

set(CMAKE_AUTOUIC ON)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Find OpenCV
find_package(OpenCV REQUIRED)

find_package(QT NAMES Qt6 REQUIRED COMPONENTS Core Gui Opengl)
find_package(Qt6 REQUIRED COMPONENTS Core Gui Widgets 3DCore 3DExtras 3DRender OpenGLWidgets)

include_directories(
/usr/include/opencv4/
)
add_executable(test_transponde
  main.cpp
  trianglerenderer.h trianglerenderer.cpp
  RenderOffScreen.h RenderOffScreen.cpp
  renderegl.h renderegl.cpp
  renderegl2.cpp renderegl2.h
  uncrustify.cfg
)
target_link_libraries(test_transponde Qt${QT_VERSION_MAJOR}::Core
Qt${QT_VERSION_MAJOR}::Gui
Qt${QT_VERSION_MAJOR}::OpenGL
# Qt6::OpenGLWidgets
# Qt6::Widgets
# Qt6::3DCore
# Qt6::3DExtras
# Qt6::3DRender

-lEGL

# -losg
# -losgDB
# -losgViewer
# -lopencv_core
# -lopencv_imgcodecs
# -lopencv_imgproc
)

include(GNUInstallDirs)
install(TARGETS test_transponde
    LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}
    RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR}
)
