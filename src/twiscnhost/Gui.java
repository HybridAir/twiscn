package twiscnhost;

import java.awt.Color;
import java.awt.Desktop;
import java.net.URL;
import java.text.SimpleDateFormat;
import java.util.Date;
import java.util.logging.Level;
import java.util.logging.Logger;
import javax.swing.DefaultListModel;
import javax.swing.JColorChooser;
import javax.swing.JOptionPane;
import javax.swing.text.BadLocationException;
import twitter4j.TwitterException;

public class Gui extends javax.swing.JFrame {
    
    private final static java.util.logging.Logger logger = java.util.logging.Logger.getLogger(LogHandler.class.getName());
    private Options opt;
    private TweetHandler twt;
    public boolean applyOptions = false;                                        //stores whether apply the options is needed asap
    
    public Gui(Options opt) {                                                   //default constructor, gets everything started, needs the Options instance
        //set windows native lookandfeel
        //<editor-fold defaultstate="collapsed" desc=" Look and feel setting code (optional) ">
        /* If Nimbus (introduced in Java SE 6) is not available, stay with the default look and feel.
         * For details see http://download.oracle.com/javase/tutorial/uiswing/lookandfeel/plaf.html 
         */
        try {
            //for (javax.swing.UIManager.LookAndFeelInfo info : javax.swing.UIManager.getInstalledLookAndFeels()) {
                //if ("com.sun.java.swing.plaf.windows.WindowsLookAndFeel".equals(info.getName())) {
                    javax.swing.UIManager.setLookAndFeel("com.sun.java.swing.plaf.windows.WindowsLookAndFeel");
                    //break;
                //}
            //}
        } catch (ClassNotFoundException ex) {
            java.util.logging.Logger.getLogger(Gui.class.getName()).log(java.util.logging.Level.SEVERE, null, ex);
        } catch (InstantiationException ex) {
            java.util.logging.Logger.getLogger(Gui.class.getName()).log(java.util.logging.Level.SEVERE, null, ex);
        } catch (IllegalAccessException ex) {
            java.util.logging.Logger.getLogger(Gui.class.getName()).log(java.util.logging.Level.SEVERE, null, ex);
        } catch (javax.swing.UnsupportedLookAndFeelException ex) {
            java.util.logging.Logger.getLogger(Gui.class.getName()).log(java.util.logging.Level.SEVERE, null, ex);
        }
        //</editor-fold>
        this.opt = opt;
        initComponents();                                                       //get all the components going
    }
    
    public void init(TweetHandler twt) {                                        //used to finish setting up and displaying the GUI, needs the tweethandler instance
        this.twt = twt;
        setDeviceDefaults();                                                    //set the default settings for the device tab
        refreshUserList();                                                          //set up the user list in the twitter tab
        deviceApplyBtn.setEnabled(false);                                       //disable the device apply button, it will get the default settings automatically
        setVisible(true);                                                       //make the primary window visible       
        addStatusLine("Program initialized");               
        setConnected(false);                                                    //set the connected status to false
    }
    
//==============================================================================
    
    public void addStatusLine(String in) {                                      //used to write a line to the top of the status box
        Date time = new Date();                                                 //get the current time
        SimpleDateFormat ft = new SimpleDateFormat ("HH:mm : ");                //get the date formatter ready
        String t = ft.format(time);                                             //create a new string with the current formatted time
        try {
            statusTxt.getDocument().insertString(0, t + in + "\n", null);       //put the in String at the top of the text area
        } catch (BadLocationException ex) {                                     //need to catch this, shouldn't ever get thrown
            Logger.getLogger(Gui.class.getName()).log(Level.SEVERE, null, ex);  //log it just in case
        }
    }   
    
    private void openProfile(String userName) {                                 //used to open the selected user's profile in the system's browser
        openWebsite("https://twitter.com/" + userName);
    }
    
    private void getUserInfo(String userName) {                                 //used to get and display the selected user's info, needs their username
        twtUserLbl.setText(userName);                                           //set the username label
        long userID;                                                            //get a long ready
        try {
            userID = twt.getUserID(userName);                                   //try to convert the username to a userID
            twtUserIDLbl.setText(String.valueOf(userID));                       //set the userID label to that ID we just got
        } catch (TwitterException ex) {                                         //catch any twitter related errors
            Logger.getLogger(Gui.class.getName()).log(Level.SEVERE, null, ex);  //log it just in case
        }
        try {
            Date d = twt.getLastTweetDate(userName);                            //try to get the date of the user's last tweet
            if(d != null) {                                                     //if they actually tweeted before
                SimpleDateFormat ft = new SimpleDateFormat ("MM.dd.yyyy 'at' hh:mm a");
                twtLastLbl.setText(ft.format(d));                               //set the last tweet label to the formatted date
            }
            else {
                twtLastLbl.setText("Unable to get last Tweet date");
            }
            //got the user's data, so start enabling the necessary buttons
            twtProfileBtn.setEnabled(true);                                     
            remUserBtn.setEnabled(true);
        } catch (TwitterException ex) {                                         
            Logger.getLogger(Gui.class.getName()).log(Level.SEVERE, null, ex);  //log it just in case
        }
    }
    
    private void openWebsite(String url) {
        try {
            //try to open the user's profile
            Desktop.getDesktop().browse(new URL(url).toURI());
        } catch (Exception e) {                                                 //catch any errors, like if the browser is unable to open
            Logger.getLogger(Gui.class.getName()).log(Level.SEVERE, null, e);   //log it just in case
        }
    }
    
//==============================================================================     
    
    public void setConnected(boolean connected) {                               //used to set the device connection status, needs a boolean
        if(connected) {
            statusLbl.setText("Connected");
        }
        else {
            statusLbl.setText("Searching");
        }
    }
    
    public void setVersions(String hardware, String firmware) {                 //used to set the device versions, needs two version Strings
        hardLbl.setText(hardware);
        firmLbl.setText(firmware);
    }
    
    private void setDeviceDefaults() {                                          //used to set the default device settings in the GUI
        opt.setDeviceDefaults();                                                //tell Options to set all device settings to default
        //reset all the necessary components
        brightnessSpnr.setValue(opt.getBrightnessInt());
        readSpnr.setValue(opt.getReadTime());
        rainEnabledChk.setSelected(opt.getRnbwStateBool());
        rainSpeedSpnr.setValue(opt.getRnbwSpeedInt());
        blinkEnabledChk.setSelected(opt.getBlinkStateBool());
        fn1Cbx.setSelectedIndex(opt.getFn1Action());
        fn2Cbx.setSelectedIndex(opt.getFn2Action());
    }
    
    private void refreshUserList() {                                                //used to update the list of users in the list
        twtUserLst.setListData(new Object[0]);                                  //clear the list
        long[] users = twt.getFollowUsers();                                    //get the user array from options
        DefaultListModel model = new DefaultListModel();                        //create a new list model
        twtUserLst.setModel(model);                                             //set the list to that empty model
        for(int i = 0;i < users.length;i++) {                                   //for each user we got
            try {
                String name = twt.getScreenName(users[i]);                      //try to convert the ID to a username String      
                model.addElement(name);                                         //add the username to the list
            } catch (TwitterException ex) {                                     //catch any twitter related errors
                Logger.getLogger(Gui.class.getName()).log(Level.SEVERE, null, ex);
            }
        }
    }
    
//==============================================================================    
    
