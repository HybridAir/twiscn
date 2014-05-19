//used to process and manage the twitterscreen settings and stuff

package twiscnhost;

import java.awt.Color;

public class DeviceOptions {
    
    private int brightness = 255;
    private Color lcdCol = new Color(0, 150, 255);
    private boolean blink = true;
    private Color blinkCol = Color.YELLOW;
    private int blinkSpd = 100;
    private boolean rainbow = false;
    private int rnbwSpd = 100;
    private byte b = 0;

    public DeviceOptions() {                                                  //default constructor, all default settings
    
    }
    
    public DeviceOptions(String fn1, String fn2, int brightness, Color lcdCol) {                      //standard constructor, basic settings
        this.brightness = brightness;
        this.lcdCol = lcdCol;   
    }
    
    public DeviceOptions(String fn1, String fn2, int brightness, Color lcdCol, boolean blink, 
      Color blinkCol, int blinkSpd, boolean rainbow, int rnbwSpd) {             //extended constructor, all settings
        this.brightness = brightness;
        this.lcdCol = lcdCol;
        this.blink = blink;
        this.blinkCol = blinkCol;
        this.blinkSpd = blinkSpd;
        this.rainbow = rainbow;
        this.rnbwSpd = rnbwSpd;
    }
    
    public void fnHandler(int in) {
        if(in == 1) {
            switchBrightness();
        }
        else if(in == 2) {
            
        }
    }
    
    private void switchBrightness() {
        
        b++;
        if(b == 6) {
            b = 0;
        }
        switch(b) {
            case 0:
                brightness = 255;
                break;
            case 1:
                brightness = 128;
                break;
            case 2:
                brightness = 64;
                break;
            case 3:
                brightness = 32;
                break;
            case 4:
                brightness = 16;
                break;
            case 5:
                brightness = 0;
                break;
            default:
                break;
            }
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
        String[] out = {getBrightness(), getLCDColor(), getBlinkState(), getBlinkSpd(), getBlinkColor(), getRnbwState(), getRnbwSpd()};
        return out;
    }
    
//    public String[] getBrightness() {
//        String b = String.valueOf(brightness);
//        String[] out = {"0", ("000" + b).substring(b.length())};
//        return out;
//    }
        
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
        return ("000" + out).substring(out.length());
    }
    
    
    
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
}
