/*
 *  Target.cpp
 *  openFrameworks
 *
 *  Created by Robert Carlsen on 24.04.2009.
 *  Copyright 2009 recv'd productions. All rights reserved.
 *
 */

#include "Target.h"

Target::Target(int _x, int _y){
	r = 3;
	x = _x;
	y = _y;
	
	size = 5;
}


void Target::update(int _x, int _y){
	x = _x;
	y = _y;
}


void Target::display(){
	ofSetColor(255,0,0);
	ofFill();
	ofSetRectMode(OF_RECTMODE_CENTER);


	glPushMatrix();
	glTranslatef(float(x), float(y), 0.0);
	glScalef(size,size,0);

	ofRect(-r,0,3,1);
	ofRect(r,0,3,1);
	ofRect(0,-r,1,3);
	ofRect(0,r,1,3);

	ofRect(-r+1,0,1,3);
	ofRect(r-1,0,1,3);
	ofRect(0,-r+1,3,1);
	ofRect(0,r-1,3,1);

	glPopMatrix();
}