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
    Interpolator() : value(0.0f), vel(0.0f), accel(0.0f), force(0.0f), mass(1.0f) { 
        damping = kDamping;
        attraction = kAttraction;
    }
    
    
    Interpolator(float _value) : vel(0.0f), accel(0.0f), force(0.0f), mass(1.0f) {
        value = _value;
        damping = kDamping;
        attraction = kAttraction;
    }
    
    
    Interpolator(float _value, float _attraction, float _damping) :
                                vel(0.0f), accel(0.0f), force(0.0f), mass(1.0f){
        value = _value;
        damping = _damping;
        attraction = _attraction;
    }
    
    // Methods:
    void set(float _value) {
        value = _value;
    }
    
    
    float get() {
        return value;
    }
    
    
    void setAttraction(float _attraction) {
        attraction = _attraction;
    }
    
    
    void setDamping(float _damping) {
        damping = _damping;
    }
    
    
    bool update() {
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
    
    void setIsTargeting(bool _t){
        targeting = _t;
    }
    
    void target(float t) {
        targeting = true;
        targetVal = t;
    }
};
#endif