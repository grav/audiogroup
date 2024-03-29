
package jvst.examples.liquinth;

public class Synthesizer {
	private static final int
		LOG2_NUM_VOICES = 3,
		NUM_VOICES = 1 << LOG2_NUM_VOICES,
		NUM_CONTROLLERS = 6,
		OVERSAMPLE = 4,
		FIR_COEFFS = 15;

	private static final String[] control_names = new String[] {
		"Overdrive",
		"Filter Cutoff",
		"Filter Resonance",
		"Filter Attack Level",
		"Filter Decay",
		"Portamento Speed"
	};

	private static final short[] fir_coeffs = new short[] {
		  -4,  -34,  -66,  175, 1289,
		3584, 6193, 7373, 6193, 3584,
		1289,  175,  -66,  -34,   -4
	};

	private int[] fir_buf;
	private MoogFilter filter;
	private Envelope filter_env;
	private Voice[] voices;
	private byte[] key_status, controllers;

	public Synthesizer( int sampling_rate ) {
		int idx;
		fir_buf = new int[ FIR_COEFFS ];
		filter = new MoogFilter();
		filter_env = new Envelope( sampling_rate );
		voices = new Voice[ NUM_VOICES ];
		for( idx = 0; idx < NUM_VOICES; idx++ ) {
			voices[ idx ] = new Voice( sampling_rate * OVERSAMPLE );
			voices[ idx ].key_on( idx );
		}
		key_status = new byte[ 128 ];
		controllers = new byte[ NUM_CONTROLLERS + Voice.NUM_CONTROLLERS ];
		set_controller( 0, 42 );
		set_controller( 1, 127 );
		all_notes_off( true );
	}

	public static int get_num_controllers() {
		return NUM_CONTROLLERS + Voice.NUM_CONTROLLERS;
	}

	public static String get_controller_name( int control ) {
		int num_controls;
		String name;
		if( control < NUM_CONTROLLERS ) {
			name = control_names[ control ];
		} else {
			name = Voice.get_controller_name( control - NUM_CONTROLLERS );
		}
		return name;
	}

	public static int[] allocate_mix_buf( int frames ) {
		return new int[ frames * OVERSAMPLE + FIR_COEFFS ];
	}

	public synchronized void get_audio( int[] out_buf, int length ) {
		int idx, input_len, input_ep1;
		int cutoff, alevel;
		/* Copy samples from previous invocation for downsampling filter.*/
		for( idx = 0; idx < FIR_COEFFS; idx++ ) {
			out_buf[ idx ] = fir_buf[ idx ];
		}
		input_len = length * OVERSAMPLE;
		/* Clear mix buffer.*/
		input_ep1 = FIR_COEFFS + input_len;
		for( idx = FIR_COEFFS; idx < input_ep1; idx++ ) {
			out_buf[ idx ] = 0;
		}
		/* Get audio from voices. */
		for( idx = 0; idx < NUM_VOICES; idx++ ) {
			voices[ idx ].get_audio( out_buf, FIR_COEFFS, input_len );
		}
		/* Store end samples for next invocation.*/
		for( idx = 0; idx < FIR_COEFFS; idx++ ) {
			fir_buf[ idx ] = out_buf[ input_len + idx ];
		}
		downsample( out_buf, length );
		/* Handle filter envelope.*/
		cutoff = ( controllers[ 1 ] + 1 ) << Maths.FP_SHIFT - 7;
		alevel = controllers[ 3 ] << Maths.FP_SHIFT - 7;
		cutoff += filter_env.get_amplitude() * alevel >> Maths.FP_SHIFT;
		if( cutoff > Maths.FP_ONE ) {
			cutoff = Maths.FP_ONE;
		}
		cutoff = Maths.exp_scale( cutoff, 8 );
		filter.set_cutoff( cutoff / ( float ) Maths.FP_ONE );
		filter.filter( out_buf, length );
		filter_env.update( length );
	}