    @SuppressWarnings("unchecked")
    // <editor-fold defaultstate="collapsed" desc="Generated Code">//GEN-BEGIN:initComponents
    private void initComponents() {

        colorFrame = new javax.swing.JFrame();
        jColorChooser1 = new javax.swing.JColorChooser();
        statusPane = new javax.swing.JPanel();
        statusScrollPane = new javax.swing.JScrollPane();
        statusTxt = new javax.swing.JTextArea();
        tabbedPane = new javax.swing.JTabbedPane();
        deviceTab = new javax.swing.JPanel();
        deviceInfoPane = new javax.swing.JPanel();
        statusName = new javax.swing.JLabel();
        hardwareName = new javax.swing.JLabel();
        firmwareName = new javax.swing.JLabel();
        statusLbl = new javax.swing.JLabel();
        firmLbl = new javax.swing.JLabel();
        hardLbl = new javax.swing.JLabel();
        deviceSettingsPane = new javax.swing.JTabbedPane();
        lcdTab = new javax.swing.JPanel();
        brightnessName = new javax.swing.JLabel();
        brightnessSpnr = new javax.swing.JSpinner();
        blinkPane = new javax.swing.JPanel();
        blinkEnabledChk = new javax.swing.JCheckBox();
        blinkColorBtn = new javax.swing.JButton();
        readName = new javax.swing.JLabel();
        readSpnr = new javax.swing.JSpinner();
        userColorBtn = new javax.swing.JButton();
        rainbowPane = new javax.swing.JPanel();
        rainEnabledChk = new javax.swing.JCheckBox();
        rainSpeedName = new javax.swing.JLabel();
        rainSpeedSpnr = new javax.swing.JSpinner();
        buttonsTab = new javax.swing.JPanel();
        fn1Name = new javax.swing.JLabel();
        fn1Cbx = new javax.swing.JComboBox();
        fn2Name = new javax.swing.JLabel();
        fn2Cbx = new javax.swing.JComboBox();
        deviceApplyBtn = new javax.swing.JButton();
        deviceDefaultsBtn = new javax.swing.JButton();
        twitterTab = new javax.swing.JPanel();
        followingPane = new javax.swing.JPanel();
        jScrollPane2 = new javax.swing.JScrollPane();
        twtUserLst = new javax.swing.JList();
        addUserBtn = new javax.swing.JButton();
        userInfoPane = new javax.swing.JPanel();
        twtUserTitle = new javax.swing.JLabel();
        twtUserIDTitle = new javax.swing.JLabel();
        remUserBtn = new javax.swing.JButton();
        twtProfileBtn = new javax.swing.JButton();
        twtLastTitle = new javax.swing.JLabel();
        twtUserLbl = new javax.swing.JLabel();
        twtUserIDLbl = new javax.swing.JLabel();
        twtLastLbl = new javax.swing.JLabel();
        twitterApplyBtn = new javax.swing.JButton();
        twitterDefaultsBtn = new javax.swing.JButton();
        settingsTab = new javax.swing.JPanel();
        jMenuBar1 = new javax.swing.JMenuBar();
        helpMenu = new javax.swing.JMenu();
        wikiMenuItm = new javax.swing.JMenuItem();
        bugMenuItm = new javax.swing.JMenuItem();
        jSeparator1 = new javax.swing.JPopupMenu.Separator();
        followMenuItm = new javax.swing.JMenuItem();
        aboutMenuItm = new javax.swing.JMenuItem();

        javax.swing.GroupLayout colorFrameLayout = new javax.swing.GroupLayout(colorFrame.getContentPane());
        colorFrame.getContentPane().setLayout(colorFrameLayout);
        colorFrameLayout.setHorizontalGroup(
            colorFrameLayout.createParallelGroup(javax.swing.GroupLayout.Alignment.LEADING)
            .addGroup(javax.swing.GroupLayout.Alignment.TRAILING, colorFrameLayout.createSequentialGroup()
                .addContainerGap(javax.swing.GroupLayout.DEFAULT_SIZE, Short.MAX_VALUE)
                .addComponent(jColorChooser1, javax.swing.GroupLayout.PREFERRED_SIZE, 646, javax.swing.GroupLayout.PREFERRED_SIZE)
                .addContainerGap())
        );
        colorFrameLayout.setVerticalGroup(
            colorFrameLayout.createParallelGroup(javax.swing.GroupLayout.Alignment.LEADING)
            .addGroup(colorFrameLayout.createSequentialGroup()
                .addContainerGap()
                .addComponent(jColorChooser1, javax.swing.GroupLayout.PREFERRED_SIZE, 344, javax.swing.GroupLayout.PREFERRED_SIZE)
                .addContainerGap(javax.swing.GroupLayout.DEFAULT_SIZE, Short.MAX_VALUE))
        );

        setDefaultCloseOperation(javax.swing.WindowConstants.EXIT_ON_CLOSE);
        setTitle("TwiScn Host");
        setResizable(false);

        statusPane.setBorder(javax.swing.BorderFactory.createTitledBorder("Status"));

        statusTxt.setColumns(20);
        statusTxt.setRows(5);
        statusTxt.setToolTipText("Program status log");
        statusTxt.setCursor(new java.awt.Cursor(java.awt.Cursor.TEXT_CURSOR));
        statusTxt.setFocusable(false);
        statusTxt.setOpaque(false);
        statusTxt.setRequestFocusEnabled(false);
        statusScrollPane.setViewportView(statusTxt);

        javax.swing.GroupLayout statusPaneLayout = new javax.swing.GroupLayout(statusPane);
        statusPane.setLayout(statusPaneLayout);
        statusPaneLayout.setHorizontalGroup(
            statusPaneLayout.createParallelGroup(javax.swing.GroupLayout.Alignment.LEADING)
            .addGroup(javax.swing.GroupLayout.Alignment.TRAILING, statusPaneLayout.createSequentialGroup()
                .addContainerGap()
                .addComponent(statusScrollPane, javax.swing.GroupLayout.DEFAULT_SIZE, 497, Short.MAX_VALUE)
                .addContainerGap())
        );
        statusPaneLayout.setVerticalGroup(
            statusPaneLayout.createParallelGroup(javax.swing.GroupLayout.Alignment.LEADING)
            .addGroup(statusPaneLayout.createSequentialGroup()
                .addContainerGap()
                .addComponent(statusScrollPane, javax.swing.GroupLayout.PREFERRED_SIZE, javax.swing.GroupLayout.DEFAULT_SIZE, javax.swing.GroupLayout.PREFERRED_SIZE)
                .addContainerGap(javax.swing.GroupLayout.DEFAULT_SIZE, Short.MAX_VALUE))
        );

        deviceInfoPane.setBorder(javax.swing.BorderFactory.createTitledBorder("Device Info"));

        statusName.setText("Status:");

        hardwareName.setText("Hardware Ver:");

        firmwareName.setText("Firmware Ver:");

        statusLbl.setFont(new java.awt.Font("Tahoma", 1, 13)); // NOI18N
        statusLbl.setHorizontalAlignment(javax.swing.SwingConstants.LEFT);

        firmLbl.setFont(new java.awt.Font("Tahoma", 1, 13)); // NOI18N
        firmLbl.setText(" ");

        hardLbl.setFont(new java.awt.Font("Tahoma", 1, 13)); // NOI18N
        hardLbl.setText(" ");
        hardLbl.setToolTipText("");

        javax.swing.GroupLayout deviceInfoPaneLayout = new javax.swing.GroupLayout(deviceInfoPane);
        deviceInfoPane.setLayout(deviceInfoPaneLayout);
        deviceInfoPaneLayout.setHorizontalGroup(
            deviceInfoPaneLayout.createParallelGroup(javax.swing.GroupLayout.Alignment.LEADING)
            .addGroup(deviceInfoPaneLayout.createSequentialGroup()
                .addContainerGap()
                .addComponent(statusName)
                .addPreferredGap(javax.swing.LayoutStyle.ComponentPlacement.RELATED)
                .addComponent(statusLbl, javax.swing.GroupLayout.PREFERRED_SIZE, 90, javax.swing.GroupLayout.PREFERRED_SIZE)
                .addGap(18, 18, 18)
                .addComponent(hardwareName)
                .addPreferredGap(javax.swing.LayoutStyle.ComponentPlacement.RELATED)
                .addComponent(hardLbl, javax.swing.GroupLayout.DEFAULT_SIZE, javax.swing.GroupLayout.DEFAULT_SIZE, Short.MAX_VALUE)
                .addPreferredGap(javax.swing.LayoutStyle.ComponentPlacement.UNRELATED)
                .addComponent(firmwareName)
                .addPreferredGap(javax.swing.LayoutStyle.ComponentPlacement.RELATED)
                .addComponent(firmLbl, javax.swing.GroupLayout.PREFERRED_SIZE, 60, javax.swing.GroupLayout.PREFERRED_SIZE)
                .addContainerGap())
        );
        deviceInfoPaneLayout.setVerticalGroup(
            deviceInfoPaneLayout.createParallelGroup(javax.swing.GroupLayout.Alignment.LEADING)
            .addGroup(deviceInfoPaneLayout.createSequentialGroup()
                .addContainerGap()
                .addGroup(deviceInfoPaneLayout.createParallelGroup(javax.swing.GroupLayout.Alignment.BASELINE)
                    .addComponent(statusName)
                    .addComponent(statusLbl)
                    .addComponent(hardwareName)
                    .addComponent(firmwareName)
                    .addComponent(firmLbl)
                    .addComponent(hardLbl))
                .addContainerGap(javax.swing.GroupLayout.DEFAULT_SIZE, Short.MAX_VALUE))
        );

        brightnessName.setText("Brightness:");

        brightnessSpnr.setModel(new javax.swing.SpinnerNumberModel(255, 0, 255, 1));
        brightnessSpnr.addChangeListener(new javax.swing.event.ChangeListener() {
            public void stateChanged(javax.swing.event.ChangeEvent evt) {
                brightnessSpnrStateChanged(evt);
            }
        });

        blinkPane.setBorder(javax.swing.BorderFactory.createTitledBorder("Tweet Blink"));

        blinkEnabledChk.setText("Enabled");
        blinkEnabledChk.addItemListener(new java.awt.event.ItemListener() {
            public void itemStateChanged(java.awt.event.ItemEvent evt) {
                blinkEnabledChkItemStateChanged(evt);
            }
        });

        blinkColorBtn.setText("Color");
        blinkColorBtn.setEnabled(false);
        blinkColorBtn.addActionListener(new java.awt.event.ActionListener() {
            public void actionPerformed(java.awt.event.ActionEvent evt) {
                blinkColorBtnActionPerformed(evt);
            }
        });

        javax.swing.GroupLayout blinkPaneLayout = new javax.swing.GroupLayout(blinkPane);
        blinkPane.setLayout(blinkPaneLayout);
        blinkPaneLayout.setHorizontalGroup(
            blinkPaneLayout.createParallelGroup(javax.swing.GroupLayout.Alignment.LEADING)
            .addGroup(blinkPaneLayout.createSequentialGroup()
                .addContainerGap()
                .addGroup(blinkPaneLayout.createParallelGroup(javax.swing.GroupLayout.Alignment.LEADING)
                    .addGroup(blinkPaneLayout.createSequentialGroup()
                        .addComponent(blinkEnabledChk)
                        .addGap(0, 34, Short.MAX_VALUE))
                    .addComponent(blinkColorBtn, javax.swing.GroupLayout.DEFAULT_SIZE, javax.swing.GroupLayout.DEFAULT_SIZE, Short.MAX_VALUE))
                .addContainerGap())
        );
        blinkPaneLayout.setVerticalGroup(
            blinkPaneLayout.createParallelGroup(javax.swing.GroupLayout.Alignment.LEADING)
            .addGroup(blinkPaneLayout.createSequentialGroup()
                .addContainerGap()
                .addComponent(blinkEnabledChk)
                .addPreferredGap(javax.swing.LayoutStyle.ComponentPlacement.RELATED, javax.swing.GroupLayout.DEFAULT_SIZE, Short.MAX_VALUE)
                .addComponent(blinkColorBtn)
                .addContainerGap())
        );

        readName.setText("Read Time:");

        readSpnr.setModel(new javax.swing.SpinnerNumberModel(2000, 0, 65535, 1));
        readSpnr.addChangeListener(new javax.swing.event.ChangeListener() {
            public void stateChanged(javax.swing.event.ChangeEvent evt) {
                readSpnrStateChanged(evt);
            }
        });

        userColorBtn.setText("Color");
        userColorBtn.addActionListener(new java.awt.event.ActionListener() {
            public void actionPerformed(java.awt.event.ActionEvent evt) {
                userColorBtnActionPerformed(evt);
            }
        });

        rainbowPane.setBorder(javax.swing.BorderFactory.createTitledBorder("Rainbow Mode"));

        rainEnabledChk.setText("Enabled");
        rainEnabledChk.addItemListener(new java.awt.event.ItemListener() {
            public void itemStateChanged(java.awt.event.ItemEvent evt) {
                rainEnabledChkItemStateChanged(evt);
            }
        });

        rainSpeedName.setText("Speed:");

        rainSpeedSpnr.setModel(new javax.swing.SpinnerNumberModel(100, 0, 65535, 1));
        rainSpeedSpnr.setEnabled(false);
        rainSpeedSpnr.addChangeListener(new javax.swing.event.ChangeListener() {
            public void stateChanged(javax.swing.event.ChangeEvent evt) {
                rainSpeedSpnrStateChanged(evt);
            }
        });

        javax.swing.GroupLayout rainbowPaneLayout = new javax.swing.GroupLayout(rainbowPane);
        rainbowPane.setLayout(rainbowPaneLayout);
        rainbowPaneLayout.setHorizontalGroup(
            rainbowPaneLayout.createParallelGroup(javax.swing.GroupLayout.Alignment.LEADING)
            .addGroup(rainbowPaneLayout.createSequentialGroup()
                .addContainerGap()
                .addGroup(rainbowPaneLayout.createParallelGroup(javax.swing.GroupLayout.Alignment.LEADING)
                    .addGroup(rainbowPaneLayout.createSequentialGroup()
                        .addComponent(rainEnabledChk)
                        .addGap(0, 0, Short.MAX_VALUE))
                    .addGroup(rainbowPaneLayout.createSequentialGroup()
                        .addComponent(rainSpeedName)
                        .addPreferredGap(javax.swing.LayoutStyle.ComponentPlacement.UNRELATED)
                        .addComponent(rainSpeedSpnr, javax.swing.GroupLayout.PREFERRED_SIZE, 1, Short.MAX_VALUE)))
                .addContainerGap())
        );
        rainbowPaneLayout.setVerticalGroup(
            rainbowPaneLayout.createParallelGroup(javax.swing.GroupLayout.Alignment.LEADING)
            .addGroup(rainbowPaneLayout.createSequentialGroup()
                .addContainerGap()
                .addComponent(rainEnabledChk)
                .addPreferredGap(javax.swing.LayoutStyle.ComponentPlacement.RELATED, javax.swing.GroupLayout.DEFAULT_SIZE, Short.MAX_VALUE)
                .addGroup(rainbowPaneLayout.createParallelGroup(javax.swing.GroupLayout.Alignment.BASELINE)
                    .addComponent(rainSpeedName)
                    .addComponent(rainSpeedSpnr, javax.swing.GroupLayout.PREFERRED_SIZE, javax.swing.GroupLayout.DEFAULT_SIZE, javax.swing.GroupLayout.PREFERRED_SIZE))
                .addContainerGap())
        );

        javax.swing.GroupLayout lcdTabLayout = new javax.swing.GroupLayout(lcdTab);
        lcdTab.setLayout(lcdTabLayout);
        lcdTabLayout.setHorizontalGroup(
            lcdTabLayout.createParallelGroup(javax.swing.GroupLayout.Alignment.LEADING)
            .addGroup(lcdTabLayout.createSequentialGroup()
                .addContainerGap()
                .addGroup(lcdTabLayout.createParallelGroup(javax.swing.GroupLayout.Alignment.LEADING, false)
                    .addGroup(lcdTabLayout.createSequentialGroup()
                        .addGroup(lcdTabLayout.createParallelGroup(javax.swing.GroupLayout.Alignment.LEADING)
                            .addComponent(brightnessName)
                            .addComponent(readName))
                        .addGap(18, 18, 18)
                        .addGroup(lcdTabLayout.createParallelGroup(javax.swing.GroupLayout.Alignment.LEADING, false)
                            .addComponent(brightnessSpnr, javax.swing.GroupLayout.DEFAULT_SIZE, 70, Short.MAX_VALUE)
                            .addComponent(readSpnr, javax.swing.GroupLayout.PREFERRED_SIZE, 1, Short.MAX_VALUE)))
                    .addComponent(userColorBtn, javax.swing.GroupLayout.DEFAULT_SIZE, javax.swing.GroupLayout.DEFAULT_SIZE, Short.MAX_VALUE))
                .addGap(18, 18, 18)
                .addComponent(rainbowPane, javax.swing.GroupLayout.DEFAULT_SIZE, javax.swing.GroupLayout.DEFAULT_SIZE, Short.MAX_VALUE)
                .addGap(18, 18, 18)
                .addComponent(blinkPane, javax.swing.GroupLayout.PREFERRED_SIZE, javax.swing.GroupLayout.DEFAULT_SIZE, javax.swing.GroupLayout.PREFERRED_SIZE)
                .addContainerGap())
        );
        lcdTabLayout.setVerticalGroup(
            lcdTabLayout.createParallelGroup(javax.swing.GroupLayout.Alignment.LEADING)
            .addGroup(lcdTabLayout.createSequentialGroup()
                .addContainerGap()
                .addGroup(lcdTabLayout.createParallelGroup(javax.swing.GroupLayout.Alignment.LEADING)
                    .addGroup(lcdTabLayout.createSequentialGroup()
                        .addComponent(userColorBtn)
                        .addPreferredGap(javax.swing.LayoutStyle.ComponentPlacement.UNRELATED)
                        .addGroup(lcdTabLayout.createParallelGroup(javax.swing.GroupLayout.Alignment.BASELINE)
                            .addComponent(brightnessName)
                            .addComponent(brightnessSpnr, javax.swing.GroupLayout.PREFERRED_SIZE, javax.swing.GroupLayout.DEFAULT_SIZE, javax.swing.GroupLayout.PREFERRED_SIZE))
                        .addPreferredGap(javax.swing.LayoutStyle.ComponentPlacement.UNRELATED)
                        .addGroup(lcdTabLayout.createParallelGroup(javax.swing.GroupLayout.Alignment.BASELINE)
                            .addComponent(readName)
                            .addComponent(readSpnr, javax.swing.GroupLayout.PREFERRED_SIZE, javax.swing.GroupLayout.DEFAULT_SIZE, javax.swing.GroupLayout.PREFERRED_SIZE))
                        .addGap(0, 0, Short.MAX_VALUE))
                    .addComponent(blinkPane, javax.swing.GroupLayout.DEFAULT_SIZE, javax.swing.GroupLayout.DEFAULT_SIZE, Short.MAX_VALUE)
                    .addComponent(rainbowPane, javax.swing.GroupLayout.DEFAULT_SIZE, javax.swing.GroupLayout.DEFAULT_SIZE, Short.MAX_VALUE))
                .addContainerGap())
        );

        deviceSettingsPane.addTab("LCD", lcdTab);

        fn1Name.setText("FN1:");

        fn1Cbx.setModel(new javax.swing.DefaultComboBoxModel(new String[] { "None", "Increment Brightness", "Toggle Rainbow Mode", "Switch Tweet", "Toggle Scroll" }));
        fn1Cbx.addItemListener(new java.awt.event.ItemListener() {
            public void itemStateChanged(java.awt.event.ItemEvent evt) {
                fn1CbxItemStateChanged(evt);
            }
        });

        fn2Name.setText("FN2:");

        fn2Cbx.setModel(new javax.swing.DefaultComboBoxModel(new String[] { "None", "Increment Brightness", "Toggle Rainbow Mode", "Switch Tweet", "Toggle Scroll" }));
        fn2Cbx.addItemListener(new java.awt.event.ItemListener() {
            public void itemStateChanged(java.awt.event.ItemEvent evt) {
                fn2CbxItemStateChanged(evt);
            }
        });

        javax.swing.GroupLayout buttonsTabLayout = new javax.swing.GroupLayout(buttonsTab);
        buttonsTab.setLayout(buttonsTabLayout);
        buttonsTabLayout.setHorizontalGroup(
            buttonsTabLayout.createParallelGroup(javax.swing.GroupLayout.Alignment.LEADING)
            .addGroup(buttonsTabLayout.createSequentialGroup()
                .addContainerGap()
                .addGroup(buttonsTabLayout.createParallelGroup(javax.swing.GroupLayout.Alignment.LEADING)
                    .addGroup(buttonsTabLayout.createSequentialGroup()
                        .addComponent(fn1Name)
                        .addPreferredGap(javax.swing.LayoutStyle.ComponentPlacement.UNRELATED)
                        .addComponent(fn1Cbx, javax.swing.GroupLayout.PREFERRED_SIZE, javax.swing.GroupLayout.DEFAULT_SIZE, javax.swing.GroupLayout.PREFERRED_SIZE))
                    .addGroup(buttonsTabLayout.createSequentialGroup()
                        .addComponent(fn2Name)
                        .addPreferredGap(javax.swing.LayoutStyle.ComponentPlacement.UNRELATED)
                        .addComponent(fn2Cbx, javax.swing.GroupLayout.PREFERRED_SIZE, javax.swing.GroupLayout.DEFAULT_SIZE, javax.swing.GroupLayout.PREFERRED_SIZE)))
                .addContainerGap(294, Short.MAX_VALUE))
        );
        buttonsTabLayout.setVerticalGroup(
            buttonsTabLayout.createParallelGroup(javax.swing.GroupLayout.Alignment.LEADING)
            .addGroup(buttonsTabLayout.createSequentialGroup()
                .addContainerGap()
                .addGroup(buttonsTabLayout.createParallelGroup(javax.swing.GroupLayout.Alignment.BASELINE)
                    .addComponent(fn1Name)
                    .addComponent(fn1Cbx, javax.swing.GroupLayout.PREFERRED_SIZE, javax.swing.GroupLayout.DEFAULT_SIZE, javax.swing.GroupLayout.PREFERRED_SIZE))
                .addGap(18, 18, 18)
                .addGroup(buttonsTabLayout.createParallelGroup(javax.swing.GroupLayout.Alignment.BASELINE)
                    .addComponent(fn2Name)
                    .addComponent(fn2Cbx, javax.swing.GroupLayout.PREFERRED_SIZE, javax.swing.GroupLayout.DEFAULT_SIZE, javax.swing.GroupLayout.PREFERRED_SIZE))
                .addContainerGap(51, Short.MAX_VALUE))
        );

        deviceSettingsPane.addTab("Buttons", buttonsTab);

        deviceApplyBtn.setText("Apply");
        deviceApplyBtn.setEnabled(false);
        deviceApplyBtn.addActionListener(new java.awt.event.ActionListener() {
            public void actionPerformed(java.awt.event.ActionEvent evt) {
                deviceApplyBtnActionPerformed(evt);
            }
        });

        deviceDefaultsBtn.setText("Defaults");
        deviceDefaultsBtn.addActionListener(new java.awt.event.ActionListener() {
            public void actionPerformed(java.awt.event.ActionEvent evt) {
                deviceDefaultsBtnActionPerformed(evt);
            }
        });

        javax.swing.GroupLayout deviceTabLayout = new javax.swing.GroupLayout(deviceTab);
        deviceTab.setLayout(deviceTabLayout);
        deviceTabLayout.setHorizontalGroup(
            deviceTabLayout.createParallelGroup(javax.swing.GroupLayout.Alignment.LEADING)
            .addGroup(javax.swing.GroupLayout.Alignment.TRAILING, deviceTabLayout.createSequentialGroup()
                .addContainerGap()
                .addGroup(deviceTabLayout.createParallelGroup(javax.swing.GroupLayout.Alignment.TRAILING)
                    .addComponent(deviceSettingsPane)
                    .addComponent(deviceInfoPane, javax.swing.GroupLayout.DEFAULT_SIZE, javax.swing.GroupLayout.DEFAULT_SIZE, Short.MAX_VALUE)
                    .addGroup(deviceTabLayout.createSequentialGroup()
                        .addComponent(deviceDefaultsBtn)
                        .addPreferredGap(javax.swing.LayoutStyle.ComponentPlacement.RELATED, javax.swing.GroupLayout.DEFAULT_SIZE, Short.MAX_VALUE)
                        .addComponent(deviceApplyBtn)))
                .addContainerGap())
        );
        deviceTabLayout.setVerticalGroup(
            deviceTabLayout.createParallelGroup(javax.swing.GroupLayout.Alignment.LEADING)
            .addGroup(deviceTabLayout.createSequentialGroup()
                .addContainerGap()
                .addComponent(deviceInfoPane, javax.swing.GroupLayout.PREFERRED_SIZE, javax.swing.GroupLayout.DEFAULT_SIZE, javax.swing.GroupLayout.PREFERRED_SIZE)
                .addPreferredGap(javax.swing.LayoutStyle.ComponentPlacement.UNRELATED)
                .addComponent(deviceSettingsPane)
                .addGap(15, 15, 15)
                .addGroup(deviceTabLayout.createParallelGroup(javax.swing.GroupLayout.Alignment.BASELINE)
                    .addComponent(deviceApplyBtn)
                    .addComponent(deviceDefaultsBtn))
                .addContainerGap())
        );

        tabbedPane.addTab("Device", deviceTab);

        followingPane.setBorder(javax.swing.BorderFactory.createTitledBorder("Following"));

        twtUserLst.addListSelectionListener(new javax.swing.event.ListSelectionListener() {
            public void valueChanged(javax.swing.event.ListSelectionEvent evt) {
                twtUserLstValueChanged(evt);
            }
        });
        jScrollPane2.setViewportView(twtUserLst);

        addUserBtn.setText("Add User");
        addUserBtn.addActionListener(new java.awt.event.ActionListener() {
            public void actionPerformed(java.awt.event.ActionEvent evt) {
                addUserBtnActionPerformed(evt);
            }
        });

        userInfoPane.setBorder(javax.swing.BorderFactory.createTitledBorder("User Info"));

        twtUserTitle.setText("Username:");

        twtUserIDTitle.setText("User ID:");

        remUserBtn.setText("Remove User");
        remUserBtn.setEnabled(false);
        remUserBtn.addActionListener(new java.awt.event.ActionListener() {
            public void actionPerformed(java.awt.event.ActionEvent evt) {
                remUserBtnActionPerformed(evt);
            }
        });

        twtProfileBtn.setText("View Profile");
        twtProfileBtn.setEnabled(false);
        twtProfileBtn.addActionListener(new java.awt.event.ActionListener() {
            public void actionPerformed(java.awt.event.ActionEvent evt) {
                twtProfileBtnActionPerformed(evt);
            }
        });

        twtLastTitle.setText("Last Tweet:");

        twtUserLbl.setText(" ");

        twtUserIDLbl.setText(" ");

        twtLastLbl.setText(" ");

        javax.swing.GroupLayout userInfoPaneLayout = new javax.swing.GroupLayout(userInfoPane);
        userInfoPane.setLayout(userInfoPaneLayout);
        userInfoPaneLayout.setHorizontalGroup(
            userInfoPaneLayout.createParallelGroup(javax.swing.GroupLayout.Alignment.LEADING)
            .addGroup(userInfoPaneLayout.createSequentialGroup()
                .addContainerGap()
                .addGroup(userInfoPaneLayout.createParallelGroup(javax.swing.GroupLayout.Alignment.LEADING)
                    .addGroup(javax.swing.GroupLayout.Alignment.TRAILING, userInfoPaneLayout.createSequentialGroup()
                        .addComponent(twtProfileBtn, javax.swing.GroupLayout.DEFAULT_SIZE, 109, Short.MAX_VALUE)
                        .addGap(46, 46, 46)
                        .addComponent(remUserBtn))
                    .addGroup(userInfoPaneLayout.createSequentialGroup()
                        .addGroup(userInfoPaneLayout.createParallelGroup(javax.swing.GroupLayout.Alignment.LEADING)
                            .addComponent(twtUserTitle)
                            .addComponent(twtUserIDTitle)
                            .addComponent(twtLastTitle))
                        .addGap(18, 18, 18)
                        .addGroup(userInfoPaneLayout.createParallelGroup(javax.swing.GroupLayout.Alignment.LEADING)
                            .addComponent(twtUserLbl, javax.swing.GroupLayout.DEFAULT_SIZE, javax.swing.GroupLayout.DEFAULT_SIZE, Short.MAX_VALUE)
                            .addComponent(twtUserIDLbl, javax.swing.GroupLayout.DEFAULT_SIZE, javax.swing.GroupLayout.DEFAULT_SIZE, Short.MAX_VALUE)
                            .addComponent(twtLastLbl, javax.swing.GroupLayout.DEFAULT_SIZE, javax.swing.GroupLayout.DEFAULT_SIZE, Short.MAX_VALUE))))
                .addContainerGap())
        );
        userInfoPaneLayout.setVerticalGroup(
            userInfoPaneLayout.createParallelGroup(javax.swing.GroupLayout.Alignment.LEADING)
            .addGroup(userInfoPaneLayout.createSequentialGroup()
                .addContainerGap()
                .addGroup(userInfoPaneLayout.createParallelGroup(javax.swing.GroupLayout.Alignment.BASELINE)
                    .addComponent(twtUserTitle)
                    .addComponent(twtUserLbl))
                .addPreferredGap(javax.swing.LayoutStyle.ComponentPlacement.RELATED)
                .addGroup(userInfoPaneLayout.createParallelGroup(javax.swing.GroupLayout.Alignment.BASELINE)
                    .addComponent(twtUserIDTitle)
                    .addComponent(twtUserIDLbl))
                .addPreferredGap(javax.swing.LayoutStyle.ComponentPlacement.RELATED)
                .addGroup(userInfoPaneLayout.createParallelGroup(javax.swing.GroupLayout.Alignment.BASELINE)
                    .addComponent(twtLastTitle)
                    .addComponent(twtLastLbl))
                .addPreferredGap(javax.swing.LayoutStyle.ComponentPlacement.RELATED, javax.swing.GroupLayout.DEFAULT_SIZE, Short.MAX_VALUE)
                .addGroup(userInfoPaneLayout.createParallelGroup(javax.swing.GroupLayout.Alignment.BASELINE)
                    .addComponent(remUserBtn)
                    .addComponent(twtProfileBtn))
                .addContainerGap())
        );

        javax.swing.GroupLayout followingPaneLayout = new javax.swing.GroupLayout(followingPane);
        followingPane.setLayout(followingPaneLayout);
        followingPaneLayout.setHorizontalGroup(
            followingPaneLayout.createParallelGroup(javax.swing.GroupLayout.Alignment.LEADING)
            .addGroup(followingPaneLayout.createSequentialGroup()
                .addContainerGap()
                .addGroup(followingPaneLayout.createParallelGroup(javax.swing.GroupLayout.Alignment.LEADING, false)
                    .addComponent(jScrollPane2, javax.swing.GroupLayout.DEFAULT_SIZE, 150, Short.MAX_VALUE)
                    .addComponent(addUserBtn, javax.swing.GroupLayout.DEFAULT_SIZE, javax.swing.GroupLayout.DEFAULT_SIZE, Short.MAX_VALUE))
                .addGap(18, 18, 18)
                .addComponent(userInfoPane, javax.swing.GroupLayout.DEFAULT_SIZE, javax.swing.GroupLayout.DEFAULT_SIZE, Short.MAX_VALUE)
                .addContainerGap())
        );
        followingPaneLayout.setVerticalGroup(
            followingPaneLayout.createParallelGroup(javax.swing.GroupLayout.Alignment.LEADING)
            .addGroup(followingPaneLayout.createSequentialGroup()
                .addContainerGap()
                .addGroup(followingPaneLayout.createParallelGroup(javax.swing.GroupLayout.Alignment.LEADING)
                    .addComponent(userInfoPane, javax.swing.GroupLayout.DEFAULT_SIZE, javax.swing.GroupLayout.DEFAULT_SIZE, Short.MAX_VALUE)
                    .addGroup(followingPaneLayout.createSequentialGroup()
                        .addComponent(jScrollPane2, javax.swing.GroupLayout.DEFAULT_SIZE, 155, Short.MAX_VALUE)
                        .addPreferredGap(javax.swing.LayoutStyle.ComponentPlacement.RELATED)
                        .addComponent(addUserBtn)))
                .addContainerGap())
        );

        twitterApplyBtn.setText("Apply");
        twitterApplyBtn.setEnabled(false);

        twitterDefaultsBtn.setText("Defaults");

        javax.swing.GroupLayout twitterTabLayout = new javax.swing.GroupLayout(twitterTab);
        twitterTab.setLayout(twitterTabLayout);
        twitterTabLayout.setHorizontalGroup(
            twitterTabLayout.createParallelGroup(javax.swing.GroupLayout.Alignment.LEADING)
            .addGroup(twitterTabLayout.createSequentialGroup()
                .addContainerGap()
                .addGroup(twitterTabLayout.createParallelGroup(javax.swing.GroupLayout.Alignment.LEADING)
                    .addComponent(followingPane, javax.swing.GroupLayout.DEFAULT_SIZE, javax.swing.GroupLayout.DEFAULT_SIZE, Short.MAX_VALUE)
                    .addGroup(javax.swing.GroupLayout.Alignment.TRAILING, twitterTabLayout.createSequentialGroup()
                        .addComponent(twitterDefaultsBtn)
                        .addPreferredGap(javax.swing.LayoutStyle.ComponentPlacement.RELATED, javax.swing.GroupLayout.DEFAULT_SIZE, Short.MAX_VALUE)
                        .addComponent(twitterApplyBtn)))
                .addContainerGap())
        );
        twitterTabLayout.setVerticalGroup(
            twitterTabLayout.createParallelGroup(javax.swing.GroupLayout.Alignment.LEADING)
            .addGroup(twitterTabLayout.createSequentialGroup()
                .addContainerGap()
                .addComponent(followingPane, javax.swing.GroupLayout.DEFAULT_SIZE, javax.swing.GroupLayout.DEFAULT_SIZE, Short.MAX_VALUE)
                .addPreferredGap(javax.swing.LayoutStyle.ComponentPlacement.UNRELATED)
                .addGroup(twitterTabLayout.createParallelGroup(javax.swing.GroupLayout.Alignment.BASELINE)
                    .addComponent(twitterApplyBtn)
                    .addComponent(twitterDefaultsBtn))
                .addContainerGap())
        );

        tabbedPane.addTab("Twitter", twitterTab);

        javax.swing.GroupLayout settingsTabLayout = new javax.swing.GroupLayout(settingsTab);
        settingsTab.setLayout(settingsTabLayout);
        settingsTabLayout.setHorizontalGroup(
            settingsTabLayout.createParallelGroup(javax.swing.GroupLayout.Alignment.LEADING)
            .addGap(0, 528, Short.MAX_VALUE)
        );
        settingsTabLayout.setVerticalGroup(
            settingsTabLayout.createParallelGroup(javax.swing.GroupLayout.Alignment.LEADING)
            .addGap(0, 302, Short.MAX_VALUE)
        );

        tabbedPane.addTab("Settings", settingsTab);

        helpMenu.setText("Help");

        wikiMenuItm.setText("See the Wiki");
        wikiMenuItm.addActionListener(new java.awt.event.ActionListener() {
            public void actionPerformed(java.awt.event.ActionEvent evt) {
                wikiMenuItmActionPerformed(evt);
            }
        });
        helpMenu.add(wikiMenuItm);

        bugMenuItm.setText("Report a bug");
        bugMenuItm.addActionListener(new java.awt.event.ActionListener() {
            public void actionPerformed(java.awt.event.ActionEvent evt) {
                bugMenuItmActionPerformed(evt);
            }
        });
        helpMenu.add(bugMenuItm);
        helpMenu.add(jSeparator1);

        followMenuItm.setText("Follow TwiScn on Twitter");
        followMenuItm.addActionListener(new java.awt.event.ActionListener() {
            public void actionPerformed(java.awt.event.ActionEvent evt) {
                followMenuItmActionPerformed(evt);
            }
        });
        helpMenu.add(followMenuItm);

        aboutMenuItm.setText("About TwiScn");
        aboutMenuItm.addActionListener(new java.awt.event.ActionListener() {
            public void actionPerformed(java.awt.event.ActionEvent evt) {
                aboutMenuItmActionPerformed(evt);
            }
        });
        helpMenu.add(aboutMenuItm);

        jMenuBar1.add(helpMenu);

        setJMenuBar(jMenuBar1);

        javax.swing.GroupLayout layout = new javax.swing.GroupLayout(getContentPane());
        getContentPane().setLayout(layout);
        layout.setHorizontalGroup(
            layout.createParallelGroup(javax.swing.GroupLayout.Alignment.LEADING)
            .addGroup(layout.createSequentialGroup()
                .addContainerGap()
                .addGroup(layout.createParallelGroup(javax.swing.GroupLayout.Alignment.LEADING)
                    .addComponent(tabbedPane, javax.swing.GroupLayout.PREFERRED_SIZE, 0, Short.MAX_VALUE)
                    .addGroup(javax.swing.GroupLayout.Alignment.TRAILING, layout.createSequentialGroup()
                        .addGap(0, 0, Short.MAX_VALUE)
                        .addComponent(statusPane, javax.swing.GroupLayout.PREFERRED_SIZE, javax.swing.GroupLayout.DEFAULT_SIZE, javax.swing.GroupLayout.PREFERRED_SIZE)))
                .addContainerGap())
        );
        layout.setVerticalGroup(
            layout.createParallelGroup(javax.swing.GroupLayout.Alignment.LEADING)
            .addGroup(javax.swing.GroupLayout.Alignment.TRAILING, layout.createSequentialGroup()
                .addContainerGap()
                .addComponent(tabbedPane, javax.swing.GroupLayout.PREFERRED_SIZE, javax.swing.GroupLayout.DEFAULT_SIZE, javax.swing.GroupLayout.PREFERRED_SIZE)
                .addPreferredGap(javax.swing.LayoutStyle.ComponentPlacement.RELATED, 24, Short.MAX_VALUE)
                .addComponent(statusPane, javax.swing.GroupLayout.PREFERRED_SIZE, javax.swing.GroupLayout.DEFAULT_SIZE, javax.swing.GroupLayout.PREFERRED_SIZE)
                .addGap(11, 11, 11))
        );

        pack();
    }// </editor-fold>//GEN-END:initComponents

