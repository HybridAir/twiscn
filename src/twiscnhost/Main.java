//Main console-based host program for controlling the TwiScnDevice
//TODO: detect handshake while running?, options saving/loading (use Properties), twitter implementation, gui (tray application), error logging?, use stadard usernames (not screennames?)
//test
package twiscnhost;

public class Main {

    public static final int VENDOR_ID = 0x16c0;                         
    public static final int PRODUCT_ID = 0x27d9;
    public static final int[] DEVICEIDS = {VENDOR_ID, PRODUCT_ID};
    public static Options devOptions;

    public static void main(String[] args) {
        devOptions = new Options();                                             //create a new instance of DeviceOptions     
        PropHandler props = new PropHandler(devOptions);        
        DeviceHandler twiScn = new DeviceHandler(DEVICEIDS, devOptions);        //create a new instance of DeviceHandler, needs the device ids and options 
        TweetHandler twt = new TweetHandler(devOptions, twiScn);
              
        while(true) {
            //check for button updates
            twiScn.monitorFNs();
        }
    }
}