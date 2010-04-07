// GL_Window.cxx: implementation of the CGL_Window class.
//
//////////////////////////////////////////////////////////////////////

#include "gl_window.h"
#include "gui.h"
#include "math.h"
#include <GL/glut.h>
#include "sail_boat_longkeel.h"
#include <stdio.h>
#include <stdlib.h>
#include <ctime>
//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////
Obj hull;
Obj main_sail;
Obj cabin;
Obj mast_boom;
Obj jib_sail;
Obj rud;

CGL_Window::CGL_Window(int x, int y, int w, int h, const char *l)
: Fl_Gl_Window(x,y,w,h,l)
{
	srand(clock());
//water
	move=5;
//animation
	count=1;
	rk4_h=.1;
//set by slider
	//camera
	rx=40;ry=-15;rz=40;
	zm=0;
	//wind varibles
	wind_direction=0;
	wind_magnitude=5; 
	//wave varibles
	wave_direction=0;
	wave_length=.5;
	wave_height=.5; 
	wave_varince=1;
	offset=5;
	//boat varibles
	rudder=0;
	sheet=0; 
	b_dir=0;
//set by math
	// boat globals //trim on the main sheet
	heel=0;  //heel of the boat dew to wind
	wave_tilt=0;  //bounce of the boat in surf
	//boat position
	b_pos[0]=50;
	b_pos[1]=0;
	b_pos[2]=50;
	//boat velocity
	b_vel[0]=0;
	b_vel[1]=0;

//loaded in	

	// rudder
	rud.vertices = Cylinder01_vertex;
	rud.v_idx = Cylinder01_vidx;
	rud.normals = Cylinder01_normal;
	rud.n_idx = Cylinder01_nidx;
	rud.uvs = Cylinder01_uv;
	rud.num_faces = (sizeof(Cylinder01_vidx)/sizeof(long));

	// Hull
	hull.vertices = mesh1_vertex;
	hull.v_idx = mesh1_vidx;
	hull.normals = mesh1_normal;
	hull.n_idx = mesh1_nidx;
	hull.uvs = mesh1_uv;
	hull.num_faces = (sizeof(mesh1_vidx)/sizeof(long));
	// cabins
	cabin.vertices = mesh2_vertex;
	cabin.v_idx = mesh2_vidx;
	cabin.normals = mesh2_normal;
	cabin.n_idx = mesh2_nidx;
	cabin.uvs = mesh2_uv;
	cabin.num_faces = (sizeof(mesh2_vidx)/sizeof(long));

	
	mast_boom.vertices = mesh3_vertex;
	mast_boom.v_idx = mesh3_vidx;
	mast_boom.normals = mesh3_normal;
	mast_boom.n_idx = mesh3_nidx;
	mast_boom.uvs = mesh3_uv;
	mast_boom.num_faces = (sizeof(mesh3_vidx)/sizeof(long));

	
	main_sail.vertices = mesh5_vertex;
	main_sail.v_idx = mesh5_vidx;
	main_sail.normals = mesh5_normal;
	main_sail.n_idx = mesh5_nidx;
	main_sail.uvs = mesh5_uv;
	main_sail.num_faces = (sizeof(mesh5_vidx)/sizeof(long));

	
	jib_sail.vertices = mesh4_vertex;
	jib_sail.v_idx = mesh4_vidx;
	jib_sail.normals = mesh4_normal;
	jib_sail.n_idx = mesh4_nidx;
	jib_sail.uvs = mesh4_uv;
	jib_sail.num_faces = (sizeof(mesh4_vidx)/sizeof(long));

//particles
	num_parts=0;
	gra=9.8;
	b_flag=1;
	rs_flag=1;
	random_ness=1.0;
	life_of_particle=10;
	velocity=.5;
	intial_color1=intial_color2=0;
	intial_color3=1;
	end_color1=end_color2=end_color3=0;
	ps.num_particles=400;
	ps.center_pt.x =b_pos[0];
	ps.center_pt.y =-10;
	ps.center_pt.z =b_pos[2];  
	ps.particles=new particle[ps.num_particles];
	//exit(0);
	//default setup for particles
	for(int i=0;i<ps.num_particles;i++){
		reset_particle(&ps.particles[i]);	
	}
}

CGL_Window::~CGL_Window()
{

}

