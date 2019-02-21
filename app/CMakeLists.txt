cmake_minimum_required(VERSION 3.4.1)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=gnu++11")
set(opencv_dir src/main/sdk/native/jni)

# native recognition library API
add_library(native-lib
        SHARED
        src/main/cpp/native-lib.cpp
        )

# OpenCV lib linking and includes
include_directories(${opencv_dir}/include)
add_library(opencv-lib SHARED IMPORTED)
set_target_properties(opencv-lib PROPERTIES IMPORTED_LOCATION src/main/sdk/native/libs/${ANDROID_ABI}/libopencv_java.so)


find_library(log-lib log)

target_link_libraries(
        native-lib
        opencv-lib
        ${log-lib}
)

target_link_libraries(native-lib ${log-lib})