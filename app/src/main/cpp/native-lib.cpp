#include <jni.h>
#include <string>
#include <opencv2/opencv.hpp>
#include <opencv2/core.hpp>
#include <opencv2/imgproc/imgproc.hpp>
#include <android/bitmap.h>
#include <opencv2/imgproc/types_c.h>

using namespace cv;

jobject mat_to_bitmap(JNIEnv *env, Mat &src, bool needPremultiplyAlpha, jobject bitmap_config) {
    jclass java_bitmap_class = (jclass) env->FindClass("android/graphics/Bitmap");
    jmethodID mid = env->GetStaticMethodID(java_bitmap_class,
                                           "createBitmap",
                                           "(IILandroid/graphics/Bitmap$Config;)Landroid/graphics/Bitmap;");

    jobject bitmap = env->CallStaticObjectMethod(java_bitmap_class,
                                                 mid, src.size().width, src.size().height,
                                                 bitmap_config);
    AndroidBitmapInfo info;
    void *pixels = 0;

    try {
        CV_Assert(AndroidBitmap_getInfo(env, bitmap, &info) >= 0);
        CV_Assert(src.type() == CV_8UC1 || src.type() == CV_8UC3 || src.type() == CV_8UC4);
        CV_Assert(AndroidBitmap_lockPixels(env, bitmap, &pixels) >= 0);
        CV_Assert(pixels);
        if (info.format == ANDROID_BITMAP_FORMAT_RGBA_8888) {
            Mat tmp(info.height, info.width, CV_8UC4, pixels);
            if (src.type() == CV_8UC1) {
                cvtColor(src, tmp, CV_GRAY2RGBA);
            } else if (src.type() == CV_8UC3) {
                cvtColor(src, tmp, CV_RGB2RGBA);
            } else if (src.type() == CV_8UC4) {
                if (needPremultiplyAlpha) {
                    cv::cvtColor(src, tmp, COLOR_RGBA2mRGBA);
                } else {
                    src.copyTo(tmp);
                }
            }
        } else {
            // info.format == ANDROID_BITMAP_FORMAT_RGB_565
            Mat tmp(info.height, info.width, CV_8UC2, pixels);
            if (src.type() == CV_8UC1) {
                cvtColor(src, tmp, CV_GRAY2BGR565);
            } else if (src.type() == CV_8UC3) {
                cvtColor(src, tmp, CV_RGB2BGR565);
            } else if (src.type() == CV_8UC4) {
                cvtColor(src, tmp, CV_RGBA2BGR565);
            }
        }
        AndroidBitmap_unlockPixels(env, bitmap);
        return bitmap;
    } catch (cv::Exception e) {
        AndroidBitmap_unlockPixels(env, bitmap);
        jclass je = env->FindClass("org/opencv/core/CvException");
        if (!je) je = env->FindClass("java/lang/Exception");
        env->ThrowNew(je, e.what());
        return bitmap;
    } catch (...) {
        AndroidBitmap_unlockPixels(env, bitmap);
        jclass je = env->FindClass("java/lang/Exception");
        env->ThrowNew(je, "Unknown exception in JNI code {nMatToBitmap}");
        return bitmap;
    }
}

#if __cplusplus
extern "C"
{
#endif
JNIEXPORT jobject JNICALL
Java_com_example_opencvtiny_OpenCv_n_1getBAndWBitmap(JNIEnv *env, jobject instance, jobject src) {
    int ret;
    AndroidBitmapInfo info;
    void *pixels = 0;
    if ((ret = AndroidBitmap_getInfo(env, src, &info)) < 0) {
        return NULL;
    }
    if (info.format != ANDROID_BITMAP_FORMAT_RGBA_8888) {
        return NULL;
    }
    if ((ret = AndroidBitmap_lockPixels(env, src, &pixels)) < 0) {
        return NULL;
    }
    Mat mbgra(info.height, info.width, CV_8UC4, pixels);
    Mat dest = mbgra.clone();
    cvtColor(mbgra, dest, CV_BGR2GRAY);
    threshold(dest, dest, 0, 255, THRESH_BINARY | THRESH_OTSU);
    jclass java_bitmap_class = (jclass) env->FindClass("android/graphics/Bitmap");
    jmethodID mid = env->GetMethodID(java_bitmap_class, "getConfig",
                                     "()Landroid/graphics/Bitmap$Config;");
    jobject bitmap_config = env->CallObjectMethod(src, mid);
    jobject _bitmap = mat_to_bitmap(env, dest, false, bitmap_config);
    AndroidBitmap_unlockPixels(env, src);
    dest.release();
    return _bitmap;

}

#if __cplusplus
}
#endif

