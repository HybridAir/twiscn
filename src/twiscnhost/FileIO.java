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
    
    OutputStream output = null;
    InputStream input = null;
    private boolean exists = false;
    private boolean usingConfig = true;
    private boolean createProps = false;
    private String fileName = "joop.properties";
    
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
        try {
            input = new FileInputStream(fileName);                              //create a new inputstream using fileName
            System.out.println("Found configuration file.");                    //if we get here, the program found the file
            exists = true;                                                      //file exists, used to break out of any loops   
        }
        catch(FileNotFoundException e) {                                        //file was not found
            System.out.println("Configuration file was not found, attempting to create now.");
            exists = false;
            createFile();                                                       //the file was not found, so try to create it
	}
        finally {
            if (input != null) {                                                //if the inputstream was started successfully
		try {                                                           //try to close it since we are done
                    input.close();                                              
		} 
                catch (IOException e) {                                         //catch any IO errors
                    e.printStackTrace();
		}
            }
        }
    }
    
    private void createFile() {                                                 //creates a new properties file
        try {
            output = new FileOutputStream(fileName);                            //try to start a new outputsteam using fileName
            System.out.println("New configuration file was created.");          //if we get here, the file was created successfully
            createProps = true;                                                 //tell prophandler that it needs to set default properties
	} 
        catch (IOException io) {                                                //check for IO errors
            System.out.println("Unable to create configuration file.");
            io.printStackTrace();
	} 
        finally {                                                               //make sure to close the file when we are done
            if (output != null) {
                try {
                    output.close();
		} 
                catch (IOException e) {                                         //check for IO errors
                    e.printStackTrace();
		}
            }
	}    
    }
    
    public void writeProperties(Properties prop) {                              //used to write new properties to the file
        try {                                                                   
            output = new FileOutputStream(fileName);                            //try to open fileName                   
            prop.store(output, null);                                           //try to store the new properties
	} 
        catch (IOException io) {                                                //check for IO errors
            io.printStackTrace();
	} 
        finally {    
            if (output != null) {                                               //make sure to close the file when we are done
                try {
                    output.close();
                } 
                catch (IOException e) {                                         //check for IO errors
                    e.printStackTrace();
                }
            }
	}
    }
    
}