/******************************************/
/*   Do your Gl work here		  */
/******************************************/
void CGL_Window::drawPicture(void)
{
		count+=1;
	glLineWidth(.5);
	
//draw water
	glPushMatrix();
	draw_water();
	glPopMatrix();	
//draw wind glyph
	glPushMatrix();
	glTranslated(b_pos[0],-10,b_pos[2]);  
	glRotated(wind_direction,0,1,0);
	material_set(1,.5,0,1);
	glutSolidCone(.5,wind_magnitude/10,5,5);
	glPopMatrix();



	//compute info from varibles
	b_position();
	glPushMatrix();
	  
	  glTranslated(b_pos[0],b_pos[1],b_pos[2]);
	  //glTranslated(offset,0,0);
	  glRotated(b_dir,0,1,0); //heading of boat
	  glRotated(90,1,0,0);
	  glRotated(270,0,0,1);

	
	//draws boat
	
	  draw_particle();// spray of ocean
	  glRotated(wave_tilt,1,0,0); //wave tilt of boat
	  glRotated(heel,0,1,0); // wind tilt 
	
	  material_set(.2,.2,.2,1);
	  draw_obj(hull);

	  material_set(.2,1,.2,1);
	  draw_obj(cabin);

	  material_set(.5,.5,.5,1);
	  draw_obj(jib_sail);
	
	  glRotated(sheet,0,0,1);
	  material_set(1,0,0,1);
	  draw_obj(mast_boom);
	
	  material_set(.5,.5,.5,1);
	  draw_obj(main_sail);
	  
	
	  glPopMatrix();
	
	

}

void CGL_Window::draw()
{
	int width, height;
	glEnable(GL_DEPTH_TEST);
	glDepthFunc(GL_LESS);
	glBlendFunc(GL_SRC_ALPHA,GL_ONE_MINUS_SRC_ALPHA);
	glEnable(GL_BLEND);
	glShadeModel(GL_SMOOTH);
	glEnable(GL_LIGHTING);
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();

	width=w(); height=h();
	glViewport(0, 0, width, height);
	gluPerspective(60, (float)width/height, 1, 360.);
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();

	gluLookAt(rx,ry,rz, b_pos[0],0, b_pos[2], 0, -1, 0);
	//gluLookAt(0,0, -10,0,0,0, 0, -1, 0);
	light();
	//  clear stuff on the canvas
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	glPushMatrix();

		drawPicture();
	glPopMatrix();
}

int CGL_Window::handle(int event)
{
	static int num,x,y;
	switch(event){
	case FL_PUSH:
		num=Fl::event_button();
		x=Fl::event_x();
		y=Fl::event_y();
		//printf("pushed %d x %d y %d\n",num,x,y);
		return 1;
	case FL_DRAG:
		//... mouse moved while down event ...
		return 1;
	case FL_RELEASE:
		//... mouse up event ...
		return 1;
	case FL_FOCUS :
		return 1;
	case FL_UNFOCUS :
		//... Return 1 if you want keyboard events, 0 otherwise
		return 1;
	case FL_KEYBOARD:
		//... keypress, key is in Fl::event_key(), ascii in Fl::event_text()
		//... Return 1 if you understand/use the keyboard event, 0 otherwise...
		return 1;
	case FL_SHORTCUT:
		//... shortcut, key is in Fl::event_key(), ascii in Fl::event_text()
		//... Return 1 if you understand/use the shortcut event, 0 otherwise...
		return 1;
	default:
		// tell FLTK that I don't understand other events
		return 0;
  }
}
void CGL_Window::camera(double value, char type){
	switch(type){
	case 'x': rx=value; break;
	case 'y': ry=value; break;
	case 'z': rz=value; break;
	case 'Z': zm=value; break;
	}
}

void CGL_Window::adjust(double value, char type){
	switch(type){ 
	case 's': sheet=value; break;
	case 'd': wind_direction=value; break;
	case 'W': wind_magnitude=value; break;
	case 'j': wave_direction=value; break;
	case 'l': wave_length=value; break;
	case 'h': wave_height=value; break;
	case 'r': rudder=value; break;
	case 'k': rk4_h=value; break;
	case 'v': wave_varince=value; break;
	case 'a': offset=value;break; 
	}
}
 
