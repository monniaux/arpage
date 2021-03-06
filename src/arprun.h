/* -*- Mode: C; indent-tabs-mode: t; c-basic-offset: 4; tab-width: 4 -*- */
/*
 * arprun.h
 * Copyright (C) Mark Vitek 2009 <straypacket@gmail.com>
 *
 * arpage is free software: you can redistribute it and/or modify it
 * under the terms of the GNU General Public License as published by the
 * Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * arpage is distributed in the hope that it will be useful, but
 * WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.
 * See the GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License along
 * with this program.  If not, see <http://www.gnu.org/licenses/>.
 */
#ifndef __ARPRUN_H__
#define __ARPRUN_H__

#include <jack/jack.h>
#include "arppatt.h"
#include "ccrange.h"

#include <iostream>

//----------------------------------------------------------------------------
//
//
//
//
//----------------------------------------------------------------------------
class arp_runner {

private:

   range<arp_runner> ccmap[128];

public:

   arp_runner( int midi_channel, int program_change, double bpp );

   inline int bar() const { return this_bar; };
   inline int beat() const { return this_beat; };
   inline int tick() const { return this_tick; };

   inline int tpp() const { return ticks_per_pulse; }
   inline int pc() const { return pulse_countdown; }


   inline void            client( jack_client_t * c ) { jack_client = c; }
   inline jack_client_t * client() const { return jack_client; }

   inline void          in_port( jack_port_t * i ) { input_port=i; }
   inline jack_port_t * in_port() const { return input_port; }

   inline void          out_port( jack_port_t * o ) { output_port=o; }
   inline jack_port_t * out_port() const { return output_port; }

   inline void * out_port_buffer() const     { return out_port_buff; }
   void   out_port_buffer( void * b ) { out_port_buff=b; }

   inline void * in_port_buffer() const     { return in_port_buff; }
   inline void   in_port_buffer( void * b ) { in_port_buff=b; }

   void refresh_buffers( jack_nframes_t nframes );

   int  sync_callback(jack_transport_state_t, jack_position_t *);
   void process_callback( jack_nframes_t nframes, const jack_position_t * );

   bool change_detect( const jack_position_t * pos );
   int calc_next( const jack_position_t * pos );
   void reposition( const jack_position_t * pos );

   int output_next_pulse( jack_nframes_t nframes );
   int output_stop_pulse( jack_nframes_t nframes );
   int output_stop_note( jack_nframes_t nframes );

   void output_stop_pattern( jack_nframes_t nframes );
   void output_stop_sound( jack_nframes_t nframes );
   void output_pc( jack_nframes_t nframes );

   inline int midi_channel() const   { return user_channel; }
   inline void midi_channel( int c ) { user_channel=c; }

   int  channelized() const { return user_channelized; }
   void channelized( int a ) { user_channelized = a; }

   inline int    active() const  { return user_active; }
   inline void   active( int a ) { user_active=a; }

   inline int    program() const  { return user_program; }
   inline void   program( int p ) { this_program = user_program;
                                    user_program=p;  }

   void program_change(jack_nframes_t nframes);
   bool program_change_needed();

   inline int    velocity() const  { return user_velocity; }
   inline void   velocity( int v ) { user_velocity=v; }

   inline int    scale() const  { return user_scale; }
   inline void   scale( int s ) { user_scale=s; }

   inline int    latched() const  { return patt.latched(); }
   inline void   latched( int l ) { 	                                  
                                      if ( ! l ) {
                                         if ( patt.latched() ) {
                                            patt.latched(0);
                                            patt.release_latch();
										 }
                                      } else {
										  patt.latched(1);
									  }
	                                  
                                   }
   inline int    tapout_enabled() const { return latch_tapout; }
   inline void   tapout_enabled( int t ) { latch_tapout=t; }

   inline int    last_tapout() const  { return last_tappedout_note; }
   inline void   last_tapout( int n ) { last_tappedout_note=n; }

   inline int    sounding() const  { return note_is_on; }
   inline void   sounding( int n ) { note_is_on = n; }

   inline void   int_pulse_percent( int p ) {
                                    user_percent=(double)p/100;
                                    pulse_duration = ticks_per_pulse * user_percent;
                                    }

   inline double pulse_percent() const { return user_percent; }
   inline void   pulse_percent( double p ) {
                    user_percent=p;
                    pulse_duration = ticks_per_pulse * user_percent;
                    }

   inline double beats_per_pulse() const { return user_beats_per_pulse; }
   inline void   beats_per_pulse( double b ) {
      user_beats_per_pulse=b;
   }

   inline int duration() const { return pulse_duration; }

   inline int  can_add_to_latched() const { return !user_locked; }
   inline void can_add_to_latched( int l ) { user_locked = !l; }

   inline int    sound_off() const  { return sound_is_off; }
   inline void   sound_off( int s ) { sound_is_off=s; }

   inline int    rolling() const  { return is_rolling; }
   inline void   rolling( int r ) { is_rolling=r; }

   inline int    control_map( int i, int v ) {
      int retval = 0;
      if ( i >= 0 && i < 127 ) {
         retval =  ccmap[i].scaleinput(v);
      }
      return retval;
   }

   arp_pattern patt;

private:

   // JACK-related variables
   jack_port_t   *output_port;
   jack_port_t   *input_port;
   jack_client_t *jack_client;
   void          *out_port_buff;
   void          *in_port_buff;

   // these two determine the timing of on/off messages for elements of the current pattern.
   int    pulse_duration;   // ticks_per_pulse * user_percent
   int    ticks_per_pulse;  // user_beats_per_pulse * jack_position->ticks_per_pulse

   // these values may be modified by the UI, so lock the UI
   // when user_active && rolling
   double user_beats_per_pulse;      // what fraction of a beat does one pulse take?
   double user_percent;              // what percentage of the pulse period will the note be sounding?
   int    user_channel;              // to what MIDI channel does the arp respond?
   int    user_program;              // program change, sent to MIDI out
   int    user_velocity;             // 0 == played velocity, 1 - 127 == fixed velocity
   int    user_scale;                // 13 == none, 0-12 major, minor, dorean, etc...
   int    user_active;               // will this arp respond to MIDI input?
   int    user_channelized;          // will this arp respond only to MIDI messages received on user_channel?
   int    user_locked;               // when latched, will the latched pattern reject new notes?

   // these are the counters to determine when to emit note-on, note-off
   int    pulse_countdown;
   int    last_pos;

   // these are used to detect changed values
   int    sound_is_off;
   int    this_program;
   int    initial_program;
   int    this_bar;
   int    this_beat;
   int    this_tick;
   int    note_is_on;
   int    fresh_run;
   int    latch_tapout;
   int    last_tappedout_note;
   int    is_rolling;
   
   int notes_sounding[PATTMAX];
   
   // also used to detect changes, but these may be modified when user values are changed
   double this_percent;
   double this_beats_per_pulse;

   // declared private, not defined - no copying objects of this class.
   arp_runner( const arp_runner & );
   arp_runner operator = ( const arp_runner & );
};

#endif
