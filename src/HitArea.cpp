/*
 *  HitArea.cpp
 *  openFrameworks
 *
 *  Created by Robert Carlsen on 24.04.2009.
 *  Copyright 2009 recv'd productions. All rights reserved.
 *
 */

#include "HitArea.h"


HitArea::HitArea(int _x, int _y){
	r = 30.0;
	scale = 1.0;
	timestamp = ofGetElapsedTimeMillis();
	rotationIncrement = PI/2;
	angle = ofRandom(0,360.0f); // start at a random angle
    
	x = _x;
	y = _y;
	
	hit = false;
	
	animatedRadius = new Interpolator();
	animatedRadius->target(r);
	
	alpha = new Interpolator(0.1f,0.009f,0.95f);
	alpha->target(200);
    
	isDisposed = false;
	isTransparent = false;
//	font1.loadFont("type/frabk.ttf",18);
    font1.loadFont("type/Arial Bold.ttf",12);

	update();
}


void HitArea::update(){
	// increment the label's rotation angle:
	angle += rotationIncrement;
	
	// animate the radius?
	// how about using the integrator class?
	animatedRadius->update();
	r = animatedRadius->get();
	
	alpha->update();
	
	if(alpha->get() <= 0) {
        alpha->set(0);
        isTransparent = true;
    }
}

void HitArea::setHit(){
	hit = true;
}

void HitArea::display(){
	if(hit){
		glColor4f(0,1.0,0,(float)alpha->get());
	}else{
		glColor4f((float)200/255,(float)200/255,(float)30/255,(float)alpha->get());
	}
	//ofSetColor(200,200,30,(int)alpha->get()); // use r,g,b,a
	ofNoFill();
	ofSetRectMode(OF_RECTMODE_CENTER);
	
	//ofEllipse(x,y,r*2,r*2);
	
	drawLabel(); // display the rotating label
}


void HitArea::move(int _x, int _y){
	x = _x;
	y = _y;
}


void HitArea::drawLabel(){
	// display a text label around the hit area
	ofPushMatrix();
	ofTranslate(x,y,0); // move to the hit location
	ofRotateZ(angle);    // rotate to the current angle
	
	// repeat several times?
	
	
	if(hit){
		int num = 24;
		float stepIncrement = 360.0f/num;
		float a = 0;
		char missed[24] = {'H',' ','i','t','!',' ',' ',' ','H',' ','i','t','!',' ',' ',' ','H',' ','i','t','!',' ',' ',' '};
		char stringToPrint[25];
		
		for(int i = 0; i< num;i++){
			ofPushMatrix();
			ofRotateZ(a+=stepIncrement);
			ofTranslate(r*1.5,0,0); // push out to the desired radius
			// TODO: use an image / texture?
			ofFill();
			//char printchar[20];
			//sprintf(printchar,"%s",missed[i]);
			
			sprintf(stringToPrint,"%c",missed[i]);
			glPushMatrix();
			glRotated(120.0f, 0.0, 0.0, 1.0);
			font1.drawString(stringToPrint, 10, 10);
			glPopMatrix();
			//ofSetRectMode(OF_RECTMODE_CENTER);
			//ofRect(0,0,5,5);
			ofPopMatrix();
		}
	}else{
		int num = 22;
		float stepIncrement = 360.0f/num;
		float a = 0;
		char missed[22] = {'M',' ','i','s','s','e','d',' ',' ',' ',' ','M',' ','i','s','s','e','d',' ',' ',' ',' '};
		char stringToPrint[25];
		
		for(int i = 0; i< num;i++){
			ofPushMatrix();
			ofRotateZ(a+=stepIncrement);
			ofTranslate(r*1.5,0,0); // push out to the desired radius
			// TODO: use an image / texture?
			ofFill();
			//char printchar[20];
			//sprintf(printchar,"%s",missed[i]);
			
			sprintf(stringToPrint,"%c",missed[i]);
			glPushMatrix();
			glRotated(120.0f, 0.0, 0.0, 1.0);
			font1.drawString(stringToPrint, 10, 10);
			glPopMatrix();
			//ofSetRectMode(OF_RECTMODE_CENTER);
			//ofRect(0,0,5,5);
			ofPopMatrix();
		}
	}
	ofPopMatrix();
}

void HitArea::dispose(){
	isDisposed = true;
	alpha->target(0);
}

bool HitArea::isDead(){
	if(isDisposed && isTransparent) return true;
	return false;
}