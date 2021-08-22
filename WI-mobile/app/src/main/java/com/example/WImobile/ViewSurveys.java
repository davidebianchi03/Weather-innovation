package com.example.WImobile;

import androidx.annotation.NonNull;
import androidx.appcompat.app.AppCompatActivity;

import android.os.Bundle;
import android.view.View;
import android.widget.Button;
import android.widget.EditText;
import android.widget.TextView;

import com.google.android.gms.maps.GoogleMap;
import com.google.android.gms.maps.OnMapReadyCallback;

import org.json.JSONException;
import org.json.JSONObject;

public class ViewSurveys extends AppCompatActivity implements OnMapReadyCallback {

    Button btn_go = null;
    EditText edit_device_id = null;
    TextView txt_temperature = null;
    TextView txt_humidity = null;
    TextView txt_pressure = null;
    TextView txt_device_id = null;
    String device_id = null;

    @Override
    protected void onCreate(Bundle savedInstanceState) {
        //requestWindowFeature(Window.FEATURE_NO_TITLE);//nascondo il titolo
        //getSupportActionBar().hide(); //nascondo la barra del titolo
        super.onCreate(savedInstanceState);
        setContentView(R.layout.activity_view_surveys);

        btn_go = findViewById(R.id.btn_go);
        edit_device_id = findViewById(R.id.txt_device_id);
        txt_temperature = findViewById(R.id.text_view_temperature);
        txt_humidity = findViewById(R.id.text_view_humidity);
        txt_pressure = findViewById(R.id.text_view_pressure);
        txt_device_id = findViewById(R.id.text_view_device_id);

        btn_go.setOnClickListener(new View.OnClickListener() {
            @Override
            public void onClick(View view) {
                device_id = edit_device_id.getText().toString();
                refresh_data();
            }
        });

    }

    void refresh_data(){
        txt_device_id.setText(device_id);

        GetLastSurvey getLastSurvey = new GetLastSurvey(device_id);
        getLastSurvey.start();

        try {
            Thread.sleep(1000);
        } catch (InterruptedException e) {
            e.printStackTrace();
        }

        String raw_json_data = getLastSurvey.getResponse();

        try {
            JSONObject json = new JSONObject(raw_json_data);
            int temperature = json.getInt("temperature");
            int humidity = json.getInt("humidity");
            int pressure = json.getInt("pressure");

            txt_temperature.setText(Integer.toString(temperature) + "°C");
            txt_humidity.setText(Integer.toString(humidity) + "%");
            txt_pressure.setText(Integer.toString(pressure) + " mBar");
        } catch (JSONException e) {
            e.printStackTrace();
        }
    }

    @Override
    public void onMapReady(@NonNull GoogleMap googleMap) {

    }
}