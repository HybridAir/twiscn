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
            //if it returns true, default properties need to and can be written to the file
            writeDefaults(false);                                               //write defaults to the file, don't need to recreate it (already did)                              
        }
        else if(fio.usingConfig()) {                                            //don't need to write defaults, check integrity if we are still using the config
            checkIntegrity();                                                   //make sure the the properties within the file exist
            parseValues();
        }
    }
    
//==============================================================================
    
    private void checkIntegrity() {                                             //used to check the integrity of the properties within the config file
        System.out.println("Checking config integrity");
        fio.openInput();                                                        //open the file for reading
        fio.loadProps(prop);                                                    //load the properties
        String[] names = opt.getPropNames();                                    //get the array of known property names
        boolean malformed = false;                                              //not malformed yet
        int i = 0;                                                              //start at the beginning

        //check if each property exists
        while(!malformed && i < names.length) {                                 //while the file is not marked as malformed and we are still going through it
            if(prop.getProperty(names[i]) == null) {                            //if it does not exists
                System.out.println("Properties file is malformed, restoring defaults.");
                malformed = true;                                               //mark the file as malformed
                writeDefaults(malformed);                                       //write default properties to the file, let it know to recreate the file
            }
            else {                                                              //if the property name does exist
                System.out.println("got " + names[i] + "=" + prop.getProperty(names[i]));
                i++;                                                            //advance to the next name
            }
        }
        
        //once we get here, it is assumed that the properties are good, or are no longer malformed (still clean)
        fio.closeInput();
    }
    
    private void applyValues() {                                                //used to get each property value out, fix them if they're retarded, and apply them
        //if(Integer.parseInt((prop.getProperty(names[i]))) > ) {

        //}
    }
    
//==============================================================================
    
    private void writeDefaults(boolean recreate) {                              //used to write default properties to the file, needs to know if it needs to be recreated first
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
    


//Color lcdCol = new Color(0, 150, 255); 
//       
//       String colorS = Integer.toString(lcdCol.getRGB());
//
//       Color noop = new Color(Integer.parseInt(colorS));
    
}
