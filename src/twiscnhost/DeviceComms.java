//Used to communicate with the device over its own protocol

package twiscnhost;

import java.util.logging.Level;

public class DeviceComms {  
    
    private final static java.util.logging.Logger logger = java.util.logging.Logger.getLogger(LogHandler.class.getName());
    private UsbHidComms twiScnHID;
    
    public DeviceComms(UsbHidComms device) {                                    //constructor, needs the usbhid device   
        twiScnHID = device;                                                      
    }
    
    public void init() {                                                        //used to connect the device, can be called if reconnection is necessary
        tryConnecting();                                                        //try connecting to the device
        handshake();                                                            //try to send a handshake to the device
    }
    
    private void tryConnecting() {                                              //tries to connect to the device, will block the program flow until connected
        boolean connected = false;
        while(!connected) {                                                     //while we are not connected
            connected = twiScnHID.connectDevice();                              //try connecting and get the connection result
        }
        logger.log(Level.INFO, "Connection successful");                        //connection was successful at this point
    }
    
    public void sendRaw(String in) {                                            //used to send raw commands/data to the device
        twiScnHID.send(in);
    }
      
    private void handshake() {                                                  //used to establish a data connection with the device
        boolean connected = false;
        logger.log(Level.INFO, "Attempting handshake"); 
        while(!connected) {
            String in = twiScnHID.getData();                                    //try to get data from the device
            if(in != null) {                                                    //if we actually got data
                if(in.equals("`")) {                                            //check if the device is currently trying to connect (spamming "`"'s)
                    twiScnHID.send("~");                                        //send the response
                    connected = true;                                           //we are connected now                    
                    logger.log(Level.INFO, "Handshake successful");
                }
            }
        }
    }
    
    public void row0(String in) {                                               //used to send text to row 0, the username row
        twiScnHID.send("@" + in.substring(0, Math.min(in.length(), 15)));       //send the new username to the device, with a 15 char cutoff
        twiScnHID.send("=");
    }
    
    public void row1(String in) {                                               //used to send text to row 1, the tweet text row
        String[] textOut = formatText(in);                                      //get the formatted string array from formatText
        for(int i = 0; i < textOut.length; i++) {                               //for each item in the array
            twiScnHID.send(textOut[i]);                                         //send it to the device
            if(i == (textOut.length-1)) {                                       //if we just sent the last string
                twiScnHID.send("=");                                            //send the end of transfer notification
            }
        }
    }
    
    public String[] formatText(String in) {
        if (in.length() < 30) {                                                 //if the output will fit in one transfer
            String[] out = new String[1];                                       //just need the first string out of the array
            out[0] = "!" + in;                                                  //add the tweet text notifier to the beginning
            return out;                                                         //send the formatted string array out to get sent probably
        }
        else {                                                                  //if the output will not fit in one transfer
            in = "!" + in;                                                      //add the tweet text notifier to the beginning
            String[] out = (in.split("(?<=\\G.{30})"));;                        //split the tweet text into strings no larger than 30 chars, put them into an array
            return out;                                                         //send the formatted string array out to get sent probably
        }
    }
        
    public String monitor() {                                                   //monitor the device for any incoming data
        String input = twiScnHID.getData();
        if (input != null) {                                                    //if we got data
            return input;                                                       //return the data we got
        }
        else
            return null;
    }
    
    public void sendOptions(String[] values) {                                  //used to send options to the device
        for(int i = 0;i <= 6;i++) {                                             //only 4 sendable option groups so far
            switch(i) {
                case 0:                                                         //send the backlight brightness settings
                    twiScnHID.send("$b" + values[0]);
                    break;
                case 1:                                                         //send the backlight color settings
                    twiScnHID.send("$c" + values[1]);
                    break;
                case 2:                                                         //send the tweetblink settings
                    twiScnHID.send("$d" + values[2] + values[3] + values[4]);
                    break;
                case 3:                                                         //send the rainbow mode settings
                    twiScnHID.send("$e" + values[5] + values[6]);
                    break;
                case 4:                                                         //send the read time setting
                    twiScnHID.send("$f" + values[7]);
                    break;
                case 5:                                                         //send the previous tweet setting
                    twiScnHID.send("$g" + values[8]);
                    break;
                case 6:                                                         //send the scroll toggle setting
                    twiScnHID.send("$h" + values[9]);
                    break;
                default:
                    break;
            }
            twiScnHID.send("=");                                                //send the end of transfer notification after each option transfer                
        }
    }
}
