package com.larryhou.lz4test;

import androidx.annotation.RequiresApi;
import androidx.appcompat.app.AppCompatActivity;

import android.annotation.SuppressLint;
import android.os.Build;
import android.os.Bundle;
import android.view.View;
import android.widget.Button;
import android.widget.ProgressBar;
import android.widget.TextView;

import java.io.File;
import java.util.Calendar;
import java.util.Timer;
import java.util.TimerTask;

public class MainActivity extends AppCompatActivity implements View.OnClickListener {

    private static final String TAG = "lz4test";

    // Used to load the 'native-lib' library on application startup.
    static {
        System.loadLibrary("lz4test");
        System.loadLibrary("native-lib");
    }

    TextView _txt_elapse;
    Button _btn_progress;
    ProgressBar _progressbar;
    Timer _timer;
    Thread _thread;


    @SuppressLint("DefaultLocale")
    @Override
    protected void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        setContentView(R.layout.activity_main);

        _txt_elapse = findViewById(R.id.txt_elapse);
        _txt_elapse.setText(String.format("%.3f", 0.0f));

        _btn_progress = findViewById(R.id.btn_decompress);
        _btn_progress.setOnClickListener(this);

        _progressbar = findViewById(R.id.progressbar);
    }

    /**
     * A native method that is implemented by the 'native-lib' native library,
     * which is packaged with this application.
     */
    public native String jnistring();
    public native boolean jnidecompress(String input, String output);
    public native double jniprogress();

    @Override
    public void onClick(View view)
    {
        int sender = view.getId();
        if (sender == R.id.btn_decompress)
        {
            File external = getExternalFilesDir("");
            final String input = external + "/sample.lz4";
            final String output = external + "/sample.lz4.dat";

            if (_timer != null) { _timer.cancel(); }

            final int[] success = new int[]{-1};
            final long[] timestamp = {0};

            _progressbar.setProgress(0);

            _timer = new Timer();
            _timer.schedule(new TimerTask()
            {
                @RequiresApi(api = Build.VERSION_CODES.N)
                @SuppressLint("DefaultLocale")
                @Override
                public void run()
                {
                    runOnUiThread(new Runnable()
                    {
                        @Override
                        public void run()
                        {
                            if (success[0] != -1)
                            {
                                _timer.cancel();
                            }

                            double progress = jniprogress();
                            if (timestamp[0] != 0)
                            {
                                long elapse = Calendar.getInstance().getTimeInMillis() - timestamp[0];

                                _txt_elapse.setText(String.format("%.3f", (double)elapse / 1000.0));
                                int value = (int)(100 * progress);
                                if (value != _progressbar.getProgress())
                                {
                                    _progressbar.setProgress(value, true);
                                }

                            }
                        }
                    });
                }
            }, 0, 33);

            if (_thread != null) { _thread.interrupt(); }
            _thread = new Thread(new TimerTask()
            {
                @Override
                public void run()
                {
                    if (timestamp[0] == 0) { timestamp[0] = Calendar.getInstance().getTimeInMillis(); }
                    success[0] = jnidecompress(input, output)? 1 : 0;
                }
            });
            _thread.start();
        }
    }
}
