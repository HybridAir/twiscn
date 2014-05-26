//used to manage files created and read by this program

package twiscnhost;

import java.io.FileInputStream;
import java.io.FileNotFoundException;
import java.io.FileOutputStream;
import java.io.IOException;
import java.io.InputStream;
import java.io.OutputStream;
import java.util.Properties;

public class FileIO {
    
    private OutputStream output = null;
    private InputStream input = null;
    private boolean exists = false;
    private boolean usingConfig = true;
    private boolean createProps = false;
    private String fileName = "config.properties";
    
    public FileIO() {
        
    }
    
    public boolean findFile() {                                                 //used to check if the config file exists, and create it if necessary/possible
        byte attempts = 0;                                                      //used to track attempts
        while(!exists && usingConfig) {                                         //while the file doesn't exist, and we are actually going to use the file
            checkExists();                                                      //check if the file exists
            attempts++;                                                         //file was not found if we got here, so increase the attempt amount
            if(attempts == 3) {                                                 //if we failed at creating the file 3 times
                //failsafe, the program can still run without a config file
                System.out.println("Failed to find/create config file 3 times. Proceeding without loading/saving settings.");
                usingConfig = false;                                            //let the program know not to use a config file
            }
        }
        return createProps;                                                     //return whether we need to set deafult properties
    }
    
//==============================================================================
    
    public boolean usingConfig() {
        return usingConfig;
    }
    
    private void checkExists() {                                                //checks if the properties file exists
        if(openInput()) {                                                       //try to open the file input
            System.out.println("Found configuration file.");                    //if we get here, the program found the file
            exists = true;                                                      //file exists, used to break out of any loops 
        }
        else {                                                                  //unable to open the input file
            System.out.println("Configuration file was not found, attempting to create now.");
            exists = false;                                                     //assumed that the file does not exist
            createFile();                                                       //create the file
        }      
        closeInput();                                                           //close the file output, not longer needed
    }
    
    public void createFile() {                                                  //creates a new properties file
        if(openOutput()) {                                                      //if the program was able to open the output stream
            System.out.println("New configuration file was created.");
            createProps = true;                                                 //tell prophandler that it needs to set default properties
        }
        else {                                                                  //unable to open output stream, and create the file
            System.out.println("Unable to create configuration file.");
        }       
        closeOutput();    
    }
    
    public void writeProperties(Properties prop) {                              //used to write new properties to the file
        if(openOutput()) {
            try {                                                                                      
                prop.store(output, null);                                       //try to store the new properties
                System.out.println("Properties written (stored)");
            } 
            catch (IOException io) {                                            //check for IO errors
                io.printStackTrace();
                System.out.println("Unable to write properties");
            } 
        }     
        closeOutput();                                                          //close the file output, not longer needed
    }
    
//==============================================================================
    
    public boolean openInput() {                                                //used to open the file for reading (input), returns true if successful
        try {
            input = new FileInputStream(fileName);                              //create a new inputstream using fileName
            System.out.println("input opened");
            return true;
        }
        catch(FileNotFoundException e) {                                        //file was not found
            System.out.println("Unable to open input");
            return false;
	}
    }
    
    public void loadProps(Properties prop) {                                    //used to load the properties from the file
        if(input != null) {                                                     //only attempt loading if input is already open
            try {                                                                   
                prop.load(input);                                               //try to load the properties from the inputstream
                System.out.println("loaded props");
            } 
            catch (IOException io) {                                            //check for IO errors
                io.printStackTrace();
                System.out.println("Unable to open props");
            }
        }
    }
    
    public void closeInput() {                                                  //used to close the config file input
        if (input != null) {                                                    //only close it if it's already open
            try {                                                               //try to close it
                input.close();                              
                System.out.println("input closed");
            } 
            catch (IOException e) {                                             //catch any IO errors
                e.printStackTrace();
            }
        }
    }
    
    public boolean openOutput() {                                               //used to open the file for writing (output), returns true if successful
        try {                                                                   
            output = new FileOutputStream(fileName);                            //try to open fileName
            System.out.println("output opened"); 
            return true;
	} 
        catch (IOException io) {                                                //check for IO errors
            io.printStackTrace();
            System.out.println("Unable to open output");
            return false;
	} 
    }
    
    public void closeOutput() {                                                 //used to close the file output
        if (output != null) {                                                   //only close it if it's already open
            try {
                output.close();                                                 //close the output stream
                System.out.println("output closed");
            } 
            catch (IOException e) {                                             //check for IO errors
                e.printStackTrace();
            }
        }   
    }
}