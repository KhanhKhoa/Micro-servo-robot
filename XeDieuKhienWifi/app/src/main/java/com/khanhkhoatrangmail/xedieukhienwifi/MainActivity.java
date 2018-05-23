package com.khanhkhoatrangmail.xedieukhienwifi;

import android.content.Context;
import android.net.ConnectivityManager;
import android.net.NetworkInfo;
import android.os.AsyncTask;
import android.support.v7.app.AppCompatActivity;
import android.os.Bundle;
import android.view.MotionEvent;
import android.view.View;
import android.widget.Button;
import android.widget.EditText;
import android.widget.Toast;

public class MainActivity extends AppCompatActivity {
    Button tang, giam, thang, lui, trai, phai, dung, chonchedo, coi;
    EditText tocdo, trangthai, chedo;
    Boolean tt=false;
    @Override
    protected void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        setContentView(R.layout.activity_main);

        tang=(Button) findViewById(R.id.tang);
        giam=(Button) findViewById(R.id.giam);
        thang=(Button)findViewById(R.id.thang);
        lui=(Button)findViewById(R.id.lui);
        trai=(Button)findViewById(R.id.trai);
        phai=(Button)findViewById(R.id.phai);
        dung=(Button)findViewById(R.id.dung);
        chonchedo=(Button)findViewById(R.id.chedo);
        tocdo=(EditText)findViewById(R.id.tocdo);
        trangthai=(EditText)findViewById(R.id.trangthai);
        chedo=(EditText)findViewById(R.id.editText);
        coi=(Button)findViewById(R.id.coi);

        tang.setOnClickListener(new View.OnClickListener() {
            @Override
            public void onClick(View v) {
                Taolenh("tang");
            }
        });
        giam.setOnClickListener(new View.OnClickListener() {
            @Override
            public void onClick(View v) {
                Taolenh("giam");
            }
        });
        thang.setOnTouchListener(new View.OnTouchListener() {
            @Override
            public boolean onTouch(View v, MotionEvent event) {
                if(event.getAction()==MotionEvent.ACTION_DOWN && !tt)
                {
                    Taolenh("thang");
                    return true;
                }
                else if(event.getAction()==MotionEvent.ACTION_UP && !tt)
                {
                    Taolenh("dung");
                    return true;
                }
                return false;
            }
        });
        lui.setOnTouchListener(new View.OnTouchListener() {
            @Override
            public boolean onTouch(View v, MotionEvent event) {
                if(event.getAction()==MotionEvent.ACTION_DOWN)
                {
                    Taolenh("lui");
                    return true;
                }
                else if(event.getAction()==MotionEvent.ACTION_UP)
                {
                    Taolenh("dung");
                    return true;
                }
                return false;
            }
        });
        phai.setOnTouchListener(new View.OnTouchListener() {
            @Override
            public boolean onTouch(View v, MotionEvent event) {
                if(event.getAction()==MotionEvent.ACTION_DOWN)
                {
                    Taolenh("phai");
                    return true;
                }
                else if(event.getAction()==MotionEvent.ACTION_UP)
                {
                    Taolenh("dung");
                    return true;
                }
                return false;
            }
        });
        trai.setOnTouchListener(new View.OnTouchListener() {
            @Override
            public boolean onTouch(View v, MotionEvent event) {
                if(event.getAction()==MotionEvent.ACTION_DOWN)
                {
                    Taolenh("trai");
                    return true;
                }
                else if(event.getAction()==MotionEvent.ACTION_UP)
                {
                    Taolenh("dung");
                    return true;
                }
                return false;
            }
        });
        chonchedo.setOnClickListener(new View.OnClickListener() {
            @Override
            public void onClick(View v) {
                Taolenh("chedo");
                tt=!tt;
            }
        });
        coi.setOnTouchListener(new View.OnTouchListener() {
            @Override
            public boolean onTouch(View v, MotionEvent event) {
                if(event.getAction()==MotionEvent.ACTION_DOWN)
                {
                    Taolenh("coi");
                    return true;
                }
                else if(event.getAction()==MotionEvent.ACTION_UP)
                {
                    Taolenh("tatcoi");
                    return true;
                }
                return false;
            }
        });
    }
    public void Taolenh(String lenh){
        ConnectivityManager connectivityManager =(ConnectivityManager)
                getSystemService(Context.CONNECTIVITY_SERVICE);
        NetworkInfo networkInfo = connectivityManager.getActiveNetworkInfo();
        if(networkInfo != null && networkInfo.isConnected()){
            String url = "http://192.168.4.1/";
            new Guilenserver().execute(url + lenh);
        }else{
            Toast.makeText(MainActivity.this, "Gui that bai",Toast.LENGTH_LONG).show();
        }
    }
    private class Guilenserver extends AsyncTask<String, Void, String> {

        @Override
        public String doInBackground(String... url) {
            return Geturl.Guilenh(url[0]);
        }
        @Override
        public void onPostExecute(String result) {
            if(result != null){
                if(result.contains("thang")){
                    trangthai.setText("xe tien");
                }
                else if(result.contains("lui")){
                    trangthai.setText("xe lui");
                }
                else if(result.contains("trai")){
                    trangthai.setText("re trai");
                }
                else if(result.contains("phai")){
                    trangthai.setText("re phai");
                }
                else if(result.contains("dung")){
                    trangthai.setText("xe dung");
                }
                if(result.contains("400")){
                    tocdo.setText("400");
                }
                else if(result.contains("500")){
                    tocdo.setText("500");
                }
                else if(result.contains("600")){
                    tocdo.setText("600");
                }
                else if(result.contains("700")){
                    tocdo.setText("700");
                }
                else if(result.contains("800")){
                    tocdo.setText("800");
                }
                else if(result.contains("900")){
                    tocdo.setText("900");
                }
                else if(result.contains("1000")){
                    tocdo.setText("1000");
                }
                if(result.contains("auto")){
                    chedo.setText("chay tu dong");
                }
                else if(result.contains("tay")){
                    chedo.setText("dieu khien tay");
                }
            }else{
                Toast.makeText(MainActivity.this, "Khong co phan hoi",Toast.LENGTH_LONG).show();
            }
        }
    }
}
