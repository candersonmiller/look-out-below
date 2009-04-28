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
	
	x = _x;
	y = _y;
	
	animatedRadius = new Interpolator();
	animatedRadius->target(r);
	
	alpha = new Interpolator(0.0f,0.009f,0.95f);
	alpha->target(200);
	
	isDisposed = false;
	isTransparent = false;
	
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
	
	if(alpha->get() <= 0) isTransparent = true;
}

void HitArea::display(){
	ofEnableAlphaBlending();
	ofSetColor(200,200,30,(int)alpha->get()); // use r,g,b,a
	ofNoFill();
	ofSetRectMode(OF_RECTMODE_CENTER);
	
	ofEllipse(x,y,r*2,r*2);
	
	drawLabel(); // display the rotating label
	ofDisableAlphaBlending();
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
	int num = 10;
	float stepIncrement = 360.0f/num;
	float a = 0;
	for(int i = 0; i< num;i++){
		ofPushMatrix();
		ofRotateZ(a+=stepIncrement);
		ofTranslate(r,0,0); // push out to the desired radius
		// TODO: use an image / texture?
		ofFill();
		ofSetRectMode(OF_RECTMODE_CENTER);
		ofRect(0,0,5,5);
		ofPopMatrix();
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