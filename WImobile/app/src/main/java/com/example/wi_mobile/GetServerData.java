package com.example.wi_mobile;

import android.content.Intent;
import android.net.http.AndroidHttpClient;
import android.telephony.mbms.DownloadStatusListener;

import org.apache.http.HttpResponse;
import org.apache.http.HttpStatus;
import org.apache.http.StatusLine;
import org.apache.http.client.HttpClient;
import org.apache.http.client.methods.HttpGet;
import org.apache.http.client.methods.HttpPost;
import org.apache.http.entity.StringEntity;
import org.apache.http.impl.client.DefaultHttpClient;
import org.apache.http.params.BasicHttpParams;
import org.apache.http.params.HttpConnectionParams;
import org.apache.http.params.HttpParams;
import org.json.JSONObject;

import java.io.ByteArrayOutputStream;
import java.io.IOException;

public class GetServerData {
    //private String serverURL = "http://weatherinnovation.pythonanywhere.com/";
    private String serverURL = "http://192.168.1.49:8000/";
    private String device_id = null;

    public GetServerData(String device_id) {
        this.device_id = device_id;
    }

    public String getLastSurvey() throws IOException {
        String request_url = serverURL + "api/getlastsurvey";

        HttpClient http_client = new DefaultHttpClient();
        HttpPost http_post = new HttpPost(request_url);

        String request = "{\"device_id\" : \"" +device_id + "\"}";
        http_post.setEntity(new StringEntity(request));
        HttpResponse response = http_client.execute(http_post);
        StatusLine status_line = response.getStatusLine();
        String responseString = Integer.toString(status_line.getStatusCode());


        if(status_line.getStatusCode() == HttpStatus.SC_OK){
            ByteArrayOutputStream out = new ByteArrayOutputStream();
            response.getEntity().writeTo(out);
            responseString = out.toString();
            out.close();
        } else{
            //Closes the connection.
            response.getEntity().getContent().close();
            //responseString = status_line.getReasonPhrase().toString();
        }

        return responseString;
    }
}
