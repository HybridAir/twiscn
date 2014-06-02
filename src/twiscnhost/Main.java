//Main console-based host program for controlling the TwiScnDevice
//TODO: gui (tray application), check twitter connection first, device sleep button
package twiscnhost;

import java.util.logging.*;

public class Main extends javax.swing.JFrame {
    
    private final static Logger logger = Logger.getLogger(LogHandler.class.getName());
    public static final int VENDOR_ID = 0x16c0;                         
    public static final int PRODUCT_ID = 0x27d9;
    public static final int[] DEVICEIDS = {VENDOR_ID, PRODUCT_ID};
    
    public static Options devOptions = new Options();
    public static PropHandler props = new PropHandler(devOptions);        
    public static Gui gui = new Gui(devOptions);
    public static DeviceHandler twiScn = new DeviceHandler(DEVICEIDS, devOptions, gui);   //create a new instance of DeviceHandler, needs the device ids and options 
    public static TweetHandler twt = new TweetHandler(devOptions, twiScn);

    public static void main(String[] args) {
        LogHandler.init();                                                      //start logging
        gui.setTwt(twt);
        twiScn.init();
        twt.init();
        
        while(true) {
            while(UsbHidComms.connected()) {                                    //keep doing this stuff while the device is still connected
                //check for button updates
                if(gui.applyOptions) {
                    twiScn.applyAllOptions();
                    gui.applyOptions = false;
                }
                twiScn.monitorFNs();
            }
                
            
            //attempt reconnecting here
            gui.setConnected(false);
            logger.log(Level.WARNING, "Lost connection to device.");
            twt.stopStream();
            twiScn.init();
            twt.init();
        }
    }
}