
package jvst.examples.liquinth;

import jvst.wrapper.*;
import jvst.wrapper.valueobjects.*;

public class LiquinthVST extends VSTPluginAdapter {
	public static final String VERSION = "LiquinthVST a30";
	public static final String AUTHOR = "(c)2007 mumart@gmail.com";

	private static final int
		NUM_PROGRAMS = 16,
		MIX_BUF_FRAMES = 1024;

	private LiquinthVSTGUI liquinth_vst_gui;
	private Synthesizer synthesizer;
	private LiquinthProgram[] programs;
	private int num_controllers, current_program;
	private int[] mix_buf;

	public LiquinthVST( long wrapper ) {
		super( wrapper );
		programs = new LiquinthProgram[ NUM_PROGRAMS ];

		num_controllers = Synthesizer.get_num_controllers();
		for( int prg_idx = 0; prg_idx < NUM_PROGRAMS; prg_idx++ ) {
			LiquinthProgram program = new LiquinthProgram();
			program.name = "Blank " + prg_idx;
			program.controllers = new int[ num_controllers ];
			program.controllers[ 0 ] = 42; /* A bit of overdrive.*/
			program.controllers[ 1 ] = 127; /* Filter fully open.*/
			programs[ prg_idx ] = program;
		}

		mix_buf = Synthesizer.allocate_mix_buf( MIX_BUF_FRAMES );

		setNumInputs( 0 );
		setNumOutputs( 1 );
		canProcessReplacing( true );
		isSynth( true );
		setUniqueID( 20070708 );

		suspend();
	}

	public void set_gui( LiquinthVSTGUI gui ) {
		liquinth_vst_gui = gui;
	}

	public void set_controller( int ctrl_idx, int value, boolean update_gui ) {
		if( ctrl_idx < 0 || ctrl_idx >= num_controllers) return;
		LiquinthProgram program = programs[ current_program ];
		program.controllers[ ctrl_idx ] = value;
		synthesizer.set_controller( ctrl_idx, value );
		if( update_gui && liquinth_vst_gui != null )
			liquinth_vst_gui.set_controller( ctrl_idx, value );
	}

	/* Deprecated as of VST 2.4 */
	public void resume() {
		wantEvents( 1 );
	}

	public void setSampleRate( float sample_rate ) {
		synthesizer = new Synthesizer( ( int ) sample_rate );
		setProgram( current_program );
	}

	public void setProgram( int index ) {
		if( index < 0 || index >= NUM_PROGRAMS ) return;
		current_program = index;
		LiquinthProgram program = programs[ index ];
		for( int ctrl_idx = 0; ctrl_idx < num_controllers; ctrl_idx++ ) {
			int controller = program.controllers[ ctrl_idx ];
			set_controller( ctrl_idx, controller, true );
		}
	}

	public void setParameter( int index, float value ) {
		set_controller( index, ( int ) Math.round( value * 127 ), true );
	}

	public float getParameter( int index ) {
		if( index < 0 || index >= num_controllers ) return 0;
		LiquinthProgram program = programs[ current_program ];
		return program.controllers[ index ] / 127f;
	}

	public void setProgramName( String name ) {
		programs[ current_program ].name = name;
	}

	public String getProgramName() {
		return programs[ current_program ].name;
	}

	public String getParameterLabel( int index ) {
		return "";
	}

	public String getParameterDisplay( int index ) {
		return "";
	}

	public String getParameterName( int index ) {
		return Synthesizer.get_controller_name( index );
	}

	public VSTPinProperties getOutputProperties( int index ) {
		VSTPinProperties vpp = null;
		if( index == 0 ) {
			vpp = new VSTPinProperties();
			vpp.setLabel( "Liquinth" );
			vpp.setFlags( VSTPinProperties.VST_PIN_IS_ACTIVE );
		}
		return vpp;
	}

	public String getProgramNameIndexed( int category, int index ) {
		if( index < 0 || index >= NUM_PROGRAMS ) return "";
		return programs[ index ].name;
	}

	/* Deprecated as of VST 2.4 */
	public boolean copyProgram( int dest_idx ) {
		if( dest_idx < 0 || dest_idx >= NUM_PROGRAMS ) return false;
		LiquinthProgram src = programs[ current_program ];
		LiquinthProgram dest = programs[ dest_idx ];
		dest.name = src.name;
		for( int ctrl_idx = 0; ctrl_idx < num_controllers; ctrl_idx++ ) {
			dest.controllers[ ctrl_idx ] = src.controllers[ ctrl_idx ];
		}
		return true;
	}