    private void rainEnabledChkItemStateChanged(java.awt.event.ItemEvent evt) {//GEN-FIRST:event_rainEnabledChkItemStateChanged
        //used to enable/disable components when the rainbow mode enable checkbox is modified
        if(rainEnabledChk.isSelected()) {
            rainSpeedSpnr.setEnabled(true);
            userColorBtn.setEnabled(false);
        }
        else {
            rainSpeedSpnr.setEnabled(false);
            userColorBtn.setEnabled(true);
        }
        deviceApplyBtn.setEnabled(true);                                        //enable the apply button, since there has been a new change
    }//GEN-LAST:event_rainEnabledChkItemStateChanged

    private void fn1CbxItemStateChanged(java.awt.event.ItemEvent evt) {//GEN-FIRST:event_fn1CbxItemStateChanged
        deviceApplyBtn.setEnabled(true);                                        //enable the apply button when the fn1 action combobox is changed
    }//GEN-LAST:event_fn1CbxItemStateChanged

    private void fn2CbxItemStateChanged(java.awt.event.ItemEvent evt) {//GEN-FIRST:event_fn2CbxItemStateChanged
        deviceApplyBtn.setEnabled(true);                                        //enable the apply button when the fn2 action combobox is changed
    }//GEN-LAST:event_fn2CbxItemStateChanged

