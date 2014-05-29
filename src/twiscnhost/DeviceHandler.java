package twiscnhost;

import java.util.logging.Level;

public class DeviceHandler {
    
    private final static java.util.logging.Logger logger = java.util.logging.Logger.getLogger(LogHandler.class.getName());
    private Options options;
    private DeviceComms comms;
    private ButtonActions btnActions;
    private String version = null;
    
    public DeviceHandler(int[] deviceIDs, Options opt) {                        //constructor, needs the device IDs to use and the options instance
        this.options = opt;
        btnActions = new ButtonActions(this.options);
        logger.log(Level.INFO, "Attempting to connect to device with VENDOR_ID: " + deviceIDs[0] + " and PRODUCT_ID: " + deviceIDs[1]);
        comms = new DeviceComms(new UsbHidComms(deviceIDs[0], deviceIDs[1]));   //try connecting to the device over usb, program will not continue until successful
        getVersion();
        applyAllOptions();                                                      
    }
    
    public void init() {                                                        //used to reconnect the device, can be called if reconnection is necessary
        logger.log(Level.INFO, "Attempting to reconnect to device.");
        comms.init();                                                           //tell comms to start reconnecting
        getVersion();
        applyAllOptions();
    }
    
    private void getVersion() {                                                 //used to set the device version
        version = null;                                                         //set it to null first since this should only get called when it needs updating
        while(version == null) {                                                //while we have no version 
            String in = comms.monitor();                                        //try to get something from the comms monitor                        
            if(in != null) {                                                    //if it returned something
                if(in.contains("$v")) {                                         //check if it's a version packet
                version = in.substring(2, 4);                                   //get the version out
                logger.log(Level.INFO, "Got device version: " + version);
                }
            }
        }
        
    }
    
//==============================================================================    
    
    public void applyAllOptions() {                                             //applys all options, use this after updating option settings
        comms.sendOptions(options.formatAll());                                 //format and send the updated options to the device
    }
    
    public void newTweet(String user, String text) {                            //used to send a new tweet to the device, needs a user and tweet text
        comms.row0(user);
        comms.row1(text);
    }
         
    public void monitorFNs() {                                                  //used to monitor the device's fn buttons, needs to be ran continuously
        String in = comms.monitor();
        if(in != null) {
            logger.log(Level.FINE, "Got " + in + "from device comms");
            if(in.equals("1")) {                                                
                try {
                        Thread.sleep(500L);                                     //half a second should be enough time for the device to get ready
                } catch (Exception e) {}
                btnActions.fn1();
                applyAllOptions();
            }
            if(in.equals("2")) {                                                
                try {
                        Thread.sleep(500L);                                     //half a second should be enough time for the device to get ready
                } catch (Exception e) {}
                btnActions.fn2();
                applyAllOptions();
            }
        }
        in = null;
    }
}
