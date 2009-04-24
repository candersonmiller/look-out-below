/*
 *  Target.h
 *  openFrameworks
 *
 *  Created by Robert Carlsen on 24.04.2009.
 *  Copyright 2009 recv'd productions. All rights reserved.
 *
 */
#ifndef _TARGET
#define _TARGET

#include "ofMain.h"

class Target {
public:
    int x;
    int y;
    float size;
    int r;
    
    Target(int _x, int _y) : r(3){
        x = _x;
        y = _y;
        
        size = 5;
    }
    
    void update(int _x, int _y){
        x = _x;
        y = _y;
    }
    
    void display() {
        ofSetColor(255,0,0);
        ofFill();
        ofSetRectMode(OF_RECTMODE_CENTER);
        
        ofPushMatrix();
        ofTranslate(x,y,0);
        ofScale(size,size,0);
        
        ofRect(-r,0,3,1);
        ofRect(r,0,3,1);
        ofRect(0,-r,1,3);
        ofRect(0,r,1,3);
        
        ofRect(-r+1,0,1,3);
        ofRect(r-1,0,1,3);
        ofRect(0,-r+1,3,1);
        ofRect(0,r-1,3,1);
        
        ofPopMatrix();
    }
};

#endif
