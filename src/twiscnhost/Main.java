//Main console-based host program for controlling the TwiScnDevice
//TODO: move handshake check to the cxheckalive thread
package twiscnhost;

import java.util.logging.*;
import static twiscnhost.Main.gui;
import static twiscnhost.Main.opt;
import static twiscnhost.Main.props;
import static twiscnhost.Main.twiScn;
import static twiscnhost.Main.twt;

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
    public static final DeviceMon devMon = new DeviceMon();
    public static final ShutdownHook shutdown = new ShutdownHook();

    public static void main(String[] args) {
        shutdown.attachShutDownHook();
        opt.init(props);
        gui.init(twt);
        twiScn.init();
        keepAlive.start();
        twt.init();
        devMon.start();
        
//        while(true) {
//            if(UsbHidComms.connected() && DeviceComms.connected) {              //keep doing this stuff while the device is still connected
//                if(gui.applyDevice) {                                           //check if device settings need to be applied to the device and saved
//                    props.writeAllProps(false);
//                    twiScn.applyAllOptions();
//                    gui.applyDevice = false;
//                }
//                if(gui.applyTwitter) {                                          //check if twitter settings need to be saved
//                    props.writeAllProps(false);
//                    gui.applyTwitter = false;
//                }
//                twiScn.monitorDevice();                                         //monitor the FN buttons for updates
//            }
//            else {
//                //device got disconnected, try reconnecting it
//                gui.setConnected(false);
//                gui.addStatusLine("Lost connection to device.");
//                logger.log(Level.WARNING, "Lost connection to device.");
//                twt.stopStream();
//                twiScn.init();
//                gui.setDeviceDefaults();
//                gui.applyDevice = true;
//                gui.applyTwitter = true;
//                twt.sendLatestTweet();
//                twt.startStream();
//            }
//        }
    }
}

class ShutdownHook {                                                                         
    public void attachShutDownHook() {                                          //used to monitor when the program is told to shutdown
        Runtime.getRuntime().addShutdownHook(new Thread() {
            @Override
            public void run() {
                Logger logger = Logger.getLogger(LogHandler.class.getName());
                logger.log(Level.WARNING, "Program exiting.");
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
                Thread.sleep(750L);
            } catch (Exception e) {}
         }
     }
 }

class DeviceMon extends Thread {                                                //thread used to send keep alive packets to the device every 100 ms
     public void run() {
         //while(true) {
             Logger logger = Logger.getLogger(LogHandler.class.getName());
                    while(true) {
            if(UsbHidComms.connected() && DeviceComms.connected) {              //keep doing this stuff while the device is still connected
                if(gui.applyDevice) {                                           //check if device settings need to be applied to the device and saved
                    props.writeAllProps(false);
                    twiScn.applyAllOptions();
                    gui.applyDevice = false;
                }
                if(gui.applyTwitter) {                                          //check if twitter settings need to be saved
                    props.writeAllProps(false);
                    gui.applyTwitter = false;
                }
                twiScn.monitorDevice();                                         //monitor the FN buttons for updates
            }
            else {
                //device got disconnected, try reconnecting it
                gui.setConnected(false);
                gui.addStatusLine("Lost connection to device.");
                logger.log(Level.WARNING, "Lost connection to device.");
                twt.stopStream();
                twiScn.init();
                gui.setDeviceDefaults();
                gui.applyDevice = true;
                gui.applyTwitter = true;
                twt.sendLatestTweet();
                twt.startStream();
            }
        }
         //}
     }
 }
