//Main console-based host program for controlling the TwiScnDevice
//TODO: device sleep button
package twiscnhost;

import java.util.logging.*;

public class Main extends javax.swing.JFrame {
    
    private final static Logger logger = Logger.getLogger(LogHandler.class.getName());
    public static final int VENDOR_ID = 0x16c0;                         
    public static final int PRODUCT_ID = 0x27d9;
    public static final int[] DEVICEIDS = {VENDOR_ID, PRODUCT_ID};
    
    public static final Options opt = new Options();
    public static final PropHandler props = new PropHandler(opt);        
    public static final Gui gui = new Gui(opt);
    public static final DeviceHandler twiScn = new DeviceHandler(DEVICEIDS, opt, gui);   //create a new instance of DeviceHandler, needs the device ids and options 
    public static final TweetHandler twt = new TweetHandler(opt, twiScn);

    public static void main(String[] args) {
        LogHandler.init();                                                      //start logging
        opt.init(props);
        gui.init(twt);
        twiScn.init();
        twt.init();
        
        while(true) {
            while(UsbHidComms.connected()) {                                    //keep doing this stuff while the device is still connected
                //check for button updates
                if(gui.applyTwitter) {
                    props.writeAllProps(false);
                    gui.applyTwitter = false;
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