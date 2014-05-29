//Main console-based host program for controlling the TwiScnDevice
//TODO: gui (tray application), check twitter connection first
package twiscnhost;

import java.util.logging.*;

public class Main {
    
    private final static Logger logger = Logger.getLogger(LogHandler.class.getName());
    public static final int VENDOR_ID = 0x16c0;                         
    public static final int PRODUCT_ID = 0x27d9;
    public static final int[] DEVICEIDS = {VENDOR_ID, PRODUCT_ID};
    public static Options devOptions;

    public static void main(String[] args) {
        LogHandler.init();
        
        devOptions = new Options();                                             //create a new instance of DeviceOptions     
        PropHandler props = new PropHandler(devOptions);        
        DeviceHandler twiScn = new DeviceHandler(DEVICEIDS, devOptions);        //create a new instance of DeviceHandler, needs the device ids and options 
        TweetHandler twt = new TweetHandler(devOptions, twiScn);
        
        while(true) {
            while(UsbHidComms.connected()) {                                    //keep doing this stuff while the device is still connected
                //check for button updates
                twiScn.monitorFNs();
            }
            
            //attempt reconnecting here
            logger.log(Level.WARNING, "Lost connection to device.");
            twt.stopStream();
            twiScn.init();
            twt.init();
        }
    }
}