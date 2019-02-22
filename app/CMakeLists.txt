cmake_minimum_required(VERSION 3.4.1)

set(CMAKE_SHARED_LINKER_FLAGS "-Wl,-ljnigraphics")
set(opencv_dir ${CMAKE_SOURCE_DIR}/src/main/sdk/native/jni)

# native recognition library API
add_library(native-lib
        SHARED
        src/main/cpp/native-lib.cpp
        )

# OpenCV lib linking and includes
include_directories(${opencv_dir}/include)
add_library(opencv-lib SHARED IMPORTED)
set_target_properties(opencv-lib PROPERTIES IMPORTED_LOCATION ${CMAKE_SOURCE_DIR}/src/main/sdk/native/libs/${ANDROID_ABI}/libopencv_tiny.so)


find_library(log-lib log)

target_link_libraries(
        native-lib
        opencv-lib
        ${log-lib}
)