    private void userColorBtnActionPerformed(java.awt.event.ActionEvent evt) {//GEN-FIRST:event_userColorBtnActionPerformed
        //used to set the user selected LCD color
        Color selectedColor = JColorChooser.showDialog(this, "Select a Color", opt.getLCDColor());
        //opens the color chooser dialog, and stores the selected color
        if(selectedColor != null) {                                             //if the user actually chose a new color
            opt.setLCDColor(selectedColor);                                     //give Options that new color
            deviceApplyBtn.setEnabled(true);                                    //enable the apply button, since there has been a new change
        }      
    }//GEN-LAST:event_userColorBtnActionPerformed

    private void blinkColorBtnActionPerformed(java.awt.event.ActionEvent evt) {//GEN-FIRST:event_blinkColorBtnActionPerformed
        //used to set the user selected LCD blink color
        Color selectedColor = JColorChooser.showDialog(this, "Select a Color", opt.getBlinkColor());
        //opens the color chooser dialog, and stores the selected color
        if(selectedColor != null) {                                             //if the user actually chose a new color
            opt.setBlinkColor(selectedColor);                                   //give Options that new color
            deviceApplyBtn.setEnabled(true);                                    //enable the apply button, since there has been a new change
        }  
    }//GEN-LAST:event_blinkColorBtnActionPerformed

