/*
 *  interpolator.cpp
 *  openFrameworks
 *
 *  Created by C. Anderson Miller on 4/28/09.
 *  Copyright 2009 __MyCompanyName__. All rights reserved.
 *
 */
#include "Interpolator.h"


Interpolator::Interpolator(){ 
	value = 0;
	vel = 0;
	accel = 0;
	force = 0;
	mass = 1;
	damping = kDamping;
	attraction = kAttraction;
}

Interpolator::Interpolator(float _value) {
	vel = 0;
	accel = 0;
	force = 0;
	mass = 1;
	value = _value;
	damping = kDamping;
	attraction = kAttraction;
}

Interpolator::Interpolator(float _value, float _attraction, float _damping) :
vel(0.0f), accel(0.0f), force(0.0f), mass(1.0f){
	vel = 0;
	accel = 0;
	force = 0;
	mass = 1;
	value = _value;
	damping = _damping;
	attraction = _attraction;
}

void Interpolator::set(float _value) {
	value = _value;
}

float Interpolator::get() {
	return value;
}


void Interpolator::setAttraction(float _attraction) {
	attraction = _attraction;
}



void Interpolator::setDamping(float _damping) {
	damping = _damping;
}

bool Interpolator::update() {
	if (targeting) {
		force += attraction * (targetVal - value);
		accel = force / mass;
		vel = (vel + accel) * damping;
		value += vel;
		force = 0;
		float absVel = (vel<0) ? vel*-1:vel;
		return (absVel > 0.0001f);
	}
	return false;
}

void Interpolator::setIsTargeting(bool _t){
	targeting = _t;
}



void Interpolator::target(float t) {
	targeting = true;
	targetVal = t;
}