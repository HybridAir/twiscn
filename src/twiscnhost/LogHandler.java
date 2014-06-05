//used for handling error/info logging
package twiscnhost;

import java.io.IOException;
 import java.util.logging.*;


public class LogHandler {
    private final static Logger logger = Logger.getLogger(LogHandler.class.getName());
    private static FileHandler fh = null;
       
    public static void init(){
        try {
            fh = new FileHandler("error.log", false);
        } 
        catch (SecurityException | IOException e) {
            e.printStackTrace();
        }
        Logger l = Logger.getLogger("");
        fh.setFormatter(new SimpleFormatter());
        l.addHandler(fh);
        l.setLevel(Level.WARNING);
        logger.log(Level.WARNING, "Logging started");
    }
}
