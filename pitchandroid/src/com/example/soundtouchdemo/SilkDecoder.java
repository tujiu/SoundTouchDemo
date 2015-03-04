package com.example.soundtouchdemo;

import android.content.Context;
import android.media.AudioManager;

public class SilkDecoder extends Thread {

	public static final String OUT1 = "temp_";
	public static final String OUT2 = "encode_decode_";
	private String file;
	private String inputFile;
	private String outFile1;
	private String outFile2;
	private EncodeDecodeNotifier notifier;
	
	public interface EncodeDecodeNotifier {
		public void encodeDecodeFinished();
	}

	public SilkDecoder(Context context, String fileName, EncodeDecodeNotifier notifier) {
		this.file = fileName;
		this.inputFile = /*context.getFilesDir().getPath() + */"/data/data/com.example.soundtouchdemo/files/" + fileName;
		this.outFile1 = "/data/data/com.example.soundtouchdemo/files/" + OUT1 + fileName;
		this.outFile2 = "/data/data/com.example.soundtouchdemo/files/" + OUT2 + fileName;
		this.notifier = notifier;
		init(context);
	}

	private void init(Context context) {
		AudioManager audiMgr = (AudioManager) context.getSystemService(Context.AUDIO_SERVICE);
		String sEc = "NULL";
		String sNs = "NULL";
		String sGc = "NULL";
		sEc = audiMgr.getParameters("ec_supported");
		sNs = audiMgr.getParameters("ns_supported");
		sGc = audiMgr.getParameters("gc_supported");
		// objTextViewS.setText("[INFO] ec: " + sEc + ", ns: " + sNs + ", gc: "
		// + sGc);
		// tv.setText("[INFO] ec: " + sEc + ", ns: " + sNs + ", gc: " + sGc);
	}

	/* A native method that is implemented by the
     * 'helloneon' native library, which is packaged
     * with this application.
     */
    public native String  stringFromJNI(String input, String output1, String output2);

    /* this is used to load the 'helloneon' library on application
     * startup. The library has already been unpacked into
     * /data/data/com.example.neon/lib/libhelloneon.so at
     * installation time by the package manager.
     */
    static {
        System.loadLibrary("codecsilk");
    }
    
	public void run() {
		stringFromJNI(inputFile, outFile1, outFile2);
		notifier.encodeDecodeFinished();
	}
	
	public String getDecodedFile() {
		return OUT2 + file;
	}
}
