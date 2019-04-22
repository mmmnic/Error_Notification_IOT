package com.example.error_notification;

import android.app.Activity;
import android.content.Intent;
import android.content.SharedPreferences;
import android.graphics.Color;
import android.os.Handler;
import android.support.v7.app.AppCompatActivity;
import android.os.Bundle;
import android.view.View;
import android.widget.Button;
import android.widget.EditText;
import android.widget.ImageView;
import android.widget.TextView;
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
import org.w3c.dom.Text;

public class MainActivity extends AppCompatActivity {

    EditText inputAddress;
    EditText inputPort;
    TextView connectionStatus;
    Button saveAndStart, reset1, reset2;
    ImageView machine1, machine2, machine3;

    long current_time = 0;
    @Override
    protected void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        setContentView(R.layout.activity_main);

        inputAddress = (EditText) findViewById(R.id.inputAddress);
        inputPort = (EditText) findViewById(R.id.inputPort);
        saveAndStart = (Button) findViewById(R.id.saveAndStart);

        reset1 = (Button) findViewById(R.id.reset1);
        reset2 = (Button) findViewById(R.id.reset2);

        machine1 = (ImageView) findViewById(R.id.machine1_status);
        machine2 = (ImageView) findViewById(R.id.machine2_status);
        machine3 = (ImageView) findViewById(R.id.machine3_status);

        connectionStatus = (TextView) findViewById(R.id.connection_status);

        // Shared preference
        final SharedPreferences pre=getSharedPreferences("Saved_Setting", MODE_PRIVATE);

        try{
            String address = pre.getString("address", "");
            String port = pre.getString("port", "");

            inputAddress.setText(address);
            inputPort.setText(port);
        }catch (Exception e){

        }

        // Start Service
//        RSSPullService rssPullService = new RSSPullService("thread1", getApplicationContext(), "http://" + inputAddress.getText() + ":" + inputPort.getText() +"/GetStatus.php");




        saveAndStart.setOnClickListener(new View.OnClickListener() {
            @Override
            public void onClick(View v) {
                final String url = "http://" + inputAddress.getText() + ":" + inputPort.getText() +"/GetStatus.php";
                GlobalStatusNoti.url = "http://" + inputAddress.getText() + ":" + inputPort.getText() +"/GetStatus.php";
//                inputPort.setText(url);

                SharedPreferences.Editor edit = pre.edit();

                edit.putString("address", String.valueOf(inputAddress.getText()));
                edit.putString("port", String.valueOf(inputPort.getText()));

                edit.commit();

//                CheckFirstTime(url);

                final Handler h = new Handler();
                h.postDelayed(new Runnable() {
                    @Override
                    public void run() {

                        CheckFirstTime(url);
                        h.postDelayed(this, 500);
                    }
                }, 500);
            }
        });

    }

    View.OnClickListener reset1OnClickListener = new View.OnClickListener() {
        @Override
        public void onClick(View v) {
            inputAddress.setText("");

        }
    };

    View.OnClickListener reset2OnClickListener = new View.OnClickListener() {
        @Override
        public void onClick(View v) {
            inputPort.setText("");
        }
    };

    private void CheckFirstTime(String url){
        RequestQueue queue = Volley.newRequestQueue(getApplicationContext());

        StringRequest stringRequest = new StringRequest(Request.Method.GET, url, new Response.Listener<String>() {
            @Override
            public void onResponse(String response) {
//                inputAddress.setText(response);
                try {
//                    JSONObject jsonObj = new JSONObject(response);
                    JSONArray array = new JSONArray(response);

                    //Set Connection Status
                    connectionStatus.setText("Connected");
                    connectionStatus.setTextColor(Color.parseColor("#0f841b"));

                    for (int i = 0; i< array.length(); i++){
                        JSONObject c = array.getJSONObject(i);

                        // Testing
                        String machine = c.getString("machine_index");
                        String error = c.getString("error_index");
//                        inputAddress.setText(machine);

                        GlobalStatusNoti.machine_index = "0";
                        GlobalStatusNoti.error_index = "0";

                        if ("1".equals(machine)){
                            if ("0".equals(error)){
                                machine1.setImageResource(R.drawable.greenbtn);
                            }
                            else {

                                if (System.currentTimeMillis() - current_time > 10000){
                                    current_time = System.currentTimeMillis();
                                    Intent serviceIntent = new Intent(getApplicationContext(), serviceNoti.class);
                                    startService(serviceIntent);
                                }

                                GlobalStatusNoti.machine_index = machine;
                                GlobalStatusNoti.error_index = error;
                                machine1.setImageResource(R.drawable.redbtn);
                            }
                        }
                        else if ("2".equals(machine)){
                            if ("0".equals(error)){
                                machine2.setImageResource(R.drawable.greenbtn);
                            }
                            else {
                                machine2.setImageResource(R.drawable.redbtn);
                                if (System.currentTimeMillis() - current_time > 10000){
                                    current_time = System.currentTimeMillis();
                                    Intent serviceIntent = new Intent(getApplicationContext(), serviceNoti.class);
                                    startService(serviceIntent);
                                }
                                GlobalStatusNoti.machine_index = machine;
                                GlobalStatusNoti.error_index = error;
                            }
                        }
                        else if ("3".equals(machine)){
                            if ("0".equals(error)){
                                machine3.setImageResource(R.drawable.greenbtn);
                            }
                            else {
                                machine3.setImageResource(R.drawable.redbtn);
                                if (System.currentTimeMillis() - current_time > 10000){
                                    current_time = System.currentTimeMillis();
                                    Intent serviceIntent = new Intent(getApplicationContext(), serviceNoti.class);
                                    startService(serviceIntent);
                                }
                                GlobalStatusNoti.machine_index = machine;
                                GlobalStatusNoti.error_index = error;
                            }
                        }
                    }
                } catch (JSONException e) {
                    inputAddress.setText(e.toString());
                }
            }
        }, new Response.ErrorListener() {
            @Override
            public void onErrorResponse(VolleyError error) {
                connectionStatus.setText("Disconnected");
                connectionStatus.setTextColor(Color.parseColor("#bc0b20"));
//                editText.setText("aaa");
            }
        });

        queue.add(stringRequest);
    }
}
