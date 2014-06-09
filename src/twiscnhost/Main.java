//Main console-based host program for controlling the TwiScnDevice
//TODO: send device sleep when exit
package twiscnhost;

import java.util.logging.*;
import static twiscnhost.Main.devMon;
import static twiscnhost.Main.keepAlive;
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
    public static final KeepAlive keepAlive = new KeepAlive(twiScn);
    public static final DeviceMon devMon = new DeviceMon(gui, props, twiScn, twt, opt);
    public static final ShutdownHook shutdown = new ShutdownHook(props);

    public static void main(String[] args) {
        shutdown.attachShutDownHook();       
        opt.init(props);
        gui.init(twt);
        twiScn.init();
        keepAlive.start();
        twt.init();
        devMon.start();
        
    }
    
    void mainInit() {
        twiScn.init();
        keepAlive.start();
        devMon.start();
    }
}



class ShutdownHook {                                                            //used to monitor when the program is told to shutdown
    PropHandler props;
    
    public ShutdownHook(PropHandler props) {
        this.props = props;
    }
    
    public void attachShutDownHook() {
        Runtime.getRuntime().addShutdownHook(new Thread() {                     //this will get triggered when a program shutdown happens
            @Override
            public void run() {
                props.writeAllProps(false);                                     //save all settings
                Logger logger = Logger.getLogger(LogHandler.class.getName());
                logger.log(Level.WARNING, "Program exiting.");
                //program will exit here
            }
        });
    }
}

class KeepAlive extends Thread {                                                //thread used to send keep alive packets to the device every 750 ms            
    DeviceHandler twiScn;
    
    public KeepAlive(DeviceHandler twiScn) {
        this.twiScn = twiScn;
    }
    
    public void run() {                                                        
        while(true) {                                                           //always do this once this thread has started
            if(UsbHidComms.connected() && DeviceComms.connected) {              //make sure the device is connected before sending it keep alives
                twiScn.keepAlive();                                             //send a keep alive packet      
            }
            try {                                                               //try to wait 750 ms before sending another
                Thread.sleep(1000L);
            } catch (Exception e) {}
        }
    }
}

class DeviceMon extends Thread {                                                //thread used for main device operation and monitoring 
    Gui gui;
    PropHandler props;
    DeviceHandler twiScn;
    TweetHandler twt;
    Options opt;
    Logger logger = Logger.getLogger(LogHandler.class.getName());
    int VENDOR_ID = 0x16c0;                         
    int PRODUCT_ID = 0x27d9;
    int[] DEVICEIDS = {VENDOR_ID, PRODUCT_ID};
    
    public DeviceMon(Gui gui, PropHandler props, DeviceHandler twiScn, TweetHandler twt, Options opt) {
        this.gui = gui;
        this.props = props;
        this.twiScn = twiScn;
        this.twt = twt;
        this.opt = opt;
    }
    
    public void run() {                                                          
        while(true) {                                                           //keep doing this forever
            if(UsbHidComms.connected() && DeviceComms.connected) {              //but only this while the device is still connected
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
            else if(UsbHidComms.restartComms) {
                UsbHidComms.restartComms = false;
                try {
                    keepAlive.wait();
                } catch (InterruptedException ex) {
                    Logger.getLogger(DeviceMon.class.getName()).log(Level.SEVERE, null, ex);
                }
                DeviceHandler twiScn = new DeviceHandler(DEVICEIDS, opt, gui);
                twiScn.init();
                keepAlive.start();
            }
            else {                                                              //device got disconnected, try reconnecting it
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
    }
 }

