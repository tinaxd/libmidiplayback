cmake_minimum_required(VERSION 3.5)

project(midiplayback LANGUAGES CXX)

set(CMAKE_INCLUDE_CURRENT_DIR ON)
set(CMAKE_AUTOUIC ON)
#set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)
set(CMAKE_CXX_STANDARD 14)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

SET(EXECUTABLE_OUTPUT_PATH ${CMAKE_BINARY_DIR}/bin)
SET(CMAKE_ARCHIVE_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/lib)
SET(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/lib)
SET(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)

add_subdirectory(ext/midifile)
add_subdirectory(ext/rtmidi)

find_package(Qt5 COMPONENTS Core REQUIRED)

qt5_wrap_cpp(moc_sources midiplayer.h)

add_library(midiplayback SHARED
  libmidiplayback_global.h
  midiplayer.cpp
  midiplayer.h
  ${moc_sources}
)

add_dependencies(midiplayback midifile rtmidi)

target_link_libraries(midiplayback PRIVATE Qt5::Core midifile rtmidi)

target_compile_definitions(midiplayback PRIVATE LIBMIDIPLAYBACK_LIBRARY)

add_executable(testmidplay EXCLUDE_FROM_ALL example/testmid_play.cpp ${moc_sources})
target_link_libraries(testmidplay PRIVATE Qt5::Core midiplayback midifile rtmidi)

add_custom_target(examples)
add_dependencies(examples testmidplay)
