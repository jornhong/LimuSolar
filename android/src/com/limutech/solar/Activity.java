package com.limutech.solar;


import android.app.Notification;
import android.app.NotificationManager;
import android.content.Context;
import android.util.Log;
import android.content.Intent;
import android.content.pm.PackageManager;
import android.app.PendingIntent;
import android.app.AlarmManager;
import android.os.Handler;
import android.os.Message;
import android.widget.Toast;
import android.content.Intent;
import android.app.PendingIntent;
import android.os.Vibrator;

public class Activity extends org.qtproject.qt5.android.bindings.QtActivity{
    private static NotificationManager m_notificationManager;
    private static Notification.Builder m_builder;
    private static Activity m_instance;
    private static final int MSG_SHOW_TOAST = 1;
    private static final String TAG = "MainActivity";

    public Activity()
    {
        m_instance = this;
    }

    public static String getVersion() throws Exception
    {
        String versionName = m_instance.getPackageManager().getPackageInfo(m_instance.getPackageName(), 0).versionName;
        return versionName;
    }

    public static int getVersionCode() throws Exception
    {
        int versionCode = m_instance.getPackageManager().getPackageInfo(m_instance.getPackageName(), 0).versionCode;
        return versionCode;
    }

    public static void restart()
    {
        Intent mStartActivity = new Intent(m_instance,Activity.class);
        int mPendingIntentId = 123456;
        PendingIntent mPendingIntent = PendingIntent.getActivity(m_instance, mPendingIntentId,mStartActivity, PendingIntent.FLAG_CANCEL_CURRENT);
        AlarmManager mgr = (AlarmManager)m_instance.getSystemService(Context.ALARM_SERVICE);
        mgr.set(AlarmManager.RTC, System.currentTimeMillis() + 3000, mPendingIntent);
        //m_instance.finish();
        System.exit(0);
    }

    public static void vibrate(int duration)
    {
        Vibrator v = (Vibrator) m_instance.getSystemService(Context.VIBRATOR_SERVICE);
        v.vibrate(duration);
    }

    public static void notify(String s)
    {
        if (m_notificationManager == null) {
            m_notificationManager = (NotificationManager)m_instance.getSystemService(Context.NOTIFICATION_SERVICE);
            m_builder = new Notification.Builder(m_instance);
            m_builder.setSmallIcon(R.drawable.icon);
            m_builder.setContentTitle("A message from Limutech !");
        }

        m_builder.setContentText(s);
        m_notificationManager.notify(1, m_builder.build());
    }



    private static Handler messageHandler = new Handler() {
        public void handleMessage(android.os.Message msg) {
            if (msg.what == MSG_SHOW_TOAST) {
                String message = (String)msg.obj;
                Toast.makeText(m_instance, message , Toast.LENGTH_SHORT).show();
            }
        }
    };

    private static void displayMessage(String s) {
        Message msg = new Message();
        msg.what = MSG_SHOW_TOAST;
        msg.obj = s;
        messageHandler.sendMessage(msg);
    }

    public static void doRestart()
    {
        Context c = m_instance;
        try {
            //check if the context is given
            if (c != null) {
                //fetch the packagemanager so we can get the default launch activity
                // (you can replace this intent with any other activity if you want
                PackageManager pm = c.getPackageManager();
                //check if we got the PackageManager
                if (pm != null) {
                    //create the intent with the default start activity for your application
                    Intent mStartActivity = pm.getLaunchIntentForPackage(
                    c.getPackageName()
                    );
                    if (mStartActivity != null) {
                        mStartActivity.addFlags(Intent.FLAG_ACTIVITY_CLEAR_TOP);
                        //create a pending intent so the application is restarted after System.exit(0) was called.
                        // We use an AlarmManager to call this intent in 100ms
                        int mPendingIntentId = 223344;
                        PendingIntent mPendingIntent = PendingIntent
                        .getActivity(c, mPendingIntentId, mStartActivity,
                        PendingIntent.FLAG_CANCEL_CURRENT);
                        AlarmManager mgr = (AlarmManager) c.getSystemService(Context.ALARM_SERVICE);
                        mgr.set(AlarmManager.RTC, System.currentTimeMillis() + 100, mPendingIntent);
                        //kill the application
                        System.exit(0);
                    } else {
                        Log.e(TAG, "Was not able to restart application, mStartActivity null");
                    }
                } else {
                    Log.e(TAG, "Was not able to restart application, PM null");
                }
            } else {
                Log.e(TAG, "Was not able to restart application, Context null");
            }
        } catch (Exception ex) {
            Log.e(TAG, "Was not able to restart application");
        }

    }
}
