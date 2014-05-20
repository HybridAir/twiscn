//used to process and manage the twitterscreen settings and stuff
//TODO: add in fn button action settings

package twiscnhost;

import java.awt.Color;

public class Options {
    
    //set default option variables   
    private Color lcdCol = new Color(0, 150, 255);  
    private Color blinkCol = Color.YELLOW; 
    private int rnbwSpd = 10000;   
    private int readTime = 1000;
    private int blinkSpd = 100;
    private int brightness = 255;
    private boolean rainbow = false;
    private boolean blink = true;
    private byte b = 0;
    //these will correspond to different actions that the buttons can do soon
    private byte fn1Action = 0;
    private byte fn2Action = 1;

    public Options() {                                                          //default constructor, use all default settings  
    }
    
    public Options(byte fn1, byte fn2, int brightness, Color lcdCol) {          //standard constructor, basic settings
        this.brightness = brightness;
        this.lcdCol = lcdCol;
        fn1Action = fn1;
        fn1Action = fn2;
    }
    
    public Options(byte fn1, byte fn2, int brightness, Color lcdCol, boolean blink, 
        Color blinkCol, int blinkSpd, boolean rainbow, int rnbwSpd) {           //extended constructor, all settings
        this.brightness = brightness;
        this.lcdCol = lcdCol;
        this.blink = blink;
        this.blinkCol = blinkCol;
        this.blinkSpd = blinkSpd;
        this.rainbow = rainbow;
        this.rnbwSpd = rnbwSpd;
    }
    
    private String formatColor(Color in) {                                              //extracts each color value, and combine them all into a string with leading zeroes
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
        String[] out = {getBrightness(), getLCDColor(), getBlinkState(), 
            getBlinkSpd(), getBlinkColor(), getRnbwState(), getRnbwSpd(), getReadTime()};
        return out;
    }
     
//==============================================================================    
    
    public String getBrightness() {
        String out = String.valueOf(brightness);
        return ("000" + out).substring(out.length());
    }
    
    public String getLCDColor() {
        return formatColor(lcdCol);
    }
    
    public String getBlinkState() {
        int out = blink ? 1 : 0;
        return String.valueOf(out);
    }
    
    public String getBlinkColor() {
        return formatColor(blinkCol);
    }
    
    public String getBlinkSpd() {
        String out = String.valueOf(blinkSpd);
        return ("000" + out).substring(out.length());
    }
    
    public String getRnbwState() {
        int out = rainbow ? 1 : 0;
        return String.valueOf(out);
    }
    
    public String getRnbwSpd() {
        String out = String.valueOf(rnbwSpd);
        return ("00000" + out).substring(out.length());
    }
    
    public String getReadTime() {
        String out = String.valueOf(readTime);
        return ("00000" + out).substring(out.length());
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
}
