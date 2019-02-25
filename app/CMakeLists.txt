cmake_minimum_required(VERSION 3.4.1)

set(opencv_dir ${PROJECT_SOURCE_DIR}/src/main/sdk/native)

# native recognition library API
add_library(native-lib
        SHARED
        src/main/cpp/native-lib.cpp
        )


find_library(jnigraphics-lib jnigraphics)

# OpenCV lib linking and includes
include_directories(${opencv_dir}/jni/include)
add_library(cv_core-lib SHARED IMPORTED)
set_target_properties(cv_core-lib PROPERTIES IMPORTED_LOCATION ${opencv_dir}/libs/${CMAKE_ANDROID_ARCH_ABI}/libopencv_core.so)

add_library(cv_imagecodecs-lib SHARED IMPORTED)
set_target_properties(cv_imagecodecs-lib PROPERTIES IMPORTED_LOCATION ${opencv_dir}/libs/${CMAKE_ANDROID_ARCH_ABI}/libopencv_imgproc.so)

find_library(log-lib log)

target_link_libraries(
        native-lib
        cv_core-lib
        cv_imagecodecs-lib
        ${log-lib}
        ${jnigraphics-lib}
)