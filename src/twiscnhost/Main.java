//Main console-based host program for controlling the TwiScnDevice
//TODO: device sleep button, keep alive signal maybe, killing the trayicon on exit
package twiscnhost;

import java.util.logging.*;
import static twiscnhost.Main.opt;
import static twiscnhost.Main.twiScn;

public class Main extends javax.swing.JFrame {
    
    private static final Logger logger = Logger.getLogger(LogHandler.class.getName());
    public static final int VENDOR_ID = 0x16c0;                         
    public static final int PRODUCT_ID = 0x27d9;
    public static final int[] DEVICEIDS = {VENDOR_ID, PRODUCT_ID};
    
    public static final LogHandler log = new LogHandler();
    public static final Options opt = new Options();
    public static final PropHandler props = new PropHandler(opt);        
    public static final Gui gui = new Gui(opt);
    public static final DeviceHandler twiScn = new DeviceHandler(DEVICEIDS, opt, gui); 
    public static final TweetHandler twt = new TweetHandler(opt, twiScn);
    public static final KeepAlive keepAlive = new KeepAlive();
    public static final ShutdownHook shutdown = new ShutdownHook();

    public static void main(String[] args) {
        shutdown.attachShutDownHook();
        opt.init(props);
        gui.init(twt);
        twiScn.init();
        keepAlive.start();
        twt.init();
        
        while(true) {
            if(UsbHidComms.connected()) {                                       //keep doing this stuff while the device is still connected
                if(gui.applyDevice) {                                           //check if device settings need to be applied to the device and saved
                    props.writeAllProps(false);
                    twiScn.applyAllOptions();
                    gui.applyDevice = false;
                }
                if(gui.applyTwitter) {                                          //check if twitter settings need to be saved
                    props.writeAllProps(false);
                    gui.applyTwitter = false;
                }
                twiScn.monitorFNs();                                            //monitor the FN buttons for updates
            }
            else {
                //device got disconnected, try reconnecting it
                gui.setConnected(false);
                logger.log(Level.WARNING, "Lost connection to device.");
                twt.stopStream();
                twiScn.init();
                twt.sendLatestTweet();
            }
        }
    }
}

class ShutdownHook {                                                                         
    public void attachShutDownHook() {                                          //used to monitor when the program is told to shutdown
        Runtime.getRuntime().addShutdownHook(new Thread() {
            @Override
            public void run() {
                if(UsbHidComms.connected()) {                                   //make sure the device is connected first
                    //opt.setSleep(true);                                         //set sleep to true
                    //twiScn.applyAllOptions();                                   //tell the device to sleep/disconnect
                }
            }
        });
    }
}

class KeepAlive extends Thread {                                                //thread used to send keep alive packets to the device every 100 ms
     public void run() {
         while(true) {
            if(UsbHidComms.connected() && DeviceComms.connected) {              //make sure the device is connected before sending it keep alives
                twiScn.keepAlive();                                             //send a keep alive packet      
            }
            try {                                                               //try to wait 100 ms before sending another
                Thread.sleep(100L);
            } catch (Exception e) {}
         }
     }
 }
