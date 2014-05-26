//used for managing the properties file and getting data in and out of it
package twiscnhost;

import java.util.Properties;

public class PropHandler {
    
    private Properties prop = new Properties();                                         //create a new instance of Properties, needed for data reading/writing
    private FileIO fio = new FileIO();                                                  //create a new instance of FileIO, have to write this data somewhere
    private Options opt;
    String[] names;
    
    
    public PropHandler(Options opt) {                                           //constructor, needs Options instance, gets the properties file going
        this.opt = opt;
        names = opt.getPropNames();                                    //get the array of known property names
        if(fio.findFile()) {                                                    //try to find and/or create the config file
            //if it returns true, default properties need to and can be written to the file
            writeAllProps(false);                                               //write defaults to the file, don't need to recreate it (already did)                              
        }
        else if(fio.usingConfig()) {                                            //don't need to write defaults, check integrity if we are still using the config
            checkNames();                                                   //make sure the the properties within the file exist
            checkValues();
        }
    }
    
//==============================================================================
    
    private void checkNames() {                                                 //used to ensure that the property names exist and are correct
        System.out.println("Checking property names");
        fio.openInput();                                                        //open the file for reading
        fio.loadProps(prop);                                                    //load the properties
        boolean malformed = false;                                              //not malformed yet
        int i = 0;                                                              //start at the beginning

        //check if each property exists
        while(!malformed && i < names.length) {                                 //while the file is not marked as malformed and we are still going through it
            if(prop.getProperty(names[i]) == null) {                            //if it does not exists
                System.out.println("Properties file is malformed, restoring defaults.");
                malformed = true;                                               //mark the file as malformed
                writeAllProps(malformed);                                       //write default properties to the file, let it know to recreate the file
            }
            else {                                                              //if the property name does exist
                System.out.println("got " + names[i] + "=" + prop.getProperty(names[i]));
                i++;                                                            //advance to the next name
            }
        }
        
        //once we get here, it is assumed that the property names are good
        fio.closeInput();
    }
    
    private void checkValues() {                                                //used to check each property value, fix, and apply them
        for(int i = 0;i < names.length;i++) {                                   //for each property we have
            try {
                    int out = Integer.parseInt((prop.getProperty(names[i])));   //try to convert the value to an int
                    opt.setPropValue(i, out);                                   //if we get here, send the probably-good value to options to get checked again and set
            }
            catch(NumberFormatException e) {                                    //parseInt will throw this if it doesn't like what it's seeing
                //don't do anything, just catch the error
                //options will reset the default value
            }
        }
        writeAllProps(false);                                                   //finished checkeing and fixing all values, go make needed changes to the file
        System.out.println("done checking");
    }
    
//==============================================================================
    
    private void writeAllProps(boolean recreate) {                              //used to write default properties to the file, needs to know if it needs to be recreated first
        if(recreate) {                                                          //if true, recreates the file
            fio.createFile();
        }
        String[] names = opt.getPropNames();                                    //get the array of property names
        String[] values = opt.getPropValues();                                  //get the array of property values 
        
        for(int i = 0;i < names.length;i++) {                                   //for each property
            prop.setProperty(names[i], values[i]);                              //set each property name and its value
        }     
        
        fio.writeProperties(prop);                                              //actually write the properties to the file            
    } 
}
