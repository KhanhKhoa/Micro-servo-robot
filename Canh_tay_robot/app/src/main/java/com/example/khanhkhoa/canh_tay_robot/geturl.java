package com.example.khanhkhoa.canh_tay_robot;

import java.io.IOException;

import okhttp3.OkHttpClient;
import okhttp3.Request;
import okhttp3.Response;

public class geturl {
    public static String Guilenh(String urllenh){
        OkHttpClient client = new OkHttpClient();

        Request request = new Request.Builder()
                .url(urllenh)
                .build();
        try{
            Response response = client.newCall(request).execute();
            return response.body().string();

        }catch (IOException erro){
            return erro.toString();
        }
    }
}