    private void deviceApplyBtnActionPerformed(java.awt.event.ActionEvent evt) {//GEN-FIRST:event_deviceApplyBtnActionPerformed
        //used to get all device options from the GUI, set them into Options, and then apply them to the device
        opt.setBrightness((int)brightnessSpnr.getValue());
        opt.setBlinkState(blinkEnabledChk.isSelected());
        opt.setRnbwState(rainEnabledChk.isSelected());
        opt.setRnbwSpd((int)rainSpeedSpnr.getValue());
        opt.setReadTime((int)readSpnr.getValue());
        opt.setFn1Action((byte)fn1Cbx.getSelectedIndex());
        opt.setFn2Action((byte)fn2Cbx.getSelectedIndex());
        if(!UsbHidComms.connected()) {                                          //if the device is not connected yet
            addStatusLine("Settings will be applied when device is connected");
        }
        else {
            addStatusLine("Settings sent to device");
        }
        applyOptions = true;                                                    //tell the program to apply the options asap
        deviceApplyBtn.setEnabled(false);                                       //disable the apply button, just applied changes
    }//GEN-LAST:event_deviceApplyBtnActionPerformed

    private void brightnessSpnrStateChanged(javax.swing.event.ChangeEvent evt) {//GEN-FIRST:event_brightnessSpnrStateChanged
        deviceApplyBtn.setEnabled(true);                                        //enable the apply button, since there has been a new change
    }//GEN-LAST:event_brightnessSpnrStateChanged

