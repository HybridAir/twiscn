//used to do things to the device when it sends button presses to us; fn button actions

package twiscnhost;

public class ButtonActions {
    
    private Options opt;
    private int brightness;
    private byte brightState = 0;
    
    public ButtonActions(Options opt) {
        this.opt = opt;
    }
    
    public void fn1() {                                                         //call this whenever we get a response from FN1 (make sure to applyOptions after)
        doAction(opt.getFn1Action());
    }
    
    private void doAction(byte type) {                                          //used to call different functions depending on the byte passed in
        switch(type) {
            case 0:
                switchBrightness();
                break;
            case 1:
                toggleRainbow();
                break;
            case 2:
                prevTweet();
                break;
            case 3:
                toggleScroll();
                break;
            default:
                break;                            
        }
    }
    
//==============================================================================
    
    private void switchBrightness() {                                           //used to switch through backlight brightness levels exponentially and on/off
        switch(brightState) {                                                   //switch through turning the backlight completely off and fading it on
            case 0:                                                             //backlight is off
                opt.setBrightness(0);                                           //set backlight to off
                brightness = 1;                                                 //prepare the brightness variable for exponential increasing
                brightState++;                                                  //advance to the next state
                break;
            case 1:                                                             //backlight is fading on
                if(brightness <= 256) {                                         //if the backlight is not at max yet
                    brightness = brightness*2;                                  //increase the current brightness by a factor of 2
                    if(brightness >= 256) {                                     //once we get to 256
                        opt.setBrightness(255);                                 //set the brightness to 255 instead, the max byte var size
                        brightness = 0;                                         //reset the brightness var
                        brightState = 0;                                        //turn the backlight off next run through
                    }
                    else {                                                      //if we are not at 256 yet
                        opt.setBrightness(brightness);                          //set the current brightness value
                    }
                }
                break;
        }     
    }
    
    private void toggleRainbow() {                                              //used to toggle the rainbow state
        if(opt.getRnbwStateBool()) {                                            //if rainbow mode is currently on
            opt.setRnbwState(false);                                            //turn it off
        }
        else {                                                                  //if it's already off, turn it on
            opt.setRnbwState(true);
        }
    }
    
    private void prevTweet() {                                                  //used to switch to the previous tweet and back
        if(opt.getPrevTweetBool()) {                                            //if previous tweet is currently active
            opt.setPrevTweet(false);                                            //go back to the current tweet
        }
        else {                                                                  //vice versa
            opt.setPrevTweet(true);
        }
    }
    
    private void toggleScroll() {                                               //used to pause the text scrolling (if applicable)
        if(opt.getScrollBool()) {                                               //if scrolling is currently enabled
            opt.setScroll(false);                                               //disable scrolling
        }
        else {                                                                  //vice versa
            opt.setScroll(true);
        }
    }
}
