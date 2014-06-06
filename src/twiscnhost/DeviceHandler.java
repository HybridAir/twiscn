package twiscnhost;

import java.util.logging.Level;

public class DeviceHandler {
    
    private final static java.util.logging.Logger logger = java.util.logging.Logger.getLogger(LogHandler.class.getName());
    private Options opt;
    private DeviceComms comms;
    private Gui gui;
    private ButtonActions btnActions;
    private String hardVersion = null;
    private String firmVersion = null;
    private int[] devIDs;
    
    public DeviceHandler(int[] devIDs, Options opt, Gui gui) {        //constructor, needs the device IDs to use, the options instance, and the guihandler instance
        this.opt = opt;
        this.gui = gui;
        this.devIDs = devIDs;
        btnActions = new ButtonActions(this.opt);
        comms = new DeviceComms(new UsbHidComms(devIDs[0], devIDs[1]));   //try connecting to the device over usb, program will not continue until successful                                                     
    }
    
    public void init() {                                                        //used to reconnect the device, can be called if reconnection is necessary
        logger.log(Level.INFO, "Attempting to connect to device with VENDOR_ID: " + devIDs[0] + " and PRODUCT_ID: " + devIDs[1]);
        gui.addStatusLine("Attempting to connect to device with VENDOR_ID: " + devIDs[0] + " and PRODUCT_ID: " + devIDs[1]);
        keepAlive();
        comms.init();                                                           //tell comms to start reconnecting
        keepAlive();
        gui.setConnected(true);
        getVersion();
        keepAlive();
        applyAllOptions();
    }
    
    public void keepAlive() {
        comms.sendRaw("%");
        comms.sendRaw("=");
    }
    
    private void getVersion() {                                                 //used to set the device version
        gui.addStatusLine("Connected to device");
        hardVersion = null;                                                     //set it to null first since this should only get called when it needs updating
        firmVersion = null;
        while(hardVersion == null && firmVersion == null) {                     //while we have no versions 
            String in = comms.monitor();                                        //try to get something from the comms monitor                        
            if(in != null) {                                                    //if it returned something
                if(in.contains("$v")) {                                         //check if it's a version packet
                    hardVersion = in.substring(2, 4);
                    firmVersion = in.substring(5, 7);
                    logger.log(Level.INFO, "Got device hardware and firmware versions: " + hardVersion + ", " + firmVersion);
                    gui.setVersions(hardVersion, firmVersion);
                }
            }
        }
        
    }
    
//==============================================================================    
    
    public void applyAllOptions() {                                             //applys all options, use this after updating option settings
        comms.sendOptions(opt.formatAll());                                     //format and send the updated options to the device
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