    private void readSpnrStateChanged(javax.swing.event.ChangeEvent evt) {//GEN-FIRST:event_readSpnrStateChanged
        deviceApplyBtn.setEnabled(true);                                        //enable the apply button, since there has been a new change
    }//GEN-LAST:event_readSpnrStateChanged

    private void rainSpeedSpnrStateChanged(javax.swing.event.ChangeEvent evt) {//GEN-FIRST:event_rainSpeedSpnrStateChanged
        deviceApplyBtn.setEnabled(true);                                        //enable the apply button, since there has been a new change
    }//GEN-LAST:event_rainSpeedSpnrStateChanged

    private void blinkEnabledChkItemStateChanged(java.awt.event.ItemEvent evt) {//GEN-FIRST:event_blinkEnabledChkItemStateChanged
        //used to enable/disable components when the blink enable checkbox is modified
        if(blinkEnabledChk.isSelected()) {
            blinkColorBtn.setEnabled(true);
        }
        else {
            blinkColorBtn.setEnabled(false);
        }
        deviceApplyBtn.setEnabled(true);                                        //enable the apply button, since there has been a new change
    }//GEN-LAST:event_blinkEnabledChkItemStateChanged

    private void deviceDefaultsBtnActionPerformed(java.awt.event.ActionEvent evt) {//GEN-FIRST:event_deviceDefaultsBtnActionPerformed
        setDeviceDefaults();                                                    //apply the default device settings
        deviceApplyBtn.setEnabled(true);                                        //enable the apply button, since there has been a new change
    }//GEN-LAST:event_deviceDefaultsBtnActionPerformed

