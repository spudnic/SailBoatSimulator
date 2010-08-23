//////////////////////////////////////////////////////////////////////
// gl_window.h: interface for the CGL_Window class.
// derived from Fl_Gl_Window class in fltk
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_GL_WINDOW_H__5A69663D_E174_43C2_8F0B_1068B54B0729__INCLUDED_)
#define AFX_GL_WINDOW_H__5A69663D_E174_43C2_8F0B_1068B54B0729__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include <FL/Fl_Gl_Window.H>
#include <FL/Fl.H>
#include <FL/gl.h>
#include <GL/glu.h>
#include <iostream>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
//#include "sail_boat_longkeel.h"

#define MAX_PTS 40
#define MAXFLOAT ((float)3.40282346638528860e+38) /* taken from math.h */
#define MINFLOAT (-MAXFLOAT)

/* Particle system data structure */
/***********************/
//simple xyz used for position and velocity
typedef struct
{
    float x, y, z;             /* coordinate values */
} coord;

//infomation about individual particles
typedef struct
{
    coord pos;
    coord vel;
    double color[3];
    float life;
} particle;

//information about the total particle system  water
typedef struct 
{
    int     num_particles;  /* How many particles there are */
    particle *particles;
    coord   center_pt;      /* Center point of particel system*/
} particle_system;

/***********************/


// milk shape import structure
/***********************/
typedef struct _point2 {
	double x,y;
} Point2;

typedef struct _point3 {
	double x,y,z;
} Point3;

typedef struct _obj {
	Point3	*vertices;
	long	*v_idx;
	Point3	*normals;
	long	*n_idx;
	Point2	*uvs;
	int		num_faces;
} Obj;
/**********************/

class CGL_Window : public Fl_Gl_Window {
private:
	

	int handle(int);	//handle events 
				//(overload of Fl_Gl_Window function)
	void draw();		//overload of the drawing function
protected:
	void drawPicture(void); //draw your picture here
public: 

	

	//counter
	int count;
	double offset;
	//camera values
	double rx,ry,rz,zm;		
	
	//animation values
	int animate_set;

	//rk4 values
	double rk4_h;

//set by slider
	//wind varibles
	double wind_direction,wind_magnitude; 
	//wave varibles
	double wave_direction,wave_length,wave_height,wave_varince; 
	//boat varibles
	double rudder,sheet; 
//set by math
	// boat globals //trim on the main sheet
	double heel;  //heel of the boat dew to wind
	double wave_tilt;  //bounce of the boat
	double b_pos[3];   //position of boat
	double b_dir;	// direction of the foward movemtn of boat
	double b_vel[2]; //velocity in x and y direction
//water
	int move;

	//particle system
	double gra;
	particle_system  ps;
	int b_flag,rs_flag;
	double life_of_particle;	
	double velocity;
	int	num_parts;
	double intial_color1, intial_color2, intial_color3,
		   end_color1,end_color2,end_color3;
	double random_ness;
	//camera functions


	//drawing funtions
	void draw_obj(Obj mesh); //draws boat
	void draw_water();

	//slider adjustment functions
	void camera(double value, char type);
	void adjust(double value, char type);

	//points of sail
	void weather();
	void reach();
	void down_wind();
	void tack();
	void jibe();
	//animation adjustment functions
	void b_position(); //calculating boat info

	void load(char *file);	//load an input file
	void quit_cb(void);     	
	
	
	CGL_Window(int,int,int,int,const char *l = 0);	//constructor
	virtual ~CGL_Window();				//destructor
	
	//double bottom_accel(double xpos,double ypos,double xpos1,double ypos1);
	//double top_accel(double xpos,double ypos,double xpos1,double ypos1);
	void material_set(double r,double g,double b,double t);
	void light();
	
	//void position1(double xpos,double ypos,double xpos1,double ypos1);
	//void animate_go();

	//particle system functiona
	void reset_particle(particle* part);
	void position_particle(particle* part);
	void position(double xpos,double ypos);
	void draw_particle();
	void restart();
};

#endif 
