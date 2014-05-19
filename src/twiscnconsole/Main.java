package twiscnconsole;

import java.awt.Color;
import java.util.*;

public class Main {

    public static final int VENDOR_ID = 0x16c0;                         
    public static final int PRODUCT_ID = 0x27d9;
    public static final int[] DEVICEIDS = {VENDOR_ID, PRODUCT_ID};
    public static TwiScnOptions twiOptions;
//    public static final int BRIGHTNESS = 20;
//    public static final boolean BLINK = false;
//    public static final int BLINKSPD = 100;
//    public static final String FN1_OP = "brightness";
//    public static final String FN2_OP = "rainbow";
//    public static final Object[] OPTIONS = new Object[]{FN1_OP, FN2_OP, BRIGHTNESS, Color.GREEN};

    public static void main(String[] args) {
        twiOptions = new TwiScnOptions();
        TwiScnHandler twiScn = new TwiScnHandler(DEVICEIDS, twiOptions);        
               
        String in = "Lorem ipsum dolor sit amet, consectetuer adipiscing elit. Aenean commodo ligula eget dolor. Aenean massa. Cum sociis natoque penatibus et ma";
        twiScn.newTweet("user:", in);
        
//        try {
//                        Thread.sleep(5000L);                                    //2 seconds should be enough time for the device to get ready
//                    } catch (Exception e) {}

//        twiOptions.setLCDColor(Color.GREEN);
//        twiScn.applyAllOptions();
        while(true) {
            twiScn.monitorFNs();
        }
    }
}