    private void twtUserLstValueChanged(javax.swing.event.ListSelectionEvent evt) {//GEN-FIRST:event_twtUserLstValueChanged
        //calls when the user selects a user in the user list
        String out = String.valueOf(twtUserLst.getSelectedValue());             //get the selected username out
        getUserInfo(out);                                                       //display that user's info
    }//GEN-LAST:event_twtUserLstValueChanged

    private void twtProfileBtnActionPerformed(java.awt.event.ActionEvent evt) {//GEN-FIRST:event_twtProfileBtnActionPerformed
        String out = String.valueOf(twtUserLst.getSelectedValue());             //get the selected username out
        openProfile(out);                                                       //open the user's profile page
    }//GEN-LAST:event_twtProfileBtnActionPerformed

    private void remUserBtnActionPerformed(java.awt.event.ActionEvent evt) {//GEN-FIRST:event_remUserBtnActionPerformed
        //removes a selected user from the user list
        String out = String.valueOf(twtUserLst.getSelectedValue());             //get the selected username out
        try {  
            opt.delFollowUser(twt.getUserID(out));                              //try to convert the username to an ID, then tell options to remove it
            refreshUserList();        
            //clear the user's info
            twtUserIDLbl.setText("");
            twtUserLbl.setText("");
            twtLastLbl.setText("");
            twtProfileBtn.setEnabled(false);                                     
            remUserBtn.setEnabled(false);
        } catch (TwitterException ex) {                                         //catch any twitter errors
            Logger.getLogger(Gui.class.getName()).log(Level.SEVERE, null, ex);
        }
    }//GEN-LAST:event_remUserBtnActionPerformed

