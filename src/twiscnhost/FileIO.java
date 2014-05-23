//used to manage files created by this program

package twiscnhost;

import java.io.FileInputStream;
import java.io.FileNotFoundException;
import java.io.FileOutputStream;
import java.io.IOException;
import java.io.InputStream;
import java.io.OutputStream;

public class FileIO {
    
    OutputStream output = null;
    InputStream input = null;
    private boolean exists = false;
    private boolean usingConfig = true;
    
    public void findFile() {                                                    //used to check if the config file exists, and create it if necessary/possible
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
    }
    
    public boolean usingConfig() {
        return usingConfig;
    }
    
    private void checkExists() {
        try {
            input = new FileInputStream("7rep.properties");
            System.out.println("Found configuration file.");
            exists = true;          
        }
        catch(FileNotFoundException e) {
            System.out.println("Configuration file was not found, attempting to create now.");
            exists = false;
            createFile();                                                       //the file was not found, so try to create it
	}
        finally {
            if (input != null) {
		try {
                    input.close();
		} 
                catch (IOException e) {
                    e.printStackTrace();
		}
            }
        }
    }
    
    private void createFile() {
        try {
            output = new FileOutputStream("7rep.properties");
            System.out.println("New configuration file was created.");
	} 
        catch (IOException io) {
            System.out.println("Unable to create configuration file.");
            io.printStackTrace();
	} 
        finally {
            if (output != null) {
                try {
                    output.close();
		} 
                catch (IOException e) {
                    e.printStackTrace();
		}
            }
	}    
    }
}
