package com.example.khanhkhoa.canh_tay_robot;

import android.content.Context;
import android.net.ConnectivityManager;
import android.net.NetworkInfo;
import android.os.AsyncTask;
import android.support.v7.app.AppCompatActivity;
import android.os.Bundle;
import android.widget.SeekBar;
import android.widget.TextView;
import android.widget.Toast;

public class MainActivity extends AppCompatActivity {
    SeekBar goc1,goc2,goc3,goc4;
    String s1="90", s2="55", s3="140", s4="75", s;
    TextView t1, t2, t3, t4;
    @Override
    protected void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        setContentView(R.layout.activity_main);
        goc1 = (SeekBar)findViewById(R.id.sb1);
        goc2 = (SeekBar)findViewById(R.id.sb2);
        goc3 = (SeekBar)findViewById(R.id.sb3);
        goc4 = (SeekBar)findViewById(R.id.sb4);
        t1 = (TextView)findViewById(R.id.tv1);
        t2 = (TextView)findViewById(R.id.tv2);
        t3 = (TextView)findViewById(R.id.tv3);
        t4 = (TextView)findViewById(R.id.tv4);

    goc1.setOnSeekBarChangeListener(new SeekBar.OnSeekBarChangeListener() {
        @Override
        public void onProgressChanged(SeekBar seekBar, int progress, boolean fromUser) {
            if(progress<10) s1 = "00" + Integer.toString(progress);
            else if(progress<100)   s1 = "0" + Integer.toString(progress);
            else    s1 = Integer.toString(progress);
            t1.setText(s1);
            s = s1 + s2 + s3 + s4;
            Taolenh(s);
        }

        @Override
        public void onStartTrackingTouch(SeekBar seekBar) {
        }

        @Override
        public void onStopTrackingTouch(SeekBar seekBar) {
            Toast.makeText(MainActivity.this, "Servo 1",Toast.LENGTH_SHORT).show();
        }
    });
    goc2.setOnSeekBarChangeListener(new SeekBar.OnSeekBarChangeListener() {
        @Override
        public void onProgressChanged(SeekBar seekBar, int progress, boolean fromUser) {
            if(progress<10) s2 = "00" + Integer.toString(progress);
            else if (progress<100) s2 = "0" + Integer.toString(progress);
            else    s2 = Integer.toString(progress);
            t2.setText(s2);
            s = s1 + s2 + s3 + s4;
            Taolenh(s);
        }

        @Override
        public void onStartTrackingTouch(SeekBar seekBar) {
        }

        @Override
        public void onStopTrackingTouch(SeekBar seekBar) {
            Toast.makeText(MainActivity.this, "Servo 2",Toast.LENGTH_SHORT).show();
        }
    });
    goc3.setOnSeekBarChangeListener(new SeekBar.OnSeekBarChangeListener() {
        @Override
        public void onProgressChanged(SeekBar seekBar, int progress, boolean fromUser) {
            //servo 3 pham vi quay tu 100 den 180 do
            if(progress < 100)    s3 = "100";
            else    s3 = Integer.toString(progress);
            t3.setText(s3);
            s = s1 + s2 + s3 + s4;
            Taolenh(s);
        }

        @Override
        public void onStartTrackingTouch(SeekBar seekBar) {
        }

        @Override
        public void onStopTrackingTouch(SeekBar seekBar) {
            Toast.makeText(MainActivity.this, "Servo 3",Toast.LENGTH_SHORT).show();
        }
    });
    goc4.setOnSeekBarChangeListener(new SeekBar.OnSeekBarChangeListener() {
        @Override
        public void onProgressChanged(SeekBar seekBar, int progress, boolean fromUser) {
            if(progress< 40)    s4 ="040";
            else if(progress <100)    s4 = "0" + Integer.toString(progress);
            else if (progress>110)     s4 = "110";
            else s4 = Integer.toString(progress);
            t4.setText(s4);
            s = s1 + s2 + s3 + s4;
            Taolenh(s);
        }

        @Override
        public void onStartTrackingTouch(SeekBar seekBar) {
        }

        @Override
        public void onStopTrackingTouch(SeekBar seekBar) {
            Toast.makeText(MainActivity.this, "Servo 4",Toast.LENGTH_SHORT).show();
        }
    });
    }
    public void Taolenh(String lenh){
        ConnectivityManager connectivityManager =(ConnectivityManager)
                getSystemService(Context.CONNECTIVITY_SERVICE);
        NetworkInfo networkInfo = connectivityManager.getActiveNetworkInfo();
        if(networkInfo != null && networkInfo.isConnected()){
            String url = "http://192.168.1.150/";
            new Guilenserver().execute(url + lenh);
        }else{
            Toast.makeText(MainActivity.this, "Không có kết nối wifi",Toast.LENGTH_LONG).show();
        }
    }
    private class Guilenserver extends AsyncTask<String, Void, String> {

        @Override
        public String doInBackground(String... url) {
            return geturl.Guilenh(url[0]);
        }
        @Override
        public void onPostExecute(String result) {
            if(result != null){
                if(result.contains("thang")){
                    //trangthai.setText("xe tien");
                }
            }else{
                Toast.makeText(MainActivity.this, "Khong co phan hoi",Toast.LENGTH_LONG).show();
            }
        }
    }
}
