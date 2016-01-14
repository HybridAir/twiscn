//used for managing the properties file and getting data in and out of it
package twiscnhost;

import java.util.Properties;
import java.util.logging.Level;

public class PropHandler {
    
    private final static java.util.logging.Logger logger = java.util.logging.Logger.getLogger(LogHandler.class.getName());
    private Properties prop = new Properties();                                 //create a new instance of Properties, needed for data reading/writing
    private FileIO fio = new FileIO();                                          //create a new instance of FileIO, have to write this data somewhere
    private Options opt;
    String[] names;
    
    
    public PropHandler(Options opt) {                                           //constructor, needs Options instance, gets the properties file going
        this.opt = opt;
        names = opt.getPropNames();                                             //get the array of known property names
        if(fio.findFile()) {                                                    //try to find and/or create the config file
            //if it returns true, default properties need to and can be written to the file
            writeAllProps(false);                                               //write defaults to the file, don't need to recreate it (already did)                              
        }
        else if(fio.usingConfig()) {                                            //don't need to write defaults, check integrity if we are still using the config
            checkNames();                                                       //make sure the the properties within the file exist
            checkValues();
        }
    }
    
//==============================================================================
    
    private void checkNames() {                                                 //used to ensure that the property names exist and are correct
        fio.openInput();                                                        //open the file for reading
        fio.loadProps(prop);                                                    //load the properties
        boolean malformed = false;                                              //not malformed yet
        int i = 0;                                                              //start at the beginning

        //check if each property exists
        while(!malformed && i < names.length) {                                 //while the file is not marked as malformed and we are still going through it
            if(prop.getProperty(names[i]) == null) {                            //if it does not exists
                logger.log(Level.WARNING, "Properties file is malformed, restoring defaults/nBad value was" + names[i]);
                malformed = true;                                               //mark the file as malformed
                writeAllProps(malformed);                                       //write default properties to the file, let it know to recreate the file
            }
            else {                                                              //if the property name does exist
                logger.log(Level.FINE, "got " + names[i] + "=" + prop.getProperty(names[i]));
                i++;                                                            //advance to the next name
            }
        }
        
        //once we get here, it is assumed that the property names are good
        fio.closeInput();
    }
    
    private void checkValues() {                                                //used to check each property value, fix, and apply them
        for(int i = 0;i < names.length;i++) {                                   //for each property we have
            opt.extractPropValue(i, prop.getProperty(names[i]));                //try to get the property value, then extract it
        }
        writeAllProps(false);                                                   //finished checkeing and fixing all values, go make needed changes to the file
        logger.log(Level.FINE, "Done checking values");
    }
    
//==============================================================================
    
    public void writeAllProps(boolean recreate) {                              //used to write properties to the file, needs to know if the file needs to be recreated first
        if(fio.usingConfig()) {
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
}
