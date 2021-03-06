/*
 *  Dropping.h
 *  openFrameworks
 *
 *  Created by Robert Carlsen on 24.04.2009.
 *  Copyright 2009 recv'd productions. All rights reserved.
 *
 */
#ifndef _DROPPING
#define _DROPPING

#include "ofMain.h"
#include "HitArea.h"
#include "Interpolator.h"

#define kGravity 9.8f;

class Dropping {
public:

//    PImage splatImg;    
//    public Dropping(int _x, int _y, PImage _splat){
//        x=_x;
//        y=_y;
//        timestamp = ofGetElapsedTimeMillis();
//        initialSize = ofRandom(50, 100);
//        height = 3;
//        
//        splatImg = _splat;
//    }
    
    Dropping(int _x, int _y);
	void update();
    
	void display();
    
	void hitTest();
	
	bool discard;
    bool onGround;
    bool tagged;
    int x;
    int y;
    
	void square();
	void circle(float radius);
	void splat(float radius);
    void fadeOut();
    void setHit(bool _hit);
    
private:
    
    int height;
    float initialSize;
    float size;
    
    float counter;

	int timestamp;
	
    bool hit;
    HitArea *hitArea;
    
    Interpolator *alpha;
    bool isFading;
    
};


#endif