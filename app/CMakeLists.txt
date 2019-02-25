cmake_minimum_required(VERSION 3.4.1)

set(CMAKE_SHARED_LINKER_FLAGS "-Wl,-ljnigraphics")
set(opencv_dir /media/vinayaka/B08484058483CBF2/Projects/Android/OpenCvTiny/app/src/main/sdk/native)

# native recognition library API
add_library(native-lib
        SHARED
        src/main/cpp/native-lib.cpp
        )

# OpenCV lib linking and includes
include_directories(${opencv_dir}/jni/include)
add_library(cv_core-lib SHARED IMPORTED)
set_target_properties(cv_core-lib PROPERTIES IMPORTED_LOCATION ${opencv_dir}/libs/${CMAKE_ANDROID_ARCH_ABI}/libopencv_core.so)

add_library(cv_imagecodecs-lib SHARED IMPORTED)
set_target_properties(cv_imagecodecs-lib PROPERTIES IMPORTED_LOCATION ${opencv_dir}/libs/${CMAKE_ANDROID_ARCH_ABI}/libopencv_imgproc.so)

find_library(log-lib log)
find_library( jnigraphics-lib jnigraphics )

target_link_libraries(
        native-lib
        ${jnigraphics-lib}
        cv_core-lib
        cv_imagecodecs-lib
        ${log-lib}
)