
#include <iostream>
using namespace std;

#include <GL/gl.h>
#include <math.h>

#include "imageloader.h"
#include "dragon.h"
#include "world.h"

void dragon::init() {
	texture=imageloader_load("data/drak.png",3,GL_RGB);
	glBindTexture(GL_TEXTURE_2D,texture);
	glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MAG_FILTER,GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MIN_FILTER,GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_WRAP_S,GL_CLAMP);
	glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_WRAP_T,GL_CLAMP);
}

void dragon::update(int mouse_x,int mouse_y,bool left_mouse_down,bool right_mouse_down,bool space,float timediff,world &w) {
	vect VX,VY,VZ;
	ori.normalize();
	VX=ori.vecx();
	VY=ori.vecy();
	VZ=ori.vecz();
	
	//mavani kridel
	if(space) spd+=VZ*timediff*-5+VY*timediff*0.8;
	
	//otaceni mysi, zaber
	mom+=ori*(0.03*quat(0,mouse_y/2,0,-mouse_x));

	//gravitace
	spd.z-=timediff*2.5;

	//vztlak kridel
	spd+=((spd%VZ)*-timediff*0.3)*VY;

	//odpor plochy kridel
	spd+=((spd%VY)*-timediff*1.5)*VY;

	//odpor strany tela
	spd+=((spd%VX)*-timediff*0.5)*VX;
	
	//otaceni nahoru, aerodynamicka rotace
	mom+=timediff*(ori*(((spd%VZ)*-timediff)*quat(0,1,0,0)));

	//otaceni dolu, aerodynamicka rotace
	float tmp=VZ%vect(0,0,1), fwd=powf(0.4,(spd%VZ));
	mom+=timediff*quat(fwd*(1-tmp)*-timediff,VZ^vect(0,0,1));
	
	cout << tmp << endl;

	//fyzika
	pos+=spd*timediff;
	ori+=mom*timediff;
	
	//odpory prostredi
	spd*=powf(0.98,timediff);
	mom*=powf(0.2,timediff);

	//nabijeni
	reload+=timediff;

	//kolize s mapou
	float hmh=w.hm.get_height(pos.x,pos.y)+0.1;
	if(hmh>pos.z) {
		pos.z=hmh;
		//na odlepovani od zeme
		if(spd.z<0) spd.z=0;
	}

	//okraje mapy
	float maxsx, maxsy;
	w.hm.get_sizes(maxsx,maxsy);
	if(pos.x<0) pos.x=0;
	if(pos.y<0) pos.y=0;
	if(pos.x>maxsx) pos.x=maxsx;
	if(pos.y>maxsy) pos.y=maxsy;
	//TODO maxz
	
	//strelba
	if(left_mouse_down && reload>=0) {
		missile& m = w.ms.add_one();
		m.pos=pos;
		m.spd=-20*VZ;
		m.type=missile_dragon_fire;
		m.power=1;
		reload-=0.1;
	}
	if(right_mouse_down && reload>=0) {
		missile& m = w.ms.add_one();
		m.pos=pos;
		m.spd=-20*VZ;
		m.type=missile_dragon_ball;
		//TODO m.power;
		reload-=0.1;
	}
}

