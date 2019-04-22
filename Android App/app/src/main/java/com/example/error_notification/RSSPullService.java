package com.example.error_notification;

import android.app.IntentService;
import android.app.Notification;
import android.app.PendingIntent;
import android.content.Context;
import android.content.Intent;
import android.os.Build;
import android.os.Handler;
import android.support.annotation.RequiresApi;
import android.support.v4.app.NotificationCompat;
import android.support.v4.app.NotificationManagerCompat;
import android.util.Log;
import android.view.View;
import android.widget.Toast;

import com.android.volley.Request;
import com.android.volley.RequestQueue;
import com.android.volley.Response;
import com.android.volley.VolleyError;
import com.android.volley.toolbox.StringRequest;
import com.android.volley.toolbox.Volley;

import org.json.JSONArray;
import org.json.JSONException;
import org.json.JSONObject;

import static com.example.error_notification.App.CHANNEL_ID;

public class RSSPullService extends IntentService {

    private NotificationManagerCompat notificationManagerCompat;

    /**
     * Creates an IntentService.  Invoked by your subclass's constructor.
     *  @param name Used to name the worker thread, important only for debugging.
     * @param context
     * @param url
     */
    public RSSPullService(String name, Context context, String url) {
        super(name);

    }

    public RSSPullService() {
        super("my_intent_thread");

    }

    @Override
    public int onStartCommand(Intent intent, int flags, int startId){
        Toast.makeText(RSSPullService.this, "service started", Toast.LENGTH_SHORT).show();
//        return super.onStartCommand(intent, flags, startId);
//        return 1;
        return START_NOT_STICKY;
    }

    @Override
    public void onDestroy(){
        Toast.makeText(RSSPullService.this, "service destroyed", Toast.LENGTH_LONG).show();
        super.onDestroy();
    }

    @Override
    protected void onHandleIntent(final Intent workIntent) {
        Log.d("INTENTAAA", "AAAAAA");
        Toast.makeText(RSSPullService.this, "Go IN", Toast.LENGTH_LONG).show();
        if (true) {

            Intent notificationIntent = new Intent(this, MainActivity.class);
            final PendingIntent pendingIntent = PendingIntent.getActivity(this, 0, notificationIntent, 0);
            final Notification notification = new NotificationCompat.Builder(this, CHANNEL_ID)
                    .setContentTitle("Notification Error")
                    .setContentText("SOMETHING ERROR")
                    .setSmallIcon(R.drawable.ic_launcher_background)
                    .setContentIntent(pendingIntent)
                    .build();
            startForeground(1, notification);
            final Handler mHandler = new Handler();
            Log.d("INTENTAAAA", "AAAAABA");
            mHandler.post(new Runnable() {
                @Override
                public void run() {
                    Toast.makeText(getApplicationContext(), "Go IN", Toast.LENGTH_LONG).show();
                }
            });
//            mHandler.postDelayed(new Runnable() {
//                @Override
//                public void run() {
//                    Log.d("INTENSWTAAAA", "AAAAABWqA");
//                    if ("0".equals(GlobalStatusNoti.error_index)){
//
//                        Toast.makeText(getApplicationContext(), "Normal", Toast.LENGTH_LONG).show();
//                    }
//                    else {
//                        RequestQueue queue = Volley.newRequestQueue(getApplicationContext());
//                        StringRequest stringRequest = new StringRequest(Request.Method.GET, GlobalStatusNoti.url, new Response.Listener<String>() {
//                            @Override
//                            public void onResponse(String response) {
////                inputAddress.setText(response);
//                                try {
////                    JSONObject jsonObj = new JSONObject(response);
//                                    JSONArray array = new JSONArray(response);
//
//                                    for (int i = 0; i < array.length(); i++) {
//                                        JSONObject c = array.getJSONObject(i);
//
//                                        // Testing
//                                        String machine = c.getString("machine_index");
//                                        String error = c.getString("error_index");
////                        inputAddress.setText(machine);
//
//                                        GlobalStatusNoti.machine_index = "0";
//                                        GlobalStatusNoti.error_index = "0";
//
//                                        if ("1".equals(machine)) {
//                                            if ("0".equals(error)) {
//
//
//                                            } else {
//                                                GlobalStatusNoti.machine_index = machine;
//                                                GlobalStatusNoti.error_index = error;
//
//                                                startForeground(1, notification);
//                                            }
//                                        } else if ("2".equals(machine)) {
//                                            if ("0".equals(error)) {
//
//
//                                            } else {
//                                                GlobalStatusNoti.machine_index = machine;
//                                                GlobalStatusNoti.error_index = error;
//                                            }
//                                        } else if ("3".equals(machine)) {
//                                            if ("0".equals(error)) {
//
//
//                                            } else {
//                                                GlobalStatusNoti.machine_index = machine;
//                                                GlobalStatusNoti.error_index = error;
//                                            }
//                                        }
//                                    }
//                                } catch (JSONException e) {
//
//                                }
//                            }
//                        }, new Response.ErrorListener() {
//                            @Override
//                            public void onErrorResponse(VolleyError error) {
//
////                editText.setText("aaa");
//                            }
//                        });
//                    }
//                    mHandler.postDelayed(this, 1000);
//                }
//            }, 1000);
        }

    }
}
