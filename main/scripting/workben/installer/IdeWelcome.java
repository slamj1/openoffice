/**************************************************************
 * 
 * Licensed to the Apache Software Foundation (ASF) under one
 * or more contributor license agreements.  See the NOTICE file
 * distributed with this work for additional information
 * regarding copyright ownership.  The ASF licenses this file
 * to you under the Apache License, Version 2.0 (the
 * "License"); you may not use this file except in compliance
 * with the License.  You may obtain a copy of the License at
 * 
 *   http://www.apache.org/licenses/LICENSE-2.0
 * 
 * Unless required by applicable law or agreed to in writing,
 * software distributed under the License is distributed on an
 * "AS IS" BASIS, WITHOUT WARRANTIES OR CONDITIONS OF ANY
 * KIND, either express or implied.  See the License for the
 * specific language governing permissions and limitations
 * under the License.
 * 
 *************************************************************/

package installer;

/*
 * Welcome.java
 *
 * Created on 04 July 2002, 15:43
 */

/**
 *
 * @author  mike
 */
import java.awt.event.*;
import javax.swing.*;
import java.io.*;
import java.net.*;
import java.awt.*;

public class IdeWelcome extends javax.swing.JPanel implements ActionListener {

    /** Creates new form Welcome */
    public IdeWelcome(InstallWizard wizard) {
		this.wizard = wizard;
		setBorder(new javax.swing.border.EtchedBorder(javax.swing.border.EtchedBorder.RAISED));
        initComponents();
    }

    /** This method is called from within the constructor to
     * initialize the form.
     * WARNING: Do NOT modify this code. The content of this method is
     * always regenerated by the Form Editor.
     */
    private void initComponents() {//GEN-BEGIN:initComponents
        welcomePanel = new javax.swing.JPanel();
        area = new javax.swing.JTextArea();
        
        setLayout(new java.awt.BorderLayout());
        
        welcomePanel.setLayout(new java.awt.BorderLayout());
        //area.setHorizontalAlignment(javax.swing.JTextField.CENTER);
		area.setEditable(false);
		area.setLineWrap(true);
		area.setText("\n Click Next to include Scripting Framework support for IDEs.");
		area.append("\n Click Cancel exit the Installation process. \n");
	if( InstUtil.hasNetbeansInstallation() ) {
		area.append("\n \tA version of Netbeans has been detected. \n");
	}
	//if( InstUtil.hasJeditInstallation() ) {
	//	area.append("\n \tA version of jEdit has been detected.");
	//}	
		
        welcomePanel.add(area, java.awt.BorderLayout.CENTER);
        add(welcomePanel, java.awt.BorderLayout.CENTER);
		NavPanel nav = new NavPanel(wizard, false, true, true, "", InstallWizard.IDEVERSIONS);
		nav.setNextListener(this);
		add(nav, java.awt.BorderLayout.SOUTH);
		
		//Banner br = new Banner();
		//add(br, java.awt.BorderLayout.WEST);
        
    }//GEN-END:initComponents

    public java.awt.Dimension getPreferredSize() {
        return new java.awt.Dimension(InstallWizard.DEFWIDTH, InstallWizard.DEFHEIGHT);
    }    

	public void actionPerformed(ActionEvent ev)
	{
		//Perform next actions here...
	}


    // Variables declaration - do not modify//GEN-BEGIN:variables
    private javax.swing.JPanel welcomePanel;
    private javax.swing.JTextArea area;
	private InstallWizard wizard;
	
    // End of variables declaration//GEN-END:variables
}
