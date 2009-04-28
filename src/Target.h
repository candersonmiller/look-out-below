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
    
    Target(int _x, int _y);
    
    void update(int _x, int _y);
    
    void display();
};

#endif