void dragon::draw() {
	glPushMatrix();
	glTranslatef(pos.x,pos.y,pos.z);
	ori.gl_rotate();

	vect points[][2] = {
		// hlava
		{vect(0,19,0),vect(0,1,0)}, 
		{vect(1,19,0),vect(0.707,0.707,0)},
		{vect(0,17,1),vect(0,0,1)},
		{vect(2,17,0),vect(1,0,0)},
		{vect(1,16,0),vect(0.707,-0.707,0)},
		//telo
		{vect(1,13,0),vect(1,0,0)},
		{vect(0,11,1.5),vect(0,0,1)},
		{vect(2,11,0),vect(1,0,0)},
		{vect(2,8,0),vect(1,0,0)},
		{vect(0,7,1),vect(0,0,1)},
		{vect(1.5,7,0),vect(1,0,0)},
		//ocas
		{vect(0,4,0.5),vect(0,0,1)},
		{vect(0.5,4,0),vect(1,0,0)},
		{vect(0,0,0),vect(0,-1,0)},
		//kridlo
		{vect(5,12,0),vect(0,0,1)},
		{vect(8,11,0),vect(0,0,1)},
		{vect(10,9,0),vect(0,0,1)},
		{vect(4,8,0),vect(0,0,1)},
		{vect(7,8,0),vect(0,0,1)},
		{vect(11,6,0),vect(0,0,1)},
		//telo spodek = brisko
		{vect(0,17,-1),vect(0,0,-1)},
		{vect(0,11,-1.5),vect(0,0,-1)},
		{vect(0,7,-1),vect(0,0,-1)},
		{vect(0,4,-0.5),vect(0,0,-1)},

	};
	
#define point(X) glNormal3fv(points[X-1][1].v); glTexCoord2f(points[X-1][0].x/19,points[X-1][0].y/24); glVertex3fv(points[X-1][0].v);

	glPushMatrix();
	glScalef(0.2,0.2,0.2);
	glRotatef(-90,1,0,0);
	glTranslatef(0,-10,0);
	glColor3f(1,1,1);
	//glColor3f(0.2,0.5,0.1);
	
	glEnable(GL_TEXTURE_2D);
	glBindTexture(GL_TEXTURE_2D,texture);

	//telo zprava shora
	glBegin(GL_TRIANGLE_FAN);
		point(3)
		point(7)
		point(6)
		point(5)
		point(4)
		point(2)
		point(1)
	glEnd();
	glBegin(GL_TRIANGLE_FAN);
		point(7)
		point(10)
		point(11)
		point(9)
		point(8)
		point(6)
	glEnd();
	glBegin(GL_TRIANGLE_STRIP);
		point(11)
		point(10)
		point(13)
		point(12)
		point(14)
	glEnd();
	//prave kridlo shora
	glBegin(GL_TRIANGLE_FAN);
		point(19)
		point(20)
		point(17)
		point(16)
		point(15)
		point(18)
	glEnd();
	glBegin(GL_TRIANGLE_STRIP);
		point(9)
		point(18)
		point(8)
		point(15)
	glEnd();
	//telo zprava zdola
	glBegin(GL_TRIANGLE_FAN);
		point(21)
		point(1)
		point(2)
		point(4)
		point(5)
		point(6)
		point(22)
	glEnd();
	glBegin(GL_TRIANGLE_FAN);
		point(22)
		point(6)
		point(8)
		point(9)
		point(11)
		point(23)
	glEnd();
	glBegin(GL_TRIANGLE_STRIP);
		point(14)
		point(23)
		point(13)
		point(23)
		point(11)
	glEnd();
	//prave kridlo zdola
	glPushMatrix();
	glRotatef(180,0,1,0);
	glScalef(-1,1,1);
	glBegin(GL_TRIANGLE_FAN);
		point(19)
		point(18)
		point(15)
		point(16)
		point(17)
		point(20)
	glEnd();
	glBegin(GL_TRIANGLE_STRIP);
		point(15)
		point(18)
		point(8)
		point(9)
	glEnd();
	glPopMatrix();

	glScalef(-1,1,1);

	//telo zleva shora
	glBegin(GL_TRIANGLE_FAN);
		point(3)
		point(1)
		point(2)
		point(4)
		point(5)
		point(6)
		point(7)
	glEnd();
	glBegin(GL_TRIANGLE_FAN);
		point(7)
		point(6)
		point(8)
		point(9)
		point(11)
		point(10)
	glEnd();
	glBegin(GL_TRIANGLE_STRIP);
		point(14)
		point(12)
		point(13)
		point(10)
		point(11)
	glEnd();
	//leve kridlo shora
	glBegin(GL_TRIANGLE_FAN);
		point(19)
		point(18)
		point(15)
		point(16)
		point(17)
		point(20)
	glEnd();
	glBegin(GL_TRIANGLE_STRIP);
		point(15)
		point(18)
		point(8)
		point(9)
	glEnd();
	//telo zleva zdola
	glBegin(GL_TRIANGLE_FAN);
		point(21)
		point(22)
		point(6)
		point(5)
		point(4)
		point(2)
		point(1)
	glEnd();
	glBegin(GL_TRIANGLE_FAN);
		point(22)
		point(23)
		point(11)
		point(9)
		point(8)
		point(6)
	glEnd();
	glBegin(GL_TRIANGLE_STRIP);
		point(11)
		point(23)
		point(13)
		point(23)
		point(14)
	glEnd();
	//leve kridlo zdola
	glPushMatrix();
	glRotatef(180,0,1,0);
	glScalef(-1,1,1);
	glBegin(GL_TRIANGLE_FAN);
		point(19)
		point(20)
		point(17)
		point(16)
		point(15)
		point(18)
	glEnd();
	glBegin(GL_TRIANGLE_STRIP);
		point(15)
		point(8)
		point(18)
		point(9)
	glEnd();
	glPopMatrix();

	glPopMatrix();

	glDisable(GL_TEXTURE_2D);

	glPopMatrix();
}

//vraci vhodou pozici kamery vuci drakovi
vect dragon::camera_pos() {
	return pos+ori.vecz()*10+ori.vecy()*3; 
}

void dragon::finish() {
	imageloader_free(texture);
}

