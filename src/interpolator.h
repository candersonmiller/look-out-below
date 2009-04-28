/*
 *  interpolator.h
 *  CloudReader
 *
 *  Originally Intrgrator.java from Ben Fry. Thanks Ben!
 *
 *  Ported by Robert Carlsen on 01.04.2009.
 *
 */
#ifndef _INTERPOLATOR
#define _INTERPOLATOR

#define kAttraction 0.1f
#define kDamping 0.9f

class Interpolator {
    
    float value;
    float vel;
    float accel;
    float force;
    float mass;
    
    float damping;
    float attraction;
    
    bool targeting;
    float targetVal;

public:
    // Constructors:
	Interpolator();
	Interpolator(float _value);    
	Interpolator(float _value, float _attraction, float _damping);
    
    // Methods:
	void set(float _value);
    
	float get();
    
	void setAttraction(float _attraction);
	void setDamping(float _damping);
    
    
	bool update();
    
	void setIsTargeting(bool _t);
	void target(float t);
};
#endif