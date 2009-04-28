/*
 *  Dropping.cpp
 *  openFrameworks
 *
 *  Created by Robert Carlsen on 24.04.2009.
 *  Copyright 2009 recv'd productions. All rights reserved.
 *
 */

#include "Dropping.h"

Dropping::Dropping(int _x, int _y){
	onGround = false;
	discard = false;
	hit = false;
	x=_x;
	y=_y;
	timestamp = ofGetElapsedTimeMillis();
	initialSize = ofRandom(50, 100);
	height = 3;
    
	hitArea = new HitArea(x,y);
};

void Dropping::update(){
	// exponential scale
	// need to first determine the distance
	// using these equations:
	// v = g * t
	// d = 0.5 * g * t^2
	if(!onGround){
		float secs = (ofGetElapsedTimeMillis() - timestamp) / 1000.0f;
		float dist = 0.5f * (pow(secs,2));
		dist *= kGravity;
		
		if(dist > height) // hit
			hitTest();
		else{
			//size = pow(size,1/dist);
			size = pow(initialSize,1/((secs+0.23f)*2)); // fudge factor for scaling
			//size = min(initialSize, size); // make sure that it doesn't get too large
		}
	} else {
		// the timestamp is reset once the ground has been hit
		// set the discard flag after several seconds
		hitArea->update();
		
		if(ofGetElapsedTimeMillis() - timestamp > 7000){
			if(!hitArea->isDisposed) hitArea->dispose();
			
			if(hitArea->isDead())
				discard = true;
		}
		
	}
}


void Dropping::square(){
	glPushMatrix();
	glColor3d(1.0,1.0,1.0);
	glTranslated(x-(0.5 * size),y-(0.5 * size),0);
	glBegin(GL_TRIANGLE_FAN);
		glVertex3f(0,0,0);
		glVertex3f(0,size,0);
		glVertex3f(size,size,0);
		glVertex3f(size,0,0);
	glEnd();
	glPopMatrix();
}


void Dropping::circle(float radius){
	float x1,y1;

	glPushMatrix();
	glTranslated(x,y,0);
	glBegin(GL_TRIANGLE_FAN);
	glColor3f(1.0f,1.0f,1.0f);
	
	x1 = (float)radius * cos(359 * PI/180.0f);
	y1 = (float)radius * sin(359 * PI/180.0f);
	for(int j = 0; j < 360; j++)
	{
		glVertex2f(x1,y1);
		x1 = (float)radius * cos(j * PI/180.0f);
		y1 = (float)radius * sin(j * PI/180.0f);
		glVertex2f(x1,y1);
	}
	glEnd();
	glPopMatrix();
}

void Dropping::splat(float radius){
	float x1,y1;
	float counter =0;
	glPushMatrix();
	glTranslated(x,y,0);
	glBegin(GL_TRIANGLE_FAN);
	glColor3f(1.0f,1.0f,1.0f);
	
	x1 = (float)radius * cos(359 * PI/180.0f);
	y1 = (float)radius * sin(359 * PI/180.0f);
	for(int j = 0; j < 360; j++)
	{
		
		glVertex2f(x1,y1);
		x1 = ((float)radius + cos(counter) )* cos(j * PI/180.0f);
		y1 = ((float)radius + cos(counter) ) * sin(j * PI/180.0f);
		glVertex2f(x1,y1);
		counter+=0.1;
	}
	glEnd();
	glPopMatrix();
}




void Dropping::display(){
	ofSetColor(204,204,204);
	ofFill();
	
	if(!onGround){
		//ofEllipse(x,y,size,size);
		//circle(size);
		circle(size);
	} else {
		//ofSetColor(235,150,100);
		//ofFill();
		
		//glColor4f(float(235/255),float(150/255),float(100/255),1.0f);
		//square();
		splat(size);
		//ofRect(x,y,size,size);
		//        pushMatrix();
		//        float imgSize = 20;
		//        translate(x - imgSize/2, y - imgSize/2);
		//        image(splatImg,0,0,20,20);
		//        popMatrix();
		
		if(hit)
			hitArea->display();
	}
}




void Dropping::hitTest(){
	// eventually test if an object (person) has been hit
	// for now, abstract splat
	onGround = true;
	
	// should actually test whether a hit was made or not, but for now just go true
	// TODO: implement hit test
	hit = true;
	hitArea->move(x,y);
	
	timestamp = ofGetElapsedTimeMillis();
}