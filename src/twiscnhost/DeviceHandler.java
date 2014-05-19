package twiscnhost;

import java.awt.Color;

public class DeviceHandler {
    
    private DeviceOptions options;
    private DeviceComms comms;
    
    public DeviceHandler(int[] deviceIDs, DeviceOptions options) {
        this.options = options;
        comms = new DeviceComms(new UsbHidComms(deviceIDs[0], deviceIDs[1]));   //establish a data connection with the twitterscreen
        applyAllOptions();   
       // options.setLCDColor(Color.GREEN);
        //options.setBrightness(100);
        //options.setBlinkState(true);
        //options.setLCDColor(Color.red);
        //options.setBlinkSpd(100);
       // comms.sendRaw("$c" + options.getLCDColor());
      //  comms.sendRaw("=");
       // System.out.println("$d" + options.getBlinkState() + options.getBlinkSpd() + options.getBlinkColor());
      //  comms.sendRaw("$d" + options.getBlinkState() + options.getBlinkSpd() + options.getBlinkColor());
        //comms.sendRaw("=");
    }
    
    public void applyAllOptions() {
        comms.sendOptions(options.formatAll());
    }
    
    public void newTweet(String user, String text) {
        comms.row0(user);
//        try {
//                        Thread.sleep(1000L);                                    //2 seconds should be enough time for the device to get ready
//                    } catch (Exception e) {}
        comms.row1(text);
    }
         
    public void monitorFNs() {
        String in = comms.monitor();
        if(in != null) {
            System.out.print(in);
            if(in.equals("FN1")) {
                options.fnHandler(1);
                try {
                        Thread.sleep(50L);                                    //2 seconds should be enough time for the device to get ready
                    } catch (Exception e) {}
                applyAllOptions();
            }
            else if (in.equals("FN2")) {
                options.fnHandler(2);
                applyAllOptions();
            }
        }
    }
}
