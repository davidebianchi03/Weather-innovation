package com.example.myapplication;

import android.util.Log;
import java.io.IOException;

public class GetLastSurvey extends Thread {

    /*
    GetLastSurvey getLastSurvey = new GetLastSurvey("a0001");
        getLastSurvey.start();
        try {
            Thread.sleep(1000);
        } catch (InterruptedException e) {
            e.printStackTrace();
        }
        txt1.setText(getLastSurvey.getResponse());
     */

    String device_id = null;
    String response;

    public GetLastSurvey(String device_id) {
        this.device_id = device_id;
    }

    @Override
    public void run() {
        GetServerData get_survey_data = new GetServerData("a0001");

        try {
            response = get_survey_data.getLastSurvey();
        } catch (IOException e) {
            e.printStackTrace();
            response = e.toString();
        }

        Log.d("RESPONSE", response);
    }

    public String getResponse() {
        return response;
    }

}
