#include <FL/Fl.H>
#include "gui.h"
#include <GL/glut.h>

//This is really all you need to do here!!!
gui *gu;

void my_idle(void*)
{
	if (!gu->animate->value()){
		gu->gl_window->animate_set=1;	
		return;
	}
	gu->gl_window->redraw();
}

int main(int argc, char **argv){
	Fl::gl_visual(FL_DEPTH | FL_DOUBLE| FL_RGB);
	gu= new gui();
	Fl::add_idle(my_idle);
	
    glutInit(&argc, argv);
    
    gu->show();

	return Fl::run();
}
