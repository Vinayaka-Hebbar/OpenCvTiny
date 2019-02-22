package com.example.opencvtiny;

import android.graphics.Bitmap;

public class OpenCv {
    static {
        System.loadLibrary("opencv_tiny");
        System.loadLibrary("native-lib");
    }

    public Bitmap getBAndWBitmap(Bitmap src){
        return n_getBAndWBitmap(src);
    }

    private native Bitmap n_getBAndWBitmap(Bitmap src);

}
