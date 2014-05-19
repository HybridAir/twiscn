//Used for handling USB HID comms, pretty generic

package twiscnconsole;

import com.codeminders.hidapi.HIDDeviceInfo;
import com.codeminders.hidapi.HIDManager;
import com.codeminders.hidapi.HIDDevice;
import java.io.IOException;
import java.util.List;
import java.util.ArrayList;

public class DeviceComms {
    
    private int vndr;
    private int pdct;
    private static HIDDevice device = null;                                      
    private static Boolean device_initialized = false;
    
    public DeviceComms(int vndr, int pdct) {                                    //constructor, accepts the vendor ID, and the product ID
        this.vndr = vndr;
        this.pdct = pdct;
    }
    
    public void connectDevice() {                                               //used to connect to the device
        System.out.println("Connecting to device " + vndr + ", " + pdct);
        deviceFindFirst();                                                      //find the device and connect if possible
        if(device == null) {                                                    //if the device was not connected
            System.out.println("Cannot find device");
        } else {
            System.out.println("Connected to device");                          //device was connected successfully
        }
    }
    
    private void deviceFindFirst() {                                            //look for the device, and then connect to it
        deviceInitialize();                                                     //maybe remove                                            
        HIDDeviceInfo[] devices = deviceFindAllDescriptors();                   //find the device that we want to connect to

        if (devices.length > 0) {                                               //check if we found a device (there will be something in that array)
            try {                                                                       
                device = devices[0].open();                                     //try to open the first device in the array, and set that to the active device var
            } catch (Exception e) {
                device = null;                                                  //device cannot be found error
            }
        }  
    }
    
    private static void deviceInitialize() {                                    //used to load the hidapi library or something
        if (!device_initialized) {                                              //only do this if the device has not yet been initialized
            device_initialized = true;                                            
            com.codeminders.hidapi.ClassPathLibraryLoader
                .loadNativeHIDLibrary();                                        //load the hidapi library
        }
    }
    
    public String getData() {                                                   //used to get data from the device, returns a string
        if( device != null) {                                                   //make sure we are connected to a device, cant read from nothing
            String result = deviceRead();                                       //try to get data from deviceRead and put it into a string  
            if( result != null ) {                                              //if deviceRead returned something
                return result;                                                  //return what we got
            }
        }
        return null;                                                            //return nothing if we couldn't do anything
    }
        
    private HIDDeviceInfo[] deviceFindAllDescriptors() {                        //returns an HIDDeviceInfo array of device descriptors, usualy only one thing in there
        deviceInitialize();                                                     //remove?
        List<HIDDeviceInfo> devlist = new ArrayList<HIDDeviceInfo>();           //create a new arraylist to store a list of devices in, put a list of devices into it
        
        try {
            HIDManager hidManager = HIDManager.getInstance();                   //create a new HIDManager, and set it to the result of HIDManager.getInstance()
            HIDDeviceInfo[] infos = hidManager.listDevices();                   //create a new HIDDeviceInfo array, fill it with a list of devices we got from the HIDManager 

            for (HIDDeviceInfo info : infos) {                                  //for each device in HIDDeviceInfo
                if (info.getVendor_id() == vndr
                    && info.getProduct_id() == pdct) {                          //look for a device that matches the vendor and product id we are looking for
                    devlist.add(info);                                          //add this device to that arraylist
                }
            }
        } catch (Exception e) {                                                 //catch any dumb errors
        }

        return devlist.toArray(new HIDDeviceInfo[devlist.size()]);              //convert that arraylist to a normal array, have the array size be the size of the arraylist, and return it
        //basically, return the single device within an array
    }
        
    private String deviceRead() {                                               //reads from the device, returns a string
        try {
            device.disableBlocking();                                           //disables read blocking for this device, you never know
            byte[] data = new byte[10];                                         //create a new array to hold data, 10 elements
            int read = device.read(data);                                       //read the device data into that array, and get the size of the transfer from it too
            if (read > 0) {                                                     //check if there was anything in that transfer
                String str = new String();                                      //make a new string to put that stuff in
                for(int i=0;i<read;i++) {                                       //for each byte in the transfer
                    if(data[i]!=0) {                                            //if we are not at the end of the data array
                         str+=((char)data[i]);                                  //convert the current byte from that data array into a char, and add it to a string
                    }                                                                       
                }
                return str;                                                     //return that string, we got useful stuff in it
            } else {
                return null;                                                    //didn't get anything, darn
            }
        } catch(IOException ioe) {
            System.err.println("deviceRead error");
        }
        return null;
    }

    public void send(String in) {                                               //used to send a string of data to the device
        if( device != null ) {                                                  //make sure we are connected to the device
            if( in != null && !in.equals("") ) {                                //make sure the user didn't put nothing in there
                deviceWrite( in );                                              //tell deviceWrite to write that stuff to the device
            }
        }
    }

    private void deviceWrite(String value) {                                    //actually writes that stuff to the device
        char[] charArray = value.toCharArray();                                 //conver the string into an array of chars, and put it in a new char array
        int size = (charArray.length > 32) ? 32 : charArray.length;             //get the transfer size (that array size), make sure it's not over 32 (some dumb hid limit)
        byte[] data = new byte[33];                                             //create a new byte array called data, size 33 (to hold the identifier byte in the beginning)
        data[0] = (byte)0;                                                      //first element needs to be a 0 converted to a byte
        int i;
        for(i = 0;i<size;i++) {                                             //for each char in the string
            data[i+1]=(byte)charArray[i];                                       //convert each char in the array to a byte, and put each into the data array
        }
        data[i+1]=0;                                                            //set the next element to 0 so we know where to stop sending
        try {
            device.sendFeatureReport(data);                                     //send that data array to the device
        } catch(Exception e) {
            e.printStackTrace();
            System.err.println("deviceWrite error");                            //jerp
        }
    }
    
    public void disconnectDevice() {                                            //disconnects the currently connected device
        if (device!=null) {                                                     //make sure the device is actually connected, cant disconnect nothing
            try {
                device.close();                                                 //try closing the device
                System.out.println("Device disconnected");
            } catch (IOException ioe) {                                         //error stuff
                ioe.printStackTrace();
            }
            device = null;                                                      //device is going to be gone at this point                       
        }
    }
}
