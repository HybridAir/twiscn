//used to process and manage the twitterscreen settings and stuff
//TODO: add in fn button action settings

package twiscnhost;

import java.awt.Color;
import java.util.ArrayList;

public class Options {
    
    //set default option variables      
    private Color lcdCol = new Color(0, 150, 255);  
    private Color blinkCol = Color.YELLOW; 
    private int rnbwSpd = 100;   
    private int readTime = 1000;
    private int blinkSpd = 100;
    private int brightness = 255;
    private boolean rainbow = false;
    private boolean blink = true;
    private boolean prevTweet = false;
    private boolean scroll = true;
    private byte fn1Action = 3;
    private byte fn2Action = 0;
    private ArrayList<Long> followUsers = new ArrayList<>();
    
    //max values for certain options
    private final int MAXBRIGHTNESS = 255;
    private final int MAXBLINKSPD = 999;
    private final int MAXRNBWSPD = 65535;
    private final int MAXREADTIME = 65535;
    private final int MAXACTION = 3;
    
    public String[] propValues;
    public String[] propNames;

    public Options() {                                                          //default constructor, use all default settings
        followUsers.add(2524002330L);                                           //add twiscn as a default follow
        followUsers.add(4536646346L);                                           //add twiscn as a default follow
        
    }
    
//==============================================================================
    
    private String formatColor(Color in) {                                      //extracts each color value, and combines them all into a string with leading zeroes
        String out = "";
        String r = String.valueOf(in.getRed());
        String g = String.valueOf(in.getGreen());
        String b = String.valueOf(in.getBlue());
        out += ("000" + r).substring(r.length());
        out += ("000" + g).substring(g.length());
        out += ("000" + b).substring(b.length());
        return out;
    }
    
    public String[] formatAll() {
        String[] out = {prepareBrightness(), prepareLCDColor(), prepareBlinkState(), 
            prepareBlinkSpd(), prepareBlinkColor(), prepareRnbwState(), prepareRnbwSpd(), 
            prepareReadTime(), preparePrevTweet(), prepareScroll()};
        return out;
    }
     
//============================================================================== 
    
    public String[] getPropNames() {
        String[] out = {"brightness", "lcdColor", "blinkState", "blinkColor", 
            "blinkSpeed", "rainbowState", "rainbowSpeed", "readTime", 
            "fn1Action", "fn2Action", "followUsers"};
        return out;
    }
    
    public String[] getPropValues() {     
        String[] out = {String.valueOf(String.valueOf(brightness)), 
            getLCDColorInt(), prepareBlinkState(), getBlinkColorInt(), 
            String.valueOf(blinkSpd), prepareRnbwState(), String.valueOf(rnbwSpd), 
            String.valueOf(readTime), getFn1Str(), getFn2Str(), getFollowUsersStr()};
        return out;
    }
    
    //public void extractPropValue(int valueID, int in) {                             //used to set the options here to values read from the config file, fixing them is necessary
    public void extractPropValue(int valueID, String in) {                             //used to set the options here to values read from the config file, fixing them is necessary
        //needs the property value ID and the value
        switch(valueID) {
            case 0:
                setBrightness(checkValLimits(MAXBRIGHTNESS, Integer.parseInt(in)));
                break;
            case 1:
                setLCDColor(new Color(Integer.parseInt(in)));
                break;
            case 2:
                setBlinkState((checkValLimits(1, Integer.parseInt(in))) <= 0 ? true : false);     //convert the int to a boolean
                break;
            case 3:
                setBlinkColor(new Color(Integer.parseInt(in)));
                break;
            case 4:
                setBlinkSpd(checkValLimits(MAXBLINKSPD, Integer.parseInt(in)));
                break;
            case 5:
                setRnbwState((checkValLimits(1, Integer.parseInt(in))) <= 0 ? true : false);      //convert the int to a boolean
                break;
            case 6:
                setRnbwSpd(checkValLimits(MAXRNBWSPD, Integer.parseInt(in)));
                break;
            case 7:
                setReadTime(checkValLimits(MAXREADTIME, Integer.parseInt(in)));
                break;
            case 8:
                setFn1Action((byte)checkValLimits(MAXACTION, Integer.parseInt(in)));
                break;
            case 9:
                setFn2Action((byte)checkValLimits(MAXACTION, Integer.parseInt(in)));
                break;
            case 10:
                extractFollowUsers(in);
                break;
        }
    }
    
    private int checkValLimits(int max, int in) {                               //used to ensure the property values are not outside their limits
        if(in > max) {                                                          //if the value is greater than the max it can be
            System.out.println("got " + in);
            return max;                                                         //return the max value
        }
        else if(in < 0) {                                                       //vice versa, all option values are never less than 0
            System.out.println("got " + in);
            return 0;
        }
        else                                                                    //property value should be good, just return it as is
            return in;    
    }
    
//==============================================================================  
    