/*********************************************************************\
 * Call to display an_object
\*********************************************************************/
void  CGL_Window::draw_obj(Obj mesh) {
	//draw frame
	glBegin( GL_TRIANGLES);
	for(int i=0; i<mesh.num_faces; i++)
	{
		//--- This is how to use the provided texture coordinates
		// glTexCoord2f( sph01.uvs[sph01.v_idx[i]].x, sph01.uvs[sph01.v_idx[i]].y );
		
		glNormal3f( mesh.normals[mesh.n_idx[i]].x,
					mesh.normals[mesh.n_idx[i]].y,
					mesh.normals[mesh.n_idx[i]].z );
		glVertex3f(	mesh.vertices[mesh.v_idx[i]].x,
					mesh.vertices[mesh.v_idx[i]].y,
					mesh.vertices[mesh.v_idx[i]].z );					
	}

	glEnd();
}
void  CGL_Window::draw_water(){
	double dir, dir1;
	int size=100; 
	material_set(0,0,1,1);
	glTranslatef(0,1,0);
	for(int i=0;i<size;i++){
		for(int j=0;j<size;j++){
			material_set((rand()%10)/50,(rand()%10)/50.0,1,.7);
			dir=sin(((i+(count*(rk4_h+1)))*wave_length))*wave_height;
			dir1=sin(((i+1+(count*(rk4_h+1)))*wave_length))*wave_height;
			wave_tilt=dir*5; //tilt dew to wave
			b_pos[1]=dir*.5; //bounce dew to wave	
			glBegin(GL_POLYGON);
			glNormal3d(0,dir,0);
			glVertex3f(i,dir,j);
			glVertex3f(i,dir,j+1);
			glVertex3f(i+1,dir1,j+1);
			glVertex3f(i+1,dir1,j);
			glEnd();
		}
	}	
	
}
	//points of sail
void CGL_Window::weather(){
	b_dir=45;
	sheet=-10;
}
void CGL_Window::reach(){
		b_dir=wind_direction;
		sheet=-45;
}

void CGL_Window::down_wind(){
	b_dir=-90;
	sheet=-100;
}
void CGL_Window::tack(){
	b_dir=90+(90-b_dir);
	sheet=sheet*(-1);// switchs the sail
}
void CGL_Window::jibe(){
	b_dir=270+(90-b_dir);// switching the tack
	sheet=sheet*(-1);// switchs the sail
}
void CGL_Window::b_position(){
	//resets boat if at edge of water
	if(b_pos[0]>100||b_pos[0]<0 || b_pos[2]>100||b_pos[2]<0)
		 restart();
	double ang=wind_direction+90-(b_dir+sheet); //angle between wind and sail
			ang=ang*((22.0/7.0)/180.0);// convert to radians for use in trig functions
	double foward=cos(ang)*-5;  //lift force from wind on sail coverted to foward 
	heel=-2*wind_magnitude*sin(ang);//heel of boat dew to wind magnitude and direction
	//seperates out force componet for each direction
	b_vel[0]=cos(b_dir*(22.0/7.0)/180.0)*(foward)*(.3*wind_magnitude); 
	b_vel[2]=sin(b_dir*(22.0/7.0)/180.0)*(foward)*(.3*wind_magnitude);
	 
	//special sailing to weather conditions
	if(b_dir==135||b_dir==45)
		b_vel[0]=-b_vel[0];
	
	// moves the boat according to euler
	b_pos[0]=b_pos[0]+(b_vel[0]*rk4_h); 
	b_pos[2]=b_pos[2]+(b_vel[2]*rk4_h);
	

	//steering of boat
	b_dir=b_dir+rudder;

}

void CGL_Window::material_set(double r,double g,double b,double t){
	GLfloat mat[] = {r,g,b,t};	
	GLfloat nomat[]={0,0,0,0};
		  glMaterialfv(GL_FRONT_AND_BACK, GL_AMBIENT, mat);
		  glMaterialfv(GL_FRONT_AND_BACK, GL_DIFFUSE, mat);
		  glMaterialfv(GL_FRONT_AND_BACK, GL_SPECULAR, mat);
		  glMaterialf(GL_FRONT_AND_BACK, GL_SHININESS, 50.0);

		  glMaterialfv(GL_FRONT, GL_EMISSION, nomat);

  }
