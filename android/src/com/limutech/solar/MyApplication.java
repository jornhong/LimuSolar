package com.limutech.solar;
import org.qtproject.qt5.android.bindings.QtApplication;
import android.app.Application;
import im.fir.sdk.FIR;

public class MyApplication extends QtApplication {

    @Override
    public void onCreate() {

        super.onCreate();
        FIR.init(this);
    }
}
