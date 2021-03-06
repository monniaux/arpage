/* -*- Mode: C; indent-tabs-mode: t; c-basic-offset: 4; tab-width: 4 -*- */
/*
 * arpui.h
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
#ifndef __ARPUI_H__
#define __ARPUI_H__

#include "arprun.h"

#include <gtkmm.h>
#include <string>
#include <iostream>

#define BLUELED_PNG      PACKAGE_DATA_DIR"/arpage/ui/ledblue_on.png"
#define BLUELED_OFF_PNG  PACKAGE_DATA_DIR"/arpage/ui/ledblue_off.png"
#define REDLED_PNG       PACKAGE_DATA_DIR"/arpage/ui/ledred_on.png"
#define REDLED_OFF_PNG   PACKAGE_DATA_DIR"/arpage/ui/ledred_off.png"
#define GREENLED_PNG     PACKAGE_DATA_DIR"/arpage/ui/ledgreen_on.png"
#define GREENLED_OFF_PNG PACKAGE_DATA_DIR"/arpage/ui/ledgreen_off.png"
#define CYANLED_PNG      PACKAGE_DATA_DIR"/arpage/ui/ledgreen_off.png"

struct arpui {

   enum {
      NOTEDUR_4   = 0,
      NOTEDUR_4T  = 1,
      NOTEDUR_8   = 2,
      NOTEDUR_8T  = 3,
      NOTEDUR_16  = 4,
      NOTEDUR_16T = 5,
      NOTEDUR_32  = 6,
      NOTEDUR_32T = 7
   };
 
   int note_active;
   int latch_active;
   int this_bar;
   int this_beat;
   int this_tick;
   int this_pulse_duration;
   int this_pc;
   int is_rolling;

   arp_runner       * runner;
   Gtk::Main        * main;
   Gtk::SpinButton  * arpchannel;
   Gtk::SpinButton  * arprange;
   Gtk::SpinButton  * arptranspose;
   Gtk::SpinButton  * arpinterval;
   Gtk::SpinButton  * arpprogram;
   Gtk::SpinButton  * arpvelocity;
   Gtk::CheckButton * arpactive;
   Gtk::CheckButton * arpsetlatch;
   Gtk::CheckButton * arplatchlock;
   Gtk::CheckButton * arpchannelized;
   Gtk::CheckButton * arptapout;

   Gtk::ComboBox    * arpscale;
   Gtk::ComboBox    * arpnoteorder;
   Gtk::ComboBox    * arpnoteduration;

   Gtk::Image       * image1;
   Gtk::Image       * image2;
   Gtk::Image       * image3;

   Glib::RefPtr<Gdk::Pixbuf>  pix_redled;
   Glib::RefPtr<Gdk::Pixbuf>  pix_redled_off;
   Glib::RefPtr<Gdk::Pixbuf>  pix_blueled;
   Glib::RefPtr<Gdk::Pixbuf>  pix_blueled_off;
   Glib::RefPtr<Gdk::Pixbuf>  pix_cyanled;
   Glib::RefPtr<Gdk::Pixbuf>  pix_greenled;
   Glib::RefPtr<Gdk::Pixbuf>  pix_greenled_off;

   Gtk::VScale      * arpnotepercent;

   arpui( const std::string & prefix, Glib::RefPtr<Gtk::Builder> & builder, 
          arp_runner * pr, Gtk::Main* pm );

   std::string arpprefix;

   void on_channel_change();
   void on_range_change();
   void on_interval_change();
   void on_transpose_change();
   void on_active_change();
   void on_setlatch_change();
   void on_tapout_change();

   bool pos_actions();
   bool note_sounding_actions();

   void on_channelized_change();
   void on_scale_change();
   void on_latchlock_change();
   void on_program_change();
   void on_velocity_change();
   void on_noteorder_change();
   void on_noteduration_change();
   void on_notepercent_change();

   bool store_session();
};
#endif
