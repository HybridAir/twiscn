package twiscnhost;

public class DeviceHandler {
    
    private Options options;
    private DeviceComms comms;
    
    public DeviceHandler(int[] deviceIDs, Options options) {                    //constructor, needs the device IDs to use and the options instance
        this.options = options;
        System.out.println("Attempting to connect to device with VENDOR_ID: " + deviceIDs[0] + " and PRODUCT_ID: " + deviceIDs[1]);
        comms = new DeviceComms(new UsbHidComms(deviceIDs[0], deviceIDs[1]));   //try connecting to the device over usb, program will not continue until successful
        applyAllOptions();   
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
            System.out.print(in);
            if(in.equals("1")) {                                                
                try {
                        Thread.sleep(500L);                                    //2 seconds should be enough time for the device to get ready
                    } catch (Exception e) {}
                comms.sendRaw("$g");
                comms.sendRaw("=");
                System.out.println("sent");
            }
        }
    }
}
