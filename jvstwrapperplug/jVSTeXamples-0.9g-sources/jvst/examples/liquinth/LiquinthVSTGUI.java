
package jvst.examples.liquinth;

import java.awt.*;
import javax.swing.*;
import javax.swing.border.*;
import javax.swing.event.*;

import jvst.wrapper.*;
import jvst.wrapper.gui.VSTPluginGUIRunner;

public class LiquinthVSTGUI extends VSTPluginGUIAdapter {
	private LiquinthVST liquinth_vst;
	private JSlider[] controllers;

	public LiquinthVSTGUI(VSTPluginGUIRunner r, VSTPluginAdapter plug) {
		super(r,plug);
		setTitle( LiquinthVST.VERSION );
		liquinth_vst = ( LiquinthVST ) plug;
		liquinth_vst.set_gui( this );
		
		JPanel main_panel = new JPanel( new GridBagLayout() );
		main_panel.setBorder( new EmptyBorder( 6, 6, 6, 6 ) );

		GridBagConstraints gbc = new GridBagConstraints();
		gbc.fill = GridBagConstraints.HORIZONTAL;
		gbc.insets = new Insets( 2, 2, 2, 2 );

		JPanel logo_panel = new JPanel( new BorderLayout() );
		logo_panel.setBackground( Color.BLACK );
		logo_panel.setBorder( new BevelBorder( BevelBorder.LOWERED, Color.WHITE, Color.GRAY ) );
		ImageIcon logo = new ImageIcon( getClass().getResource( "liquinth.png" ) );
		logo_panel.add( new JLabel( logo ), BorderLayout.CENTER );
		gbc.gridwidth = GridBagConstraints.REMAINDER;
		main_panel.add( logo_panel, gbc );

		int num_controllers = Synthesizer.get_num_controllers();
		controllers = new JSlider[ num_controllers ];
		for( int idx = 0; idx < num_controllers; idx++ ) {
			gbc.weightx = 0;
			gbc.gridwidth = 1;
			String control_name = Synthesizer.get_controller_name( idx );
			main_panel.add( new JLabel( control_name ), gbc );
			gbc.weightx = 1;
			gbc.gridwidth = GridBagConstraints.REMAINDER;
			controllers[ idx ] = new JSlider( JSlider.HORIZONTAL, 0, 127, 0 );
			controllers[ idx ].addChangeListener( new SliderListener( idx ) );
			main_panel.add( controllers[ idx ], gbc );
		}

		getContentPane().setLayout( new BorderLayout() );
		getContentPane().add( main_panel, BorderLayout.CENTER );
		
		//this is needed on the mac only, 
	    //java guis are handled there in a pretty different way than on win/linux
	    //XXX
	    if (RUNNING_MAC_X) show();
	}


	public void set_controller( int controller, int value ) {
		if( controller >= 0 && controller < controllers.length ) {
			controllers[ controller ].setValue( value );
		}
	}

	private class SliderListener implements ChangeListener {
		private int controller;

		public SliderListener( int control_idx ) {
			controller = control_idx;
		}

		public void stateChanged( ChangeEvent e ) {
			int value;
			value = controllers[ controller ].getValue();
			liquinth_vst.set_controller( controller, value, false );
		}
	}
}
