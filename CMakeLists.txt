cmake_minimum_required(VERSION 3.5)
project(gwd)

set(CMAKE_CXX_STANDARD 17)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -pedantic -Wall -Wextra -Wcast-align -Wcast-qual -Wctor-dtor-privacy -Wdisabled-optimization -Wformat=2 -Winit-self -Wlogical-op -Wmissing-include-dirs -Wnoexcept -Wold-style-cast -Woverloaded-virtual -Wredundant-decls -Wshadow -Wsign-conversion -Wsign-promo -Wstrict-null-sentinel -Wstrict-overflow=5 -Wswitch-default -Wundef -Werror -Wno-unused")

set(CMAKE_INCLUDE_CURRENT_DIR ON)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)

find_package(roslib REQUIRED)

find_package(Qt5 REQUIRED COMPONENTS Widgets Xml)

# Not actually using this right now, but should I be? Needs more research
# find_package(SDFormat REQUIRED)

set(SOURCE_FILES main.cpp MainWindow.cpp WorldView.cpp ModelTreeItem.cpp ModelTreeModel.cpp qdarkstyle/style.qrc
        sdf/World.cpp sdf/IncludedElement.cpp sdf/Scene.cpp sdf/World.cpp sdf/SDFElement.cpp sdf/Color.cpp
        sdf/IncludedElement.cpp sdf/Pose.cpp sdf/Scene.cpp sdf/Parser.cpp PropertyPane.cpp)

add_executable(gwd ${SOURCE_FILES})
target_include_directories(gwd PRIVATE ${SDFormat_INCLUDE_DIRS} ${roslib_INCLUDE_DIRS})
target_link_directories(gwd PRIVATE ${roslib_LIBRARY_DIRS})
target_link_libraries(gwd PRIVATE Qt5::Widgets Qt5::Xml ${SDFormat_LIBRARIES} ${roslib_LIBRARIES})