    private void addUserBtnActionPerformed(java.awt.event.ActionEvent evt) {//GEN-FIRST:event_addUserBtnActionPerformed
        //adds a user to the user list
        String input = null;
        //spawn an input dialog requesting the username to add
        input = JOptionPane.showInputDialog(null, 
           "Type in the user name found in the user's profile URL:", 
           "Add User", JOptionPane.QUESTION_MESSAGE,null,null,"").toString();
        
        if(input != null) {                                                     //if the user gave us something to use
            if(twt.checkExists(input)) {                                        //check if the username exists
                try {
                    long ID = twt.getUserID(input);                             //try to convert the username to an ID
                    if(!twt.getProtected(ID)) {                                 //if the user is not protected
                        opt.addFollowUser(ID);                                  //add the userID to the list
                        refreshUserList();
                        //clear the user's info
                        twtUserIDLbl.setText("");
                        twtUserLbl.setText("");
                        twtLastLbl.setText("");
                        twtProfileBtn.setEnabled(false);                                     
                        remUserBtn.setEnabled(false);
                    }
                    else {                                                      //user's tweets are protected, cant do much with that
                        JOptionPane.showMessageDialog(null, input + "'s tweets are protected, unable to add user.", "Error", JOptionPane.ERROR_MESSAGE, null);
                    }
                } catch (TwitterException ex) {                                 //catch any twitter errors
                    Logger.getLogger(Gui.class.getName()).log(Level.SEVERE, null, ex);
                }
            }
            else {                                                              //couldn't find the user
                JOptionPane.showMessageDialog(null, "Failed to find the user.", "Error", JOptionPane.ERROR_MESSAGE, null);
            }
        }       
    }//GEN-LAST:event_addUserBtnActionPerformed

    private void wikiMenuItmActionPerformed(java.awt.event.ActionEvent evt) {//GEN-FIRST:event_wikiMenuItmActionPerformed
        //used to go the wiki
        openWebsite("https://github.com/HybridAir/TwiScnHost/wiki");
    }//GEN-LAST:event_wikiMenuItmActionPerformed

    private void bugMenuItmActionPerformed(java.awt.event.ActionEvent evt) {//GEN-FIRST:event_bugMenuItmActionPerformed
        //used to go the issues page
        openWebsite("https://github.com/HybridAir/TwiScnHost/issues");
    }//GEN-LAST:event_bugMenuItmActionPerformed

    private void followMenuItmActionPerformed(java.awt.event.ActionEvent evt) {//GEN-FIRST:event_followMenuItmActionPerformed
        //used to go the issues page
        openWebsite("https://twitter.com/twiscn");
    }//GEN-LAST:event_followMenuItmActionPerformed

    private void aboutMenuItmActionPerformed(java.awt.event.ActionEvent evt) {//GEN-FIRST:event_aboutMenuItmActionPerformed
        AboutPanel about = new AboutPanel();
    }//GEN-LAST:event_aboutMenuItmActionPerformed

    // Variables declaration - do not modify//GEN-BEGIN:variables
    private javax.swing.JMenuItem aboutMenuItm;
    private javax.swing.JButton addUserBtn;
    private javax.swing.JButton blinkColorBtn;
    private javax.swing.JCheckBox blinkEnabledChk;
    private javax.swing.JPanel blinkPane;
    private javax.swing.JLabel brightnessName;
    private javax.swing.JSpinner brightnessSpnr;
    private javax.swing.JMenuItem bugMenuItm;
    private javax.swing.JPanel buttonsTab;
    private javax.swing.JFrame colorFrame;
    private javax.swing.JButton deviceApplyBtn;
    private javax.swing.JButton deviceDefaultsBtn;
    private javax.swing.JPanel deviceInfoPane;
    private javax.swing.JTabbedPane deviceSettingsPane;
    private javax.swing.JPanel deviceTab;
    protected javax.swing.JLabel firmLbl;
    private javax.swing.JLabel firmwareName;
    private javax.swing.JComboBox fn1Cbx;
    private javax.swing.JLabel fn1Name;
    private javax.swing.JComboBox fn2Cbx;
    private javax.swing.JLabel fn2Name;
    private javax.swing.JMenuItem followMenuItm;
    private javax.swing.JPanel followingPane;
    protected javax.swing.JLabel hardLbl;
    private javax.swing.JLabel hardwareName;
    private javax.swing.JMenu helpMenu;
    private javax.swing.JColorChooser jColorChooser1;
    private javax.swing.JMenuBar jMenuBar1;
    private javax.swing.JScrollPane jScrollPane2;
    private javax.swing.JPopupMenu.Separator jSeparator1;
    private javax.swing.JPanel lcdTab;
    private javax.swing.JCheckBox rainEnabledChk;
    private javax.swing.JLabel rainSpeedName;
    private javax.swing.JSpinner rainSpeedSpnr;
    private javax.swing.JPanel rainbowPane;
    private javax.swing.JLabel readName;
    private javax.swing.JSpinner readSpnr;
    private javax.swing.JButton remUserBtn;
    private javax.swing.JPanel settingsTab;
    protected javax.swing.JLabel statusLbl;
    private javax.swing.JLabel statusName;
    private javax.swing.JPanel statusPane;
    private javax.swing.JScrollPane statusScrollPane;
    protected javax.swing.JTextArea statusTxt;
    private javax.swing.JTabbedPane tabbedPane;
    private javax.swing.JButton twitterApplyBtn;
    private javax.swing.JButton twitterDefaultsBtn;
    private javax.swing.JPanel twitterTab;
    private javax.swing.JLabel twtLastLbl;
    private javax.swing.JLabel twtLastTitle;
    private javax.swing.JButton twtProfileBtn;
    private javax.swing.JLabel twtUserIDLbl;
    private javax.swing.JLabel twtUserIDTitle;
    private javax.swing.JLabel twtUserLbl;
    private javax.swing.JList twtUserLst;
    private javax.swing.JLabel twtUserTitle;
    private javax.swing.JButton userColorBtn;
    private javax.swing.JPanel userInfoPane;
    private javax.swing.JMenuItem wikiMenuItm;
    // End of variables declaration//GEN-END:variables
}
