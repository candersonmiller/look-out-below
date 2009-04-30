/*
 Look out below
 Spatial Media Final Project
 Spring 2009
 
 C. Anderson Miller
 Robert Carlsen
 
 Rooftop projector installation
 Take on the role of a bird and aim droppings at passersby on the below sidewalk
 
*/

// TODO: indicate that there are birds here?
    // use a flocking system of boids which are disturbed by movement (walkers)?
    // draw a label around hits or misses?


#ifndef _TEST_APP
#define _TEST_APP

#include "ofMain.h"

#include "ofxOpenCv.h"
#include "Target.h"
#include "Dropping.h"
#include "HitArea.h"

//#define _USE_LIVE_VIDEO		// uncomment this to use a live camera
								// otherwise, we'll use a movie file

#define kTargetStepIncrement 5
#define kTargetStepMax 50
#define kTargetAccelerationDelay 200

class testApp : public ofBaseApp{
public:

    void setup();
    void update();
    void draw();

    void keyPressed  (int key);
    void keyReleased(int key);
    void mouseMoved(int x, int y );
    void mouseDragged(int x, int y, int button);
    void mousePressed(int x, int y, int button);
    void mouseReleased(int x, int y, int button);
    void windowResized(int w, int h);

    virtual bool hitTest(int _x, int _y);
    
    #ifdef _USE_LIVE_VIDEO
      ofVideoGrabber 		vidGrabber;
    #else
      ofVideoPlayer 		vidPlayer;
    #endif

    ofxCvColorImage		colorImg;

    ofxCvGrayscaleImage 	grayImage;
    ofxCvGrayscaleImage 	grayBg;
    ofxCvGrayscaleImage 	grayDiff;

    ofxCvContourFinder 	contourFinder;

    int 				threshold;
    bool				bLearnBakground;

    Target  *target;
    vector<Dropping*>droppings;
    vector<HitArea*>hits;
    
    ofTrueTypeFont font1;
    bool debugVision;
    bool debugVisionFullscreen;
    bool displayVideoFullscreen;
    
    bool keyFirstPress;
    long keyPressedTimer; // to enable acceleration of the target movement with a joystick
    
    // for warping the image for correct distortion
    void drawMatrix();
    void calculateMatrix();
    void resetCorners();
    ofPoint corners[4];
    int whichCorner;
    
    Boolean isWarpingSetupMode;
    
    int vidWidth;
    int vidHeight;
};

#endif
