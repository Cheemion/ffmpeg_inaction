cmake_minimum_required(VERSION 3.18)
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}/bin)
project(FFMPEG_INACTION)
include_directories(./include)
link_directories(./lib)
link_libraries(
    avcodec
    avformat
    avfilter
    avdevice
    swresample
    swscale
    avutil
)
ADD_EXECUTABLE(save_rtsp  "save_rtsp.cpp") 
