
#include <math.h>
#include <GL/gl.h>
#include <GL/glu.h>

#include "camera.h"

void camera::collide_with_heightmap(heightmap& hm) {	
	float hmh=hm.get_height(pos.x,pos.y)+0.1;
	if(hmh>pos.z) pos.z=hmh;
}

void camera::follow_pos(const vect &Pos,float speed,float timediff) {
	float ratio=powf(speed,timediff);
	//vazeny prumer ze stare a nove pozice
	pos=(ratio*pos)+((1-ratio)*Pos);
}

void camera::follow_ori(const quat &Ori,float speed,float timediff) {
	float ratio=powf(speed,timediff);
	ori=(ratio*ori)+((1-ratio)*Ori);
	ori.normalize();
}

void camera::set_gl() {
	//bod tesne pred kamerou
	vect t=pos-ori.vecz();
	//smer nahoru nad kameru
	vect u=ori.vecy();
	//nastavi kameru
	gluLookAt(pos.x,pos.y,pos.z,
		t.x,t.y,t.z,
		u.x,u.y,u.z);
}