	public String getEffectName() {
		return VERSION;
	}

	public String getVendorString() {
		return AUTHOR;
	}

	public String getProductString() {
		return VERSION;
	}

	public int getNumPrograms() {
		return NUM_PROGRAMS;
	}

	public int getNumParams() {
		return num_controllers;
	}

	public boolean setBypass( boolean value ) {
		return false;
	}

	public int getProgram() {	
		return current_program;
	}

	public int getPlugCategory() {
		return VSTPluginAdapter.PLUG_CATEG_SYNTH;
	}

	public int canDo( String feature ) {
		if( feature.equals( CANDO_PLUG_RECEIVE_VST_EVENTS ) )
			return CANDO_YES;
		if( feature.equals( CANDO_PLUG_RECEIVE_VST_MIDI_EVENT ) )
			return CANDO_YES;
		return CANDO_NO;
	}

	public boolean string2Parameter( int index, String value ) {
		try {
			float float_value = Float.parseFloat( value );
			setParameter( index, float_value );
		} catch( Exception e ) {
			return false;
		}
		return true;
	}

	/* Deprecated as of VST 2.4 */
	public void process( float[][] inputs, float[][] outputs, int frames ) {
		float[] output = outputs[ 0 ];
		int out_idx = 0;
		while( frames > 0 ) {
			int length = frames;
			if( length > MIX_BUF_FRAMES ) length = MIX_BUF_FRAMES;
			synthesizer.get_audio( mix_buf, length );
			for( int mix_idx = 0; mix_idx < length; mix_idx++ ) {
				float out = mix_buf[ mix_idx ];
				output[ out_idx++ ] += out * 0.00003f; 
			}
			frames -= length;
		}
	}

	public void processReplacing( float[][] inputs, float[][] outputs, int frames ) {
		float[] output = outputs[ 0 ];
		int out_idx = 0;
		while( frames > 0 ) {
			int length = frames;
			if( length > MIX_BUF_FRAMES ) length = MIX_BUF_FRAMES;
			synthesizer.get_audio( mix_buf, length );
			for( int mix_idx = 0; mix_idx < length; mix_idx++ ) {
				float out = mix_buf[ mix_idx ];
				output[ out_idx++ ] = out * 0.00003f; 
			}
			frames -= length;
		}
	}

	public int processEvents( VSTEvents vst_events ) {
		VSTEvent[] events = vst_events.getEvents();
		int num_events = vst_events.getNumEvents();
		for( int ev_idx = 0; ev_idx < num_events; ev_idx++ ) {
			VSTEvent event = events[ ev_idx ];
			if( event.getType() != VSTEvent.VST_EVENT_MIDI_TYPE ) continue;
			byte[] msg_data = ( ( VSTMidiEvent ) event ).getData();
			switch( msg_data[ 0 ] & 0xF0 ) {
				case 0x80: /* Note off.*/
					synthesizer.note_on( msg_data[ 1 ] & 0x7F, 0 );
					break;
				case 0x90: /* Note on.*/
					/* It seems note on with velocity = 0 is also note off.*/
					synthesizer.note_on( msg_data[ 1 ] & 0x7F, msg_data[ 2 ] & 0x7F );
					break;
				case 0xB0: /* Control change.*/
					/* Controller 120 = all sound off */
					/* Controller 121 = reset all controllers */
					/* Controller 123 = all notes off */
					int ctrl = msg_data[ 1 ] & 0x7F;
					int value = msg_data[ 2 ] & 0x7F;
					if( ctrl >= 20 && ctrl < num_controllers + 20 )
						set_controller( ctrl - 20, value, true );
					if( ctrl == 0x7E || ctrl == 0x7B )
						synthesizer.all_notes_off( false );
					break;
				case 0xE0: /* Pitch wheel.*/
					int pitch = ( msg_data[ 1 ] & 0x7F ) | ( ( msg_data[ 2 ] & 0x7F ) << 7 );
					synthesizer.set_pitch_wheel( pitch - 8192 );
					break;
			}
		}
		return 1;
	}
}

class LiquinthProgram {
	public String name;
	public int[] controllers;
}
