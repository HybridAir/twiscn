//Main console-based host program for controlling the TwiScnDevice
//TODO: add exception catching for usb exceptions, options saving/loading, twitter implementation, gui (tray application maybe?)
package twiscnhost;

import java.awt.Color;
import java.util.*;

public class Main {

    public static final int VENDOR_ID = 0x16c0;                         
    public static final int PRODUCT_ID = 0x27d9;
    public static final int[] DEVICEIDS = {VENDOR_ID, PRODUCT_ID};
    public static DeviceOptions twiOptions;

    public static void main(String[] args) {
        twiOptions = new DeviceOptions();                                       //create a new instance of options
        DeviceHandler twiScn = new DeviceHandler(DEVICEIDS, twiOptions);        
               
        String tweet = "Lorem ipsum dolor sit amet, consectetuer adipiscing elit. Aenean commodo ligula eget dolor. Aenean massa. Cum sociis natoque penatibus et ma";
        String user = "Blake:";
        twiScn.newTweet(user, tweet);
        
        while(true) {
            twiScn.monitorFNs();
        }
    }
}












