//Used to communicate with the twitterscreen

package twiscnconsole;

import java.util.*;
import java.text.*;

public class TwiScnComms {
    
    private DeviceComms twitterScnHID;
    
    public TwiScnComms(DeviceComms device) {   
        twitterScnHID = device; 
        twitterScnHID.connectDevice();
        handshake();
    }
    
    public void sendRaw(String in) {
        twitterScnHID.send(in);
    }
      
    private void handshake() {
        boolean connected = false;
        System.out.println("Attempting handshake");
        while(!connected) {
            String in = twitterScnHID.getData();                                //try to get data from the device
            if(in != null) {                                                    //if we actually got data
                if(in.equals("`")) {                                            //check if the device is currently trying to connect
                    twitterScnHID.send("~");                                    //send the response
                    connected = true;                                           //time to break out of this loop
                    
                    try {
                        Thread.sleep(2000L);                                    //2 seconds should be enough time for the device to get ready
                    } catch (Exception e) {}                                    //you never know
                    System.out.println("Handshake successful");
                }
            }
        }
    }
    
    public void row0(String in) {
        twitterScnHID.send("@" + in.substring(0, Math.min(in.length(), 15)));
        twitterScnHID.send("=");
    }
    
    public void row1(String in) {
        String[] textOut = formatText(in);
        for(int i = 0; i < textOut.length; i++) {
            twitterScnHID.send(textOut[i]);
            if(i == (textOut.length-1)) {
                twitterScnHID.send("=");
            }
//            else {
//                twitterScnHID.send("+");
//            }
        }
    }
    
    public String[] formatText(String in) {
        if (in.length() > 140) {
                System.err.println("That tweet is greater than 140 chars?");
                return null;
        }
        else {
            Date time = new Date();
            SimpleDateFormat ft = new SimpleDateFormat (" hh:mm a");
            in += ft.format(time);
            if (in.length() < 30) {
                String[] out = new String[0];
                out[0] = "!" + in;
                return out;
            }
            else {
                in = "!" + in;
                String[] out = (in.split("(?<=\\G.{30})"));;
                return out;
            }
        }
    }
        
    public String monitor() {
        String input = twitterScnHID.getData();
        if (input != null) {
            return input;
        }
        else
            return null;
    }
    
//    private void processFN(String btn) {
//        if(btn.contains("FN1")) {
//            brightness = switchBrightness();
//        }
//            b++;
//            if(b == 5) {
//                b = 0;
//            }
//            switch(b) {
//                case 0:
//                    brightness = "0";
//                    break;
//                case 1:
//                    brightness = "64";
//                    break;
//                case 2:
//                    brightness = "128";
//                    break;
//                case 3:
//                    brightness = "192";
//                    break;
//                case 4:
//                    brightness = "255";
//                    break;
//                default:
//                    break;
//            }
//            try {
//                        Thread.sleep(50L);                                    //2 seconds should be enough time for the device to get ready
//                    } catch (Exception e) {}
//            applySettings();
//            
//        }
    
    public void sendOptions(String[] values) {
        for(int i = 0;i <= 3;i++) {
            switch(i) {
                case 0:
                    System.out.println("$b" + values[0]);
                    twitterScnHID.send("$b" + values[0]);
                    break;
                case 1:
                    System.out.println("$c" + values[1]);
                    twitterScnHID.send("$c" + values[1]);
                    break;
                case 2:
                    System.out.println("$d" + values[2] + values[3] + values[4]);
                    twitterScnHID.send("$d" + values[2] + values[3] + values[4]);
//                    try {
//                        Thread.sleep(1000L);                                    //2 seconds should be enough time for the device to get ready
//                    } catch (Exception e) {}
                    break;
                case 3:
                    System.out.println("$e" + values[5] + values[6]);
                    twitterScnHID.send("$e" + values[5] + values[6]);
                    break;
                default:
                    break;
            }
            twitterScnHID.send("=");
                    
        }
    }
}
