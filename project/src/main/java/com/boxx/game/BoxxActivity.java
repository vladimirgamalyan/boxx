package com.boxx.game;

import android.app.NativeActivity;
import android.os.Bundle;

import android.content.Intent;

import com.google.zxing.client.android.Intents;
import com.google.zxing.integration.android.IntentIntegrator;
import com.google.zxing.integration.android.IntentResult;

import android.widget.Toast;

public class BoxxActivity extends NativeActivity {
	private static BoxxActivity activity;
	
    @Override
    protected void onCreate(Bundle savedInstanceState) {
		super.onCreate(savedInstanceState);
		activity = this;
    }
	
	public void getQrCode() {
        IntentIntegrator integrator = new IntentIntegrator(this);
        // integrator.setDesiredBarcodeFormats(IntentIntegrator.ONE_D_CODE_TYPES);
        integrator.setPrompt("Scan a QR Code");
        integrator.setCameraId(0);
        integrator.setBeepEnabled(false);
        integrator.setBarcodeImageEnabled(true);
        integrator.initiateScan();			
	}
	
    public static void runQrScanner() {
		activity.getQrCode();
    }
	
	//public static native void qrCodeReady(String result);
	
    @Override
    protected void onActivityResult(int requestCode, int resultCode, Intent data) {
		IntentResult result = IntentIntegrator.parseActivityResult(requestCode, resultCode, data);
		if (result != null) {
			if (result.getContents() == null) {
				//qrCodeReady("");
			} else {
				//qrCodeReady(result.getContents());
			}
		} else {
			super.onActivityResult(requestCode, resultCode, data);
		}
	}

}