	public synchronized void note_on( int key, int velocity ) {
		int idx;
		int porta_voice, assigned_voice, quietest_voice;
		int highest_key, voice_key, voice_vol, min_vol;
		boolean key_is_on;

		if( key < 0 || key > 127 ) {
			return;
		}

		key_status[ key ] = 0;
		if( velocity > 0 ) {
			key_status[ key ] = 1;
		}
		/* Determine highest depressed key. */
		highest_key = 128;
		for( idx = 0; idx < 128; idx++ ) {
			if( key_status[ idx ] > 0 ) {
				highest_key = idx;
			}
		}

		min_vol = -1;
		porta_voice = -1;
		assigned_voice = -1;
		quietest_voice = -1;
		for( idx = 0; idx < NUM_VOICES; idx++ ) {
			key_is_on = voices[ idx ].key_is_on();
			voice_key = voices[ idx ].get_key();
			if( key == voice_key ) {
				/* Voice has this key already assigned to it. */
				if( key_is_on || assigned_voice < 0 ) {
					/* Voices may have the same key. Prefer */
					/* the keyed-on voice over a keyed off one. */
					assigned_voice = idx;
				}
			}
			if( key_is_on ) {
				if( controllers[ 5 ] > 0 ) {
					/* Portamento mode. */
					if( porta_voice > -1 ) {
						/* Only one voice should be active.*/
						voices[ porta_voice ].key_off( false );
					}
					porta_voice = idx;
				}
			} else {
				/* Test if this is the quietest. */
				voice_vol = voices[ idx ].get_volume();
				if( quietest_voice < 0 || voice_vol < min_vol ) {
					quietest_voice = idx;
					min_vol = voice_vol;
				}
			}
		}

		if( velocity > 0 ) {
			/* Key on */
			if( porta_voice > -1 ) {
				if( key == highest_key ) {
					/* Key pressed is higher than before.*/
					filter_env.key_on();
					filter_env.key_off( false );
					/* New key is the highest. */
					voices[ porta_voice ].key_on( key );
				}
			} else {
				filter_env.key_on();
				filter_env.key_off( false );
				if( assigned_voice > -1 ) {
					/* Re-key previously assigned voice. */
					voices[ assigned_voice ].key_on( key );
				} else if( quietest_voice > -1 ) {
					/* Allocate new voice.*/
					voices[ quietest_voice ].key_on( key );
				}
			}
		} else {
			/* Key off */
			if( porta_voice > -1 ) {
				if( highest_key > 127 ) {
					/* Porta voice released.*/
					voices[ porta_voice ].key_off( false );
				} else if( key > highest_key ) {
					/* Highest key released, keys still down. */
					voices[ porta_voice ].key_on( highest_key );
				}
			} else {
				if( assigned_voice > -1 ) {
					/* Key off assigned voice. */
					voices[ assigned_voice ].key_off( false );
				}
			}
		}
	}

	public synchronized int get_controller( int controller ) {
		int value;
		value = 0;
		if( controller >= 0 && controller < NUM_CONTROLLERS ) {
			return controllers[ controller ];
		}
		return value;
	}

	public synchronized void set_controller( int controller, int value ) {
		int idx;
		if( controller < 0 || controller >= controllers.length ) {
			return;
		}
		if( value < 0 || value > 127 ) {
			return;
		}
		controllers[ controller ] = ( byte ) value;
		switch( controller ) {
			case 0:
				value = value << Maths.FP_SHIFT - 7;
				value = Maths.exp_scale( value, LOG2_NUM_VOICES );
				for( idx = 0; idx < NUM_VOICES; idx++ ) {
					voices[ idx ].set_volume( value );
				}
				break;
			case 1:
				/* Filter cutoff, handled in envelope calculations.*/
				break;
			case 2:
				filter.set_resonance( value * 0.0314f );
				break;
			case 3:
				/* Filter envelope level.*/
				break;
			case 4:
				filter_env.set_release_time( value << 4 );
				break;
			case 5:
				value = value << Maths.FP_SHIFT - 7;
				value = Maths.exp_scale( value, 7 );
				for( idx = 0; idx < NUM_VOICES; idx++ ) {
					voices[ idx ].set_portamento_time( value * 1000 / Maths.FP_ONE );
				}
				break;
			default:
				for( idx = 0; idx < NUM_VOICES; idx++ ) {
					voices[ idx ].set_controller( controller - NUM_CONTROLLERS, value );
				}
				break;
		}
	}

	public synchronized void set_pitch_wheel( int value ) {
		int idx;
		for( idx = 0; idx < NUM_VOICES; idx++ ) {
			voices[ idx ].set_pitch_wheel( value );
		}
	}

	public synchronized void all_notes_off( boolean sound_off ) {
		int idx;
		for( idx = 0; idx < NUM_VOICES; idx++ ) {
			voices[ idx ].key_off( sound_off );
		}
		for( idx = 0; idx < 128; idx++ ) {
			key_status[ idx ] = 0;
		}
	}

	private void downsample( int[] buf, int num_output ) {
		int idx, in_idx, out_idx, out;
		in_idx = out_idx = 0;
		while( out_idx < num_output ) {
			out = 0;
			for( idx = 0; idx < FIR_COEFFS; idx++ ) {
				out += buf[ in_idx + idx ] * fir_coeffs[ idx ] >> Maths.FP_SHIFT;
			}
			buf[ out_idx++ ] = out;
			in_idx += OVERSAMPLE;
		}
	}
}

