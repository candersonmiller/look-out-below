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

#define kGravity 9.8f;

class Dropping {
public:
    int x;
    int y;
    int timestamp;
    int height;
    float initialSize;
    float size;
    bool onGround;
    bool discard;
    bool hit;
    HitArea *hitArea;

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
    
    Dropping::Dropping(int _x, int _y) : onGround(false), discard(false), hit(false){
        x=_x;
        y=_y;
        timestamp = ofGetElapsedTimeMillis();
        initialSize = ofRandom(50, 100);
        height = 3;
    
        hitArea = new HitArea(x,y);
    };
    
    void update(){
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
    
    void display(){
        ofSetColor(204,204,204);
        ofFill();
        
        if(!onGround){
            ofEllipse(x,y,size,size);
        } else {
            ofSetColor(235,150,100);
            ofFill();
            ofRect(x,y,size,size);
            //        pushMatrix();
            //        float imgSize = 20;
            //        translate(x - imgSize/2, y - imgSize/2);
            //        image(splatImg,0,0,20,20);
            //        popMatrix();
            
            if(hit)
                hitArea->display();
        }
    }
    
    void hitTest(){
        // eventually test if an object (person) has been hit
        // for now, abstract splat
        onGround = true;
        
        // should actually test whether a hit was made or not, but for now just go true
        // TODO: implement hit test
        hit = true;
        hitArea->move(x,y);
        
        timestamp = ofGetElapsedTimeMillis();
    }
    
    
};


#endif