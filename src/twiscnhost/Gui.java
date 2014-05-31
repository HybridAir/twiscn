package twiscnhost;

import java.text.SimpleDateFormat;
import java.util.Date;
import java.util.logging.Level;
import java.util.logging.Logger;
import javax.swing.text.BadLocationException;

public class Gui extends javax.swing.JFrame {
    
    public Gui() {                                                              //default constructor, gets everything started
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
      
        setVisible(true);                                                       //make the primary window visible
        initComponents();                                                       //get all the components going
    }  
    
    @SuppressWarnings("unchecked")
    // <editor-fold defaultstate="collapsed" desc="Generated Code">//GEN-BEGIN:initComponents
    private void initComponents() {

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
        jPanel5 = new javax.swing.JPanel();
        jScrollPane2 = new javax.swing.JScrollPane();
        jList1 = new javax.swing.JList();
        jButton2 = new javax.swing.JButton();
        jPanel6 = new javax.swing.JPanel();
        jLabel1 = new javax.swing.JLabel();
        jLabel2 = new javax.swing.JLabel();
        jButton1 = new javax.swing.JButton();
        jButton5 = new javax.swing.JButton();
        jLabel9 = new javax.swing.JLabel();
        jButton3 = new javax.swing.JButton();
        jButton4 = new javax.swing.JButton();
        settingsTab = new javax.swing.JPanel();
        jMenuBar1 = new javax.swing.JMenuBar();
        helpMenu = new javax.swing.JMenu();
        wikiMenuItm = new javax.swing.JMenuItem();
        bugMenuItm = new javax.swing.JMenuItem();
        jSeparator1 = new javax.swing.JPopupMenu.Separator();
        followMenuItm = new javax.swing.JMenuItem();
        aboutMenuItm = new javax.swing.JMenuItem();

        setDefaultCloseOperation(javax.swing.WindowConstants.EXIT_ON_CLOSE);

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

        blinkPane.setBorder(javax.swing.BorderFactory.createTitledBorder("Tweet Blink"));

        blinkEnabledChk.setText("Enabled");

        blinkColorBtn.setText("Color");
        blinkColorBtn.setEnabled(false);

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

        userColorBtn.setText("Color");

        rainbowPane.setBorder(javax.swing.BorderFactory.createTitledBorder("Rainbow Mode"));

        rainEnabledChk.setText("Enabled");

        rainSpeedName.setText("Speed:");

        rainSpeedSpnr.setEnabled(false);

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
                        .addComponent(rainSpeedSpnr, javax.swing.GroupLayout.DEFAULT_SIZE, 61, Short.MAX_VALUE)))
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
                .addGroup(lcdTabLayout.createParallelGroup(javax.swing.GroupLayout.Alignment.TRAILING)
                    .addComponent(userColorBtn, javax.swing.GroupLayout.DEFAULT_SIZE, javax.swing.GroupLayout.DEFAULT_SIZE, Short.MAX_VALUE)
                    .addGroup(lcdTabLayout.createSequentialGroup()
                        .addGroup(lcdTabLayout.createParallelGroup(javax.swing.GroupLayout.Alignment.LEADING)
                            .addComponent(brightnessName)
                            .addComponent(readName))
                        .addGap(18, 18, 18)
                        .addGroup(lcdTabLayout.createParallelGroup(javax.swing.GroupLayout.Alignment.LEADING)
                            .addComponent(readSpnr, javax.swing.GroupLayout.DEFAULT_SIZE, 61, Short.MAX_VALUE)
                            .addComponent(brightnessSpnr))))
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
        fn1Cbx.addActionListener(new java.awt.event.ActionListener() {
            public void actionPerformed(java.awt.event.ActionEvent evt) {
                fn1CbxActionPerformed(evt);
            }
        });

        fn2Name.setText("FN2:");

        fn2Cbx.setModel(new javax.swing.DefaultComboBoxModel(new String[] { "None", "Increment Brightness", "Toggle Rainbow Mode", "Switch Tweet", "Toggle Scroll" }));
        fn2Cbx.addActionListener(new java.awt.event.ActionListener() {
            public void actionPerformed(java.awt.event.ActionEvent evt) {
                fn2CbxActionPerformed(evt);
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

        deviceDefaultsBtn.setText("Defaults");

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

        jPanel5.setBorder(javax.swing.BorderFactory.createTitledBorder("Following"));

        jList1.setModel(new javax.swing.AbstractListModel() {
            String[] strings = { "Item 1", "Item 2", "Item 3", "Item 4", "Item 5" };
            public int getSize() { return strings.length; }
            public Object getElementAt(int i) { return strings[i]; }
        });
        jScrollPane2.setViewportView(jList1);

        jButton2.setText("Add User");

        jPanel6.setBorder(javax.swing.BorderFactory.createTitledBorder("User Info"));

        jLabel1.setText("Username:");

        jLabel2.setText("User ID:");

        jButton1.setText("Remove User");
        jButton1.setEnabled(false);

        jButton5.setText("View Profile");
        jButton5.setEnabled(false);

        jLabel9.setText("Last Tweet:");

        javax.swing.GroupLayout jPanel6Layout = new javax.swing.GroupLayout(jPanel6);
        jPanel6.setLayout(jPanel6Layout);
        jPanel6Layout.setHorizontalGroup(
            jPanel6Layout.createParallelGroup(javax.swing.GroupLayout.Alignment.LEADING)
            .addGroup(jPanel6Layout.createSequentialGroup()
                .addContainerGap()
                .addGroup(jPanel6Layout.createParallelGroup(javax.swing.GroupLayout.Alignment.LEADING)
                    .addGroup(javax.swing.GroupLayout.Alignment.TRAILING, jPanel6Layout.createSequentialGroup()
                        .addComponent(jButton5, javax.swing.GroupLayout.DEFAULT_SIZE, 109, Short.MAX_VALUE)
                        .addGap(46, 46, 46)
                        .addComponent(jButton1))
                    .addGroup(jPanel6Layout.createSequentialGroup()
                        .addGroup(jPanel6Layout.createParallelGroup(javax.swing.GroupLayout.Alignment.LEADING)
                            .addComponent(jLabel1)
                            .addComponent(jLabel2)
                            .addComponent(jLabel9))
                        .addGap(0, 0, Short.MAX_VALUE)))
                .addContainerGap())
        );
        jPanel6Layout.setVerticalGroup(
            jPanel6Layout.createParallelGroup(javax.swing.GroupLayout.Alignment.LEADING)
            .addGroup(jPanel6Layout.createSequentialGroup()
                .addContainerGap()
                .addComponent(jLabel1)
                .addPreferredGap(javax.swing.LayoutStyle.ComponentPlacement.RELATED)
                .addComponent(jLabel2)
                .addPreferredGap(javax.swing.LayoutStyle.ComponentPlacement.RELATED)
                .addComponent(jLabel9)
                .addPreferredGap(javax.swing.LayoutStyle.ComponentPlacement.RELATED, javax.swing.GroupLayout.DEFAULT_SIZE, Short.MAX_VALUE)
                .addGroup(jPanel6Layout.createParallelGroup(javax.swing.GroupLayout.Alignment.BASELINE)
                    .addComponent(jButton1)
                    .addComponent(jButton5))
                .addContainerGap())
        );

        javax.swing.GroupLayout jPanel5Layout = new javax.swing.GroupLayout(jPanel5);
        jPanel5.setLayout(jPanel5Layout);
        jPanel5Layout.setHorizontalGroup(
            jPanel5Layout.createParallelGroup(javax.swing.GroupLayout.Alignment.LEADING)
            .addGroup(jPanel5Layout.createSequentialGroup()
                .addContainerGap()
                .addGroup(jPanel5Layout.createParallelGroup(javax.swing.GroupLayout.Alignment.LEADING, false)
                    .addComponent(jScrollPane2)
                    .addComponent(jButton2, javax.swing.GroupLayout.DEFAULT_SIZE, 150, Short.MAX_VALUE))
                .addGap(18, 18, 18)
                .addComponent(jPanel6, javax.swing.GroupLayout.DEFAULT_SIZE, javax.swing.GroupLayout.DEFAULT_SIZE, Short.MAX_VALUE)
                .addContainerGap())
        );
        jPanel5Layout.setVerticalGroup(
            jPanel5Layout.createParallelGroup(javax.swing.GroupLayout.Alignment.LEADING)
            .addGroup(jPanel5Layout.createSequentialGroup()
                .addContainerGap()
                .addGroup(jPanel5Layout.createParallelGroup(javax.swing.GroupLayout.Alignment.LEADING)
                    .addComponent(jPanel6, javax.swing.GroupLayout.DEFAULT_SIZE, javax.swing.GroupLayout.DEFAULT_SIZE, Short.MAX_VALUE)
                    .addGroup(jPanel5Layout.createSequentialGroup()
                        .addComponent(jScrollPane2, javax.swing.GroupLayout.DEFAULT_SIZE, 156, Short.MAX_VALUE)
                        .addPreferredGap(javax.swing.LayoutStyle.ComponentPlacement.RELATED)
                        .addComponent(jButton2)))
                .addContainerGap())
        );

        jButton3.setText("Apply");
        jButton3.setEnabled(false);

        jButton4.setText("Defaults");

        javax.swing.GroupLayout twitterTabLayout = new javax.swing.GroupLayout(twitterTab);
        twitterTab.setLayout(twitterTabLayout);
        twitterTabLayout.setHorizontalGroup(
            twitterTabLayout.createParallelGroup(javax.swing.GroupLayout.Alignment.LEADING)
            .addGroup(twitterTabLayout.createSequentialGroup()
                .addContainerGap()
                .addGroup(twitterTabLayout.createParallelGroup(javax.swing.GroupLayout.Alignment.LEADING)
                    .addComponent(jPanel5, javax.swing.GroupLayout.DEFAULT_SIZE, javax.swing.GroupLayout.DEFAULT_SIZE, Short.MAX_VALUE)
                    .addGroup(javax.swing.GroupLayout.Alignment.TRAILING, twitterTabLayout.createSequentialGroup()
                        .addComponent(jButton4)
                        .addPreferredGap(javax.swing.LayoutStyle.ComponentPlacement.RELATED, javax.swing.GroupLayout.DEFAULT_SIZE, Short.MAX_VALUE)
                        .addComponent(jButton3)))
                .addContainerGap())
        );
        twitterTabLayout.setVerticalGroup(
            twitterTabLayout.createParallelGroup(javax.swing.GroupLayout.Alignment.LEADING)
            .addGroup(twitterTabLayout.createSequentialGroup()
                .addContainerGap()
                .addComponent(jPanel5, javax.swing.GroupLayout.DEFAULT_SIZE, javax.swing.GroupLayout.DEFAULT_SIZE, Short.MAX_VALUE)
                .addPreferredGap(javax.swing.LayoutStyle.ComponentPlacement.RELATED)
                .addGroup(twitterTabLayout.createParallelGroup(javax.swing.GroupLayout.Alignment.BASELINE)
                    .addComponent(jButton3)
                    .addComponent(jButton4))
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
        helpMenu.add(wikiMenuItm);

        bugMenuItm.setText("Report a bug");
        helpMenu.add(bugMenuItm);
        helpMenu.add(jSeparator1);

        followMenuItm.setText("Follow TwiScn on Twitter");
        helpMenu.add(followMenuItm);

        aboutMenuItm.setText("About TwiScn");
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
                .addComponent(tabbedPane, javax.swing.GroupLayout.PREFERRED_SIZE, 327, javax.swing.GroupLayout.PREFERRED_SIZE)
                .addPreferredGap(javax.swing.LayoutStyle.ComponentPlacement.RELATED, javax.swing.GroupLayout.DEFAULT_SIZE, Short.MAX_VALUE)
                .addComponent(statusPane, javax.swing.GroupLayout.PREFERRED_SIZE, javax.swing.GroupLayout.DEFAULT_SIZE, javax.swing.GroupLayout.PREFERRED_SIZE)
                .addGap(11, 11, 11))
        );

        pack();
    }// </editor-fold>//GEN-END:initComponents

    private void fn1CbxActionPerformed(java.awt.event.ActionEvent evt) {//GEN-FIRST:event_fn1CbxActionPerformed
        // TODO add your handling code here:
    }//GEN-LAST:event_fn1CbxActionPerformed

    private void fn2CbxActionPerformed(java.awt.event.ActionEvent evt) {//GEN-FIRST:event_fn2CbxActionPerformed
        // TODO add your handling code here:
    }//GEN-LAST:event_fn2CbxActionPerformed

    // Variables declaration - do not modify//GEN-BEGIN:variables
    private javax.swing.JMenuItem aboutMenuItm;
    private javax.swing.JButton blinkColorBtn;
    private javax.swing.JCheckBox blinkEnabledChk;
    private javax.swing.JPanel blinkPane;
    private javax.swing.JLabel brightnessName;
    private javax.swing.JSpinner brightnessSpnr;
    private javax.swing.JMenuItem bugMenuItm;
    private javax.swing.JPanel buttonsTab;
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
    protected javax.swing.JLabel hardLbl;
    private javax.swing.JLabel hardwareName;
    private javax.swing.JMenu helpMenu;
    private javax.swing.JButton jButton1;
    private javax.swing.JButton jButton2;
    private javax.swing.JButton jButton3;
    private javax.swing.JButton jButton4;
    private javax.swing.JButton jButton5;
    private javax.swing.JLabel jLabel1;
    private javax.swing.JLabel jLabel2;
    private javax.swing.JLabel jLabel9;
    private javax.swing.JList jList1;
    private javax.swing.JMenuBar jMenuBar1;
    private javax.swing.JPanel jPanel5;
    private javax.swing.JPanel jPanel6;
    private javax.swing.JScrollPane jScrollPane2;
    private javax.swing.JPopupMenu.Separator jSeparator1;
    private javax.swing.JPanel lcdTab;
    private javax.swing.JCheckBox rainEnabledChk;
    private javax.swing.JLabel rainSpeedName;
    private javax.swing.JSpinner rainSpeedSpnr;
    private javax.swing.JPanel rainbowPane;
    private javax.swing.JLabel readName;
    private javax.swing.JSpinner readSpnr;
    private javax.swing.JPanel settingsTab;
    protected javax.swing.JLabel statusLbl;
    private javax.swing.JLabel statusName;
    private javax.swing.JPanel statusPane;
    private javax.swing.JScrollPane statusScrollPane;
    protected javax.swing.JTextArea statusTxt;
    private javax.swing.JTabbedPane tabbedPane;
    private javax.swing.JPanel twitterTab;
    private javax.swing.JButton userColorBtn;
    private javax.swing.JMenuItem wikiMenuItm;
    // End of variables declaration//GEN-END:variables
}
