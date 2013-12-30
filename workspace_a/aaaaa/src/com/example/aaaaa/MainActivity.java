package com.example.aaaaa;

import java.io.File;
import java.io.FileReader;
import java.io.IOException;
import java.nio.ByteBuffer;
import java.nio.ByteOrder;
import java.security.AlgorithmParameters;
import java.security.InvalidKeyException;
import java.security.KeyFactory;
import java.security.KeyPair;
import java.security.NoSuchAlgorithmException;
import java.security.PublicKey;
import java.security.interfaces.RSAPublicKey;
import java.security.spec.X509EncodedKeySpec;
import java.text.SimpleDateFormat;
import java.util.Hashtable;
import java.util.List;
import java.util.Locale;
import java.util.TimeZone;

import javax.crypto.*;
import javax.crypto.spec.IvParameterSpec;

import org.bouncycastle.asn1.x509.SubjectPublicKeyInfo;
import org.bouncycastle.crypto.AsymmetricBlockCipher;
import org.bouncycastle.crypto.digests.SHA1Digest;
import org.bouncycastle.crypto.encodings.OAEPEncoding;
import org.bouncycastle.crypto.engines.RSAEngine;
import org.bouncycastle.crypto.params.AsymmetricKeyParameter;
import org.bouncycastle.crypto.util.PublicKeyFactory;
import org.bouncycastle.openssl.PEMKeyPair;
import org.bouncycastle.openssl.PEMParser;
import org.bouncycastle.openssl.jcajce.JcaPEMKeyConverter;

import android.os.Build;
import android.os.Bundle;
import android.os.Environment;
import android.annotation.SuppressLint;
import android.annotation.TargetApi;
import android.app.Activity;
import android.app.AlertDialog;
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
		rnd = new java.util.Random();

		test3(null);
		try {
			kgen = KeyGenerator.getInstance("AES");
		} catch (NoSuchAlgorithmException e) {
			e.printStackTrace();
		}
		kgen.init(256);
		android.widget.EditText txt2 = (android.widget.EditText) this
				.findViewById(R.id.editText2);
		txt2.setText(Environment.getExternalStorageDirectory().getPath()
				+ "/public.pem");
		test6(null);
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
	KeyGenerator kgen;
	SubjectPublicKeyInfo pk;

	public void alert(String s) {
		(new AlertDialog.Builder(this)).setMessage(s)
				.setPositiveButton("OK", null).show();

	}

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

	public SubjectPublicKeyInfo test5(String path) throws java.lang.Exception {
		FileReader fileReader = new FileReader(path);
		PEMParser p = new PEMParser(fileReader);
		try {
			// JcaPEMKeyConverter converter = new JcaPEMKeyConverter()
			// .setProvider("BC");
			// return converter
			// .getPublicKey((SubjectPublicKeyInfo) p.readObject());
			return (SubjectPublicKeyInfo) p.readObject();
		} finally {
			try {
				fileReader.close();
			} catch (java.lang.Exception ex) {
			}
		}
	}

	public void test6(View v) {
		android.widget.EditText txt = (android.widget.EditText) this
				.findViewById(R.id.editText2);
		try {
			pk = test5(txt.getText().toString());
		} catch (Exception e) {
			alert(e.toString());
		}
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
		TextView t = (TextView) this.findViewById(R.id.textView2);
		t.setText(Integer.toString(asdfg) + "/" + Integer.toString(zxcvb));
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

	public void dosave(byte[] data) throws java.lang.Exception {
		java.util.Date d = new java.util.Date();
		SimpleDateFormat dt = new SimpleDateFormat("yyyy_MM_dd_HH_mm_ss",
				Locale.getDefault());
		dt.setTimeZone(TimeZone.getTimeZone("UTC"));
		String s = dt.format(d);

		SecretKey key1;
		synchronized (kgen) {
			key1 = kgen.generateKey();
			s += "_" + Integer.toString(rnd.nextInt(10000));
		}
		byte[] aeskey = key1.getEncoded();
		/* Encrypt the message. */
		Cipher cipher = Cipher.getInstance("AES/CBC/PKCS5Padding");
		cipher.init(Cipher.ENCRYPT_MODE, key1);
		byte[] iv = cipher.getIV();
		byte[] ciphertext = cipher.doFinal(data);

		AsymmetricKeyParameter param = PublicKeyFactory.createKey(pk);
		AsymmetricBlockCipher rsa = new OAEPEncoding(new RSAEngine(),
				new SHA1Digest());
		rsa.init(true, param);

		byte[] enckey = rsa.processBlock(aeskey, 0, aeskey.length);

		java.io.FileOutputStream fs = new java.io.FileOutputStream(this.dir()
				+ "/" + s + ".asdfg", false);
		fs.write(ByteBuffer.allocate(4).order(ByteOrder.BIG_ENDIAN)
				.putInt(enckey.length).array());
		fs.write(ByteBuffer.allocate(4).order(ByteOrder.BIG_ENDIAN)
				.putInt(iv.length).array());
		fs.write(enckey);
		fs.write(iv);
		fs.write(ciphertext);
		fs.close();
	}

	public void save(final byte[] data) throws java.lang.Exception {
		Thread th = new Thread(new Runnable() {
			@Override
			public void run() {
				try {
					dosave(data);
				} catch (Exception e) {
					e.printStackTrace();
				}
			}
		});
		th.setPriority(Thread.MIN_PRIORITY);
		th.start();
	}

	@Override
	public void onPictureTaken(byte[] data, Camera c) {
		Log.e("error", "data received");
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
