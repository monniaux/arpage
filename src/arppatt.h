/* -*- Mode: C; indent-tabs-mode: t; c-basic-offset: 4; tab-width: 4 -*- */
/*
 * arppatt.h
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
#ifndef __ARPPATT_H__
#define __ARPPATT_H__

#include "pattelem.h"
#include "scales.h"

#define PATTMAX  256

//----------------------------------------------------------------------------
//
//----------------------------------------------------------------------------
class arp_pattern
{
public:
   //----------------------------------------------------------------------------
   // Play modes (determine order of notes output  by arpeggiator)
   //----------------------------------------------------------------------------
   
   enum {
	    MODE_IN      = 0,
        MODE_UP      = 1,
	    MODE_DN      = 2,
	    MODE_UPDN    = 3,
	    MODE_DNUP    = 4,
	    MODE_UPDNEXC = 5,
	    MODE_DNUPEXC = 6,
	    MODE_RAND    = 7,
	    MODE_RANDDYN = 8,
   };
   
   arp_pattern() :
      pattern_mode(MODE_IN),
      pattern_size(0),
      pattern_pos(0),
      pattern_latched(0),
      unlatch_size(0),
      patt_interval(0),
      patt_range(0),
      patt_scale(13),
      patt_transpose(0),
      release_size(0),
	  pattern_sorter(NULL)
   {
      initpattern();
   }

   void initpattern(void);

   int pattern_advance(void);

   inline int note_at( int npos ) const {
      return pattern_input[npos].note_number();
   }

   inline int velocity_at(int npos) const {
      return pattern_input[npos].msg.velocity();
   }

   inline int input_note_at( int npos ) const {
      return pattern_input[npos].input_note_number();
   }

   inline pattern_element & element_at( int npos ) {
      return pattern_input[npos];
   }

   int findnote( int note ) const;
   int findinputnote( int note ) const;

   void addnote( const MidiMessage & msg );
   void delnote( const MidiMessage & msg, bool ignore_latch = false );

   void clear_pattern(void);
   void release_latch(void);

   inline int    pos() const        { return pattern_pos; }
   inline void   pos( int p )       { pattern_pos = p; }

   inline int    size() const       { return pattern_size; }

   inline void   latched( int l )   { pattern_latched = (l != 0); }
   inline int    latched() const    { return pattern_latched != 0; }
   inline int    mode() const       { return pattern_mode; }
   inline void   mode( int m )      { pattern_mode = m; }
   inline int    range() const      { return patt_range; }
   inline void   range( int r )     { patt_range=r; }
   inline int    scale() const      { return patt_scale; }
   inline void   scale( int s )     { patt_scale=s; }
   inline int    interval() const   { return patt_interval; }
   inline void   interval( int i )  { patt_interval=i; }
   inline int    transpose() const  { return patt_transpose; }
   inline void   transpose( int i ) { patt_transpose=i; }

   // pull the next pending note-off event
   int release_advance(void);

   // put an item into the release_output[] array
   void addreleasenote( const pattern_element & elem, int elem_ttl );

   // get the 1st element of the release_output[] array
   pattern_element & releasehead() { return release_output[0]; }

   // how many items in the release_output[] array
   int relsize(void) const { return release_size; }

   // decrement the time-to-live for each element in the release_output[] array
   int release_countdown( int c );

private:
   int    pattern_mode;
   int    pattern_pos;
   int    pattern_latched;
   int    patt_interval;
   int    patt_transpose;
   int    patt_range;
   int    patt_scale;

   int    pattern_size;
   int    unlatch_size;
   
   int  (*pattern_sorter)(const void*,const void*);
   scales patt_scales;
   
   pattern_element          pattern_input[PATTMAX];
   pattern_element pattern_pending_delete[PATTMAX];
   
   int release_size;
   pattern_element release_output[PATTMAX];
   int ttl[PATTMAX];
   
   arp_pattern( const arp_pattern & );
   arp_pattern & operator = ( const arp_pattern & );
};

//===========================================================================-
//
//===========================================================================-
#endif