    public String prepareBrightness() {
        String out = String.valueOf(brightness);
        return ("000" + out).substring(out.length());
    }
    
    public String prepareLCDColor() {
        return formatColor(lcdCol);
    }
    
    public String prepareBlinkState() {
        int out = blink ? 1 : 0;
        return String.valueOf(out);
    }
    
    public String prepareBlinkColor() {
        return formatColor(blinkCol);
    }
    
    public String prepareBlinkSpd() {
        String out = String.valueOf(blinkSpd);
        return ("000" + out).substring(out.length());
    }
    
    public String prepareRnbwState() {
        int out = rainbow ? 1 : 0;
        return String.valueOf(out);
    }
    
    public String prepareRnbwSpd() {
        String out = String.valueOf(rnbwSpd);
        return ("00000" + out).substring(out.length());
    }
    
    public String prepareReadTime() {
        String out = String.valueOf(readTime);
        return ("00000" + out).substring(out.length());
    }
    
    public String preparePrevTweet() {
        int out = prevTweet ? 1 : 0;
        return String.valueOf(out);
    }
    
    public String prepareScroll() {
        int out = scroll ? 1 : 0;
        return String.valueOf(out);
    }
    
//============================================================================== 
    
    public int getBrightnessInt() {
        return brightness;
    }
    
    private String getLCDColorInt() {
        return Integer.toString(lcdCol.getRGB());
    }
    
    private String getBlinkColorInt() {
        return Integer.toString(blinkCol.getRGB());
    }
    
    public boolean getRnbwStateBool() {
        return rainbow;
    }
    
    public boolean getPrevTweetBool() {
        return prevTweet;
    }
    
    public boolean getScrollBool() {
        return scroll;
    }
    
    public byte getFn1Action() {
        return fn1Action;
    }
    
    private String getFn1Str() {
        String out = "" + fn1Action;
        return out;
    }
    
    public byte getFn2Action() {
        return fn2Action;
    }
    
    private String getFn2Str() {
        String out = "" + fn2Action;
        return out;
    }
    
//==============================================================================   
    
    public void setBrightness(int in) {
        brightness = in;
    }
    
    public void setLCDColor(Color in) {
        lcdCol = in;
    }
    
    public void setBlinkState(boolean in) {
        blink = in;
    }
    
    public void setBlinkColor(Color in) {
        blinkCol = in;
    }
    
    public void setBlinkSpd(int in) {
        blinkSpd = in;
    }
    
    public void setRnbwState(boolean in) {
        rainbow = in;
    }
    
    public void setRnbwSpd(int in) {
        rnbwSpd = in;
    }
    
    public void setReadTime(int in) {
        readTime = in;
    }
    
    public void setPrevTweet(boolean in) {
        prevTweet = in;
    }
    
    public void setScroll(boolean in) {
        scroll = in;
    }
    
    public void setFn1Action(byte in) {
        fn1Action = in;
    }
    
    public void setFn2Action(byte in) {
        fn2Action = in;
    } 
    
//==============================================================================  
    
    public long[] getFollowUsers() {                                            //converts the followUsers arraylist to an array of longs
        long[] out = new long[followUsers.size()];                              //create a new array of longs
        for (int i = 0; i < followUsers.size(); i++) {                          //for each entry in the arraylist
            out[i] = followUsers.get(i);                                        //put that entry in the corresponding array element
        }
        return out;    
    }

    public String getFollowUsersStr() {                                         //returns a formatted delimited String containing the userIDs to follow
        long[] in = getFollowUsers();                                           //get the followUsers into a new array
        String out = "";
        for(int i = 0;i <= in.length - 1;i++) {                                 //for each followUser
            out += in[i];                                                       //add the ID to the output String
            if(i < in.length - 1) {                                             //if we are not on the last ID
                out += ",";                                                     //add a , to be used as a delimiter
            }
        }
        return out;
    }
    
    private void extractFollowUsers(String in) {                                //extracts the userIDs from the delimited String, and apply them
        String[] stringIn = in.split(",");                                      //split the string up into managable parts
        clearFollowUsers();                                                     //clear the list of followUsers
        for(int i = 0;i < stringIn.length;i++) {                                //for each userID string we got
            addFollowUser(Long.parseLong(stringIn[i]));                         //conver the ID to a long, and add it to the list
        }
    }
      
    public void addFollowUser(long in) {
        followUsers.add(in);
    }
    
    public void delFollowUser(long in) {
        followUsers.remove(in);
    }
    
    public void clearFollowUsers() {
        followUsers.clear();
    }
}
