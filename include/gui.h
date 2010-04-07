// generated by Fast Light User Interface Designer (fluid) version 1.0104

#ifndef gui_h
#define gui_h
#include <FL/Fl.H>
#include <FL/Fl_Double_Window.H>
#include <FL/fl_draw.H>
#include "gl_window.h"
#include <FL/Fl_Group.H>
#include <FL/Fl_Slider.H>
#include <FL/Fl_Button.H>
#include <FL/Fl_Value_Slider.H>

class gui {
public:
  gui();
  Fl_Double_Window *form;
  CGL_Window *gl_window;
  Fl_Slider *x;
private:
  inline void cb_x_i(Fl_Slider*, void*);
  static void cb_x(Fl_Slider*, void*);
public:
  Fl_Slider *y;
private:
  inline void cb_y_i(Fl_Slider*, void*);
  static void cb_y(Fl_Slider*, void*);
public:
  Fl_Slider *z;
private:
  inline void cb_z_i(Fl_Slider*, void*);
  static void cb_z(Fl_Slider*, void*);
public:
  Fl_Button *quit;
private:
  inline void cb_quit_i(Fl_Button*, void*);
  static void cb_quit(Fl_Button*, void*);
public:
  Fl_Value_Slider *h;
private:
  inline void cb_h_i(Fl_Value_Slider*, void*);
  static void cb_h(Fl_Value_Slider*, void*);
public:
  Fl_Button *animate;
private:
  inline void cb_animate_i(Fl_Button*, void*);
  static void cb_animate(Fl_Button*, void*);
public:
  Fl_Slider *sheet;
private:
  inline void cb_sheet_i(Fl_Slider*, void*);
  static void cb_sheet(Fl_Slider*, void*);
public:
  Fl_Slider *rudder;
private:
  inline void cb_rudder_i(Fl_Slider*, void*);
  static void cb_rudder(Fl_Slider*, void*);
public:
  Fl_Button *reset_ruder;
private:
  inline void cb_reset_ruder_i(Fl_Button*, void*);
  static void cb_reset_ruder(Fl_Button*, void*);
public:
  Fl_Slider *wind_mag;
private:
  inline void cb_wind_mag_i(Fl_Slider*, void*);
  static void cb_wind_mag(Fl_Slider*, void*);
public:
  Fl_Slider *height;
private:
  inline void cb_height_i(Fl_Slider*, void*);
  static void cb_height(Fl_Slider*, void*);
public:
  Fl_Slider *length;
private:
  inline void cb_length_i(Fl_Slider*, void*);
  static void cb_length(Fl_Slider*, void*);
public:
  Fl_Button *restart_b;
private:
  inline void cb_restart_b_i(Fl_Button*, void*);
  static void cb_restart_b(Fl_Button*, void*);
public:
  Fl_Button *weather_b;
private:
  inline void cb_weather_b_i(Fl_Button*, void*);
  static void cb_weather_b(Fl_Button*, void*);
public:
  Fl_Button *down_wind;
private:
  inline void cb_down_wind_i(Fl_Button*, void*);
  static void cb_down_wind(Fl_Button*, void*);
public:
  Fl_Button *tack;
private:
  inline void cb_tack_i(Fl_Button*, void*);
  static void cb_tack(Fl_Button*, void*);
public:
  Fl_Button *reach;
private:
  inline void cb_reach_i(Fl_Button*, void*);
  static void cb_reach(Fl_Button*, void*);
public:
  void show();
private:
  char filename[1024];
};
#endif