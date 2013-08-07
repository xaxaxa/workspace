package com.example.aaaaa;

import java.io.File;
import java.io.IOException;
import java.text.SimpleDateFormat;
import java.util.Hashtable;
import java.util.List;
import java.util.Locale;
import java.util.TimeZone;

import android.os.Build;
import android.os.Bundle;
import android.os.Environment;
import android.annotation.SuppressLint;
import android.annotation.TargetApi;
import android.app.Activity;
import android.content.Context;
import android.content.res.Configuration;
import android.view.Display;
import android.view.KeyEvent;
import android.view.Menu;
import android.view.OrientationEventListener;
import android.view.Surface;
import android.view.View;
import android.widget.*;
import android.graphics.ImageFormat;
import android.graphics.PixelFormat;
import android.hardware.Camera;
import android.hardware.Camera.CameraInfo;
import android.hardware.Camera.Size;
import android.hardware.SensorManager;
import android.util.Log;
import android.view.WindowManager;

public class MainActivity extends Activity implements Camera.PictureCallback,
		Camera.AutoFocusCallback {
	static {
		System.loadLibrary("ndk1");
	}

	public native void helloLog(String logThis);

	public java.util.Hashtable<Object, Integer> ht;

	@Override
	protected void onCreate(Bundle savedInstanceState) {
		super.onCreate(savedInstanceState);
		setContentView(R.layout.activity_main);

		int n = android.hardware.Camera.getNumberOfCameras();

		RadioGroup rg = (RadioGroup) this.findViewById(R.id.radioGroup1);
		ht = new Hashtable<Object, Integer>();
		for (int i = 0; i < n; i++) {
			RadioButton rb = new RadioButton(this
					.findViewById(R.id.radioGroup1).getContext());
			rb.setText(String.valueOf(i));
			ht.put(rb, new Integer(i));
			rg.addView(rb);
			if (i == 0)
				rb.setChecked(true);
		}
		helloLog("This will log to LogCat via the native call.");
		rnd = new java.util.Random();

		test3(null);
	}

	@Override
	public void onConfigurationChanged(Configuration newConfig) {
		super.onConfigurationChanged(newConfig);
		// Log.e("orientation", Integer.toString(newConfig.orientation));
	}

	@Override
	public boolean onCreateOptionsMenu(Menu menu) {
		// Inflate the menu; this adds items to the action bar if it is present.
		getMenuInflater().inflate(R.menu.main, menu);
		return true;
	}

	@Override
	public boolean dispatchKeyEvent(KeyEvent event) {
		int action = event.getAction();
		int keyCode = event.getKeyCode();
		switch (keyCode) {
		case KeyEvent.KEYCODE_VOLUME_UP:
			if (action == KeyEvent.ACTION_DOWN) {
				start();
			}
			return true;
		case KeyEvent.KEYCODE_VOLUME_DOWN:
			if (action == KeyEvent.ACTION_DOWN) {
				start();
			}
			return true;
		default:
			return super.dispatchKeyEvent(event);
		}
	}

	java.util.Random rnd;
	android.hardware.Camera c;
	int asdfg;
	int zxcvb;
	int n = 3;
	boolean ddddd = false;

	public void test1(View v) {
		start();
	}

	public void test2(View v) {
		android.widget.EditText txt = (android.widget.EditText) this
				.findViewById(R.id.editText1);
		txt.setText("/storage/extSdCard/zxcvb");
	}
	public void test3(View v) {
		String dir = Environment.getExternalStorageDirectory().getPath()
			+ "/zxcvb";
		android.widget.EditText txt = (android.widget.EditText) this
			.findViewById(R.id.editText1);
		txt.setText(dir);
	}

	public void start() {
		if (ddddd) {
			zxcvb = asdfg + 3;
			updateLabel();
			return;
		}
		ddddd = true;
		asdfg = 0;
		zxcvb = n;
		updateLabel();
		prepare();
		CheckBox chk = (CheckBox) this.findViewById(R.id.checkBox1);
		if (chk.isChecked()) {
			c.autoFocus(this);
		} else
			perform();
	}

	@Override
	public void onAutoFocus(boolean b, Camera c) {
		perform();
	}

	public int getSizeArea(Size s) {
		return s.width * s.height;
	}

	String dir() {
		return ((android.widget.EditText) this.findViewById(R.id.editText1))
				.getText().toString();
	}
	public void updateLabel() {
		TextView t=(TextView)this.findViewById(R.id.textView2);
		t.setText(Integer.toString(asdfg)+"/"+Integer.toString(zxcvb));
	}

	public void prepare() {
		String dir = this.dir();
		File tmp = new File(dir);
		if (!tmp.exists())
			tmp.mkdirs();

		android.widget.Button b = (android.widget.Button) this
				.findViewById(R.id.button1);
		RadioGroup rg = (RadioGroup) this.findViewById(R.id.radioGroup1);
		int id = ht.get(this.findViewById(rg.getCheckedRadioButtonId()))
				.intValue();
		Log.w("aaaaa", Integer.toString(id));

		CameraInfo ci = new CameraInfo();
		Camera.getCameraInfo(id, ci);

		c = android.hardware.Camera.open(id);
		for (int i = 0; i < 2; i++) {
			Camera.Parameters p = c.getParameters();
			p.setJpegQuality(100);
			p.setFlashMode(Camera.Parameters.FLASH_MODE_OFF);
			List<Size> sizes = p.getSupportedPictureSizes();
			Size lastSize = sizes.get(0);
			for (int ii = 1; ii < sizes.size(); ii++) {
				if (getSizeArea(sizes.get(ii)) > getSizeArea(lastSize))
					lastSize = sizes.get(ii);
			}
			p.setPictureSize(lastSize.width, lastSize.height);

			// p.set("rawsave-mode", "2");
			// p.set("rawfname", "/mnt/sdcard/aaaaa.raw");
			if (i == 0)
				p.setFocusMode(Camera.Parameters.FOCUS_MODE_CONTINUOUS_VIDEO);
			try {
				c.setParameters(p);
				break;
			} catch (java.lang.Exception ex) {
			}
		}
		c.startPreview();
	}

	public void perform() {
		if (asdfg < zxcvb)
			c.takePicture(null, null, this);
		else {
			ddddd = false;
			c.release();
		}
	}

	public void save(byte[] data) throws IOException {
		java.util.Date d = new java.util.Date();
		SimpleDateFormat dt = new SimpleDateFormat("yyyy_MM_dd_HH_mm_ss",
				Locale.getDefault());
		dt.setTimeZone(TimeZone.getTimeZone("UTC"));
		String s = dt.format(d);

		s += "_" + Integer.toString(rnd.nextInt(10000));
		java.io.FileOutputStream fs = new java.io.FileOutputStream(this.dir()
				+ "/" + s + ".jpg", false);
		fs.write(data);
		fs.flush();
		fs.close();
	}

	@Override
	public void onPictureTaken(byte[] data, Camera c) {
		Log.e("error", "data: " + (data == null ? "null" : data.toString()));
		if (data == null)
			return;
		try {
			save(data);
		} catch (java.lang.Exception ex) {
			Log.e("EXCEPTION", ex.getMessage());
		}
		asdfg++;
		updateLabel();
		perform();
	}

}
