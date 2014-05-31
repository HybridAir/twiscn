/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */

package twiscnhost;

import java.text.SimpleDateFormat;
import java.util.Date;
import javax.swing.text.BadLocationException;

/**
 *
 * @author Blake
 */
public class GuiHandler extends Gui {
    
    public GuiHandler() {
        addStatusLine("Program initialized");
        setConnected(false);
    }
    
    public void setConnected(boolean connected) {
        if(connected) {
            statusLbl.setText("Connected");
        }
        else {
            statusLbl.setText("Searching");
        }
    }
    
    public void setVersions(String hardware, String firmware) {
        hardLbl.setText(hardware);
        firmLbl.setText(firmware);
    }
    
    public void addStatusLine(String in) {                                      //used to write a line to the top of the status box
        Date time = new Date();                                                 //get the current time
        SimpleDateFormat ft = new SimpleDateFormat ("HH:mm : ");                //get the date formatter ready
        String t = ft.format(time);                                             //create a new string with the current formatted time
        try {
            statusTxt.getDocument().insertString(0, t + in + "\n", null);       //put the in String at the top of the text area
        } catch (BadLocationException ex) {                                     //need to catch this, shouldn't ever get thrown
            ex.printStackTrace();
        }
    }
}
