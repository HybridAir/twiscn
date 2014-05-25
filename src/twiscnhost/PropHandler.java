//used for managing the properties file and getting data in and out of it
package twiscnhost;

import java.util.Properties;

public class PropHandler {
    
    private Properties prop = new Properties();                                         //create a new instance of Properties, needed for data reading/writing
    private FileIO fio = new FileIO();                                                  //create a new instance of FileIO, have to write this data somewhere
    private Options opt;
    
    public PropHandler(Options opt) {                                           //constructor, needs Options instance, gets the properties file going
        this.opt = opt;
        if(fio.findFile()) {                                                    //try to find and/or create the config file
            //if it returns true, default properties need to be written to the file
            writeDefaults();
        }
        else if(fio.usingConfig()) {                                            //don't need to write defaults, check integrity if we are using the config still
            checkIntegrity();                                                   //make sure the the properties within the file are not retarded
        }
    }
    
    private void checkIntegrity() {                                             //used to check the integrity of the properties within the config file
        if(fio.usingConfig()) {
            
        }
    }
    
    private void writeDefaults() {                                              //used to write default properties to a newly created properties file
        String[] names = opt.getPropNames();                                    //get the array of property names
        String[] values = opt.getPropValues();                                  //get the array of property values 
        
        for(int i = 0;i < names.length;i++) {                                   //for each property
            prop.setProperty(names[i], values[i]);                              //set each property name and its value
        }     
        
        fio.writeProperties(prop);                                              //actually write the properties to the file            
    }
    


//Color lcdCol = new Color(0, 150, 255); 
//       
//       String colorS = Integer.toString(lcdCol.getRGB());
//
//       Color noop = new Color(Integer.parseInt(colorS));
    
}
