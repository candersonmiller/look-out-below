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
#include "Interpolator.h"

class HitArea {
public:
    HitArea(int _x, int _y);    
    void update();
    void move(int _x, int _y);
    void drawLabel();
	void display();
    void dispose();    
    bool isDead();

	bool isDisposed;
	
private:
    int x;
    int y;
    float r;
    float scale;

    bool isTransparent;
	long timestamp;
    float angle;
    float rotationIncrement;
    
    Interpolator *animatedRadius;
    Interpolator *alpha;
};


#endif