void CGL_Window::light(){
	float light0ambient[] = {0.3, 0.3, 0.3, 1.0};
	float light0specular[] = {.8, .8, .8, 1.0};
	float light0diffuse[] = {0.2, 0.2, 0.2, 1.0};
 	float light0position[] = {0, 30, 10, 1}; 
	float light0direction[] = {0.0, -1.0, 0.0};
	glLightfv(GL_LIGHT0, GL_AMBIENT, light0ambient);
	glLightfv(GL_LIGHT0, GL_SPECULAR, light0specular);
	glLightfv(GL_LIGHT0, GL_POSITION, light0position);
	glLightfv(GL_LIGHT0, GL_SPOT_DIRECTION, light0direction);
	glLightf(GL_LIGHT0, GL_SPOT_CUTOFF, 360);
	glLightf(GL_LIGHT0, GL_SPOT_EXPONENT, 2);
	glEnable(GL_LIGHT0);
	material_set(1,0,0,1);
	glutSolidCone(5,40,10,10);

}

//particles functions
/********************/
void CGL_Window::position_particle(particle* part){          
	double k1,k2,k3,k4,j1,j2,j3,j4;
	//limits the area of particle projection
	if(part->pos.y<-8 || part->pos.y>0 || 
		part->pos.x<-2 || part->pos.x>2 ||
		part->pos.z<-2 || part->pos.z>0)
			reset_particle(part);
	
	part->color[0]+=.05;
	part->color[1]+=.05;
	part->color[2]+=.05;

	//rk4
	k1=part->pos.y;
	j1=-gra;
	k2=(rk4_h/2)*j1+part->vel.y;
	j2=-gra;
	k3=(rk4_h/2)*j2+part->vel.y;
	j3=-gra;
	k4=(rk4_h*j3)+part->vel.y;
	j4=-gra;
	part->pos.y= part->pos.y + (rk4_h/6)*(k1+(2*k2)+(2*k3)+k4);
	part->vel.y= part->vel.y + (rk4_h/6)*(j1+(2*j2)+(2*j3)+j4);

	//euler
	part->pos.z= part->pos.z+(rk4_h*part->vel.z);
	part->pos.x= part->pos.x+(rk4_h*part->vel.x);
	part->life+=1;
}

void CGL_Window::reset_particle(particle* part){
	//resets position
	float ran=rand();
	//boat position
	part->pos.x=0;                                                         
	part->pos.y=0;                                                         
	part->pos.z=0;                                                         
	//resets velocity
	part->vel.x=sin(ran)*random_ness*sin(rand());                                                              
	part->vel.y=(velocity+1)*sin(ran)*random_ness*sin(rand())*.5;
	part->vel.z=cos(ran)*random_ness*sin(rand()); 
	//reset color
	part->color[0]=intial_color1;
	part->color[1]=intial_color2;
	part->color[2]=intial_color3;
	//reset life counter
	part->life=0;
	
}

void CGL_Window::draw_particle(){ 
	  for(int i=0;i<ps.num_particles;i++){
			glPushMatrix();
			glTranslated(0,-b_pos[1],0);
			position_particle(&ps.particles[i]);
			
			material_set(ps.particles[i].color[0],
						 ps.particles[i].color[1],
						 ps.particles[i].color[2],.5
						 );
			glTranslatef(ps.particles[i].pos.x,
						 ps.particles[i].pos.y+3,
					     ps.particles[i].pos.z-.5);     	
			glutSolidSphere(.007*(rand()%100), 4, 4);
		  glPopMatrix();
	}
}

/*********************/

//resets all values of the boat
void CGL_Window::restart(){
	count=1;
	rk4_h=.1;
//set by slider
	//camera
	rx=40;ry=-15;rz=40;
	zm=0;
	//wind varibles
	wind_direction=0;
	wind_magnitude=5; 
	//wave varibles
	wave_direction=0;
	wave_length=.35;
	wave_height=.5; 
	wave_varince=1;
	offset=5;
	//boat varibles
	rudder=0;
	sheet=0; 
	b_dir=0;
//set by math
	// boat globals 
	heel=0;  //heel of the boat dew to wind
	wave_tilt=0;  //bounce of the boat in surf
	//boat position
	b_pos[0]=50;
	b_pos[1]=0;
	b_pos[2]=50;

	b_vel[0]=0;
	b_vel[1]=0;

}
// memory clean up
void CGL_Window::quit_cb(void){
	exit(0);
}
