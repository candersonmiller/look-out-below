/*
 *  HitArea.h
 *  openFrameworks
 *
 *  Created by Robert Carlsen on 24.04.2009.
 *  Copyright 2009 recv'd productions. All rights reserved.
 *
 */

// this should encircle the subject, to let them know that they were hit or not.

#ifndef _HITAREA
#define _HITAREA

#include "ofMain.h"
#include "interpolator.h"

class HitArea {
public:
    
    int x;
    int y;
    float r;
    float scale;
    long timestamp;
    float angle;
    float rotationIncrement;
    
    Interpolator *animatedRadius;
    Interpolator *alpha;
    
    bool isDisposed;
    bool isTransparent;
    
    HitArea(int _x, int _y) : 
                                r(30.0f), 
                                scale(1.0f), 
                                timestamp(ofGetElapsedTimeMillis()),
                                rotationIncrement(PI/2.0f) // has to be degrees
    {
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
    
    void update(){
        // increment the label's rotation angle:
        angle += rotationIncrement;
        
        // animate the radius?
        // how about using the integrator class?
        animatedRadius->update();
        r = animatedRadius->get();
        
        alpha->update();
        
        if(alpha->get() <= 0) isTransparent = true;
    }
    
    void display(){
        ofEnableAlphaBlending();
        ofSetColor(200,200,30,(int)alpha->get()); // use r,g,b,a
        ofNoFill();
        ofSetRectMode(OF_RECTMODE_CENTER);
        
        ofEllipse(x,y,r*2,r*2);
        
        drawLabel(); // display the rotating label
        ofDisableAlphaBlending();
    }
    
    void move(int _x, int _y){
        x = _x;
        y = _y;
    }
    
    void drawLabel(){
        // display a text label around the hit area
        ofPushMatrix();
        ofTranslate(x,y,0); // move to the hit location
        ofRotateZ(angle);    // rotate to the current angle

        // repeat several times?
        int num = 5;
        float stepIncrement = 360.0f/num;
        float a = 0;
        for(int i = 0; i< num;i++){
            ofPushMatrix();
            ofRotateZ(a+=stepIncrement);
            ofTranslate(r,0,0); // push out to the desired radius
            // TODO: use an image / texture?
            ofFill();
            ofSetRectMode(OF_RECTMODE_CENTER);
            ofRect(0,0,10,10);
            ofPopMatrix();
        }
        
        ofPopMatrix();
    }
    
    void dispose(){
        isDisposed = true;
        alpha->target(0);
    }
    
    bool isDead(){
        if(isDisposed && isTransparent) return true;
        return false;
    }
};


#endif