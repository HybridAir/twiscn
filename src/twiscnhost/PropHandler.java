//used for managing the properties file and getting data in and out of it
package twiscnhost;

import java.io.FileInputStream;
import java.io.FileNotFoundException;
import java.io.OutputStream;
import java.io.InputStream;
import java.util.Properties;

public class PropHandler {
    
    Properties prop = new Properties();
    OutputStream output = null;
    InputStream input = null;
    Options opt;
    private boolean exists = false;
    private boolean usingConfig = true;
    
    public PropHandler(Options opt) {
        this.opt = opt;
        findFile();
        checkIntegrity();
    }
    
    
    private void findFile() {                                                   //used to check if the config file exists, and create it if necessary/possible
        byte attempts = 0;
        checkExists();                                                          //check if the config file exists
        while(!exists) {                                                        //while the config file doesn't exist
            checkExists();                                                      //check if the file exists
            createFile();                                                       //if we get here, the file does not exist, so try to create it
            attempts++;                                                         //increase the attempt amount
            if(attempts == 3) {                                                 //if we failed at creating the file 3 times
                //failsafe, the program can still run without a config file
                System.out.println("Failed to create config file 3 times. Proceeding without config file.");
                usingConfig = false;
            }
        }
    }
    
    private void checkExists() {
        try {
            input = new FileInputStream("goop.properties");
            exists = true;          
        }
        catch(FileNotFoundException e) {
            System.out.println("Configuration file does not exist, creating it now.");
            exists = false;
	}       
    }
    
    private void createFile() {
        
    }
    
    private void checkIntegrity() {
        
    }
    
}
