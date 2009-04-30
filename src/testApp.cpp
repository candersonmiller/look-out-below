#include "testApp.h"


//--------------------------------------------------------------
void testApp::setup(){
    ofSetFrameRate(60);
    ofSetVerticalSync(true);
    
	#ifdef _USE_LIVE_VIDEO
        vidGrabber.setVerbose(true);
        vidGrabber.initGrabber(320,240);
	#else
        vidPlayer.loadMovie("fingers.mov");
        vidPlayer.play();
	#endif

    colorImg.allocate(320,240);
	grayImage.allocate(320,240);
	grayBg.allocate(320,240);
	grayDiff.allocate(320,240);

	bLearnBakground = true;
	threshold = 80;
    
    target = new Target(0,0);
    debugVision = 0;
    debugVisionFullscreen = 0;
    font1.loadFont("type/frabk.ttf",18);
    
    ofEnableAlphaBlending();
    
    isWarpingSetupMode = false;
    
    //this is just for our gui / mouse handles
	//this will end up being the destination quad we are warping too
	corners[0].x = 0.0;
	corners[0].y = 0.0;
    
	corners[1].x = 1.0;
	corners[1].y = 0.0;
    
	corners[2].x = 1.0;
	corners[2].y = 1.0;
	
	corners[3].x = 0.0;
	corners[3].y = 1.0;
}

//--------------------------------------------------------------
void testApp::update(){
	ofBackground(0,0,0);
    
    // update the list of droppings:
    for(int i=0;i<droppings.size();i++){
        droppings[i]->update();
        
        if(droppings[i]->onGround && droppings[i]->tagged==false){
            droppings[i]->tagged = true;
            droppings[i]->setHit(hitTest(droppings[i]->x, droppings[i]->y));
        }
        
        if(droppings[i]->discard){
            droppings.erase(droppings.begin() + i);
            // sanity check:
            if(i>= droppings.size())
                break;
        }
    }
    
    //---//
    
    bool bNewFrame = false;

	#ifdef _USE_LIVE_VIDEO
       vidGrabber.grabFrame();
	   bNewFrame = vidGrabber.isFrameNew();
    #else
        vidPlayer.idleMovie();
        bNewFrame = vidPlayer.isFrameNew();
	#endif

	if (bNewFrame){

		#ifdef _USE_LIVE_VIDEO
            colorImg.setFromPixels(vidGrabber.getPixels(), 320,240);
	    #else
            colorImg.setFromPixels(vidPlayer.getPixels(), 320,240);
        #endif

        grayImage = colorImg;
		if (bLearnBakground == true){
			grayBg = grayImage;
			bLearnBakground = false;
		}

		// take the abs value of the difference between 
        // background and incoming and then threshold:
		grayDiff.absDiff(grayBg, grayImage);
		grayDiff.threshold(threshold);

		// find contours which are between the size of 20 pixels and 1/3 the w*h pixels.
		// also, find holes is set to true so we will get interior contours as well....
		contourFinder.findContours(grayDiff, 20, (340*240)/3, 10, true);	// find holes
	}

//	printf("%f \n", ofGetFrameRate());
}

//--------------------------------------------------------------
void testApp::draw(){
    // does all the warping magic:
    calculateMatrix();
    
    ofSetRectMode(OF_RECTMODE_CORNER);
    
	// draw the incoming, the grayscale, the bg and the thresholded difference
	if(debugVisionFullscreen){
        ofSetColor(0xffffff);
        grayDiff.draw(0,0,ofGetWidth(),ofGetHeight());
    }
    
	if(debugVision){
		ofSetColor(0xffffff);
		colorImg.draw(20,20);
		grayImage.draw(360,20);
		grayBg.draw(20,280);
		grayDiff.draw(360,280);

		// then draw the contours:

		ofFill();
		ofSetColor(0x333333);
		ofRect(360,540,320,240);
		ofSetColor(0xffffff);

		// we could draw the whole contour finder
		//contourFinder.draw(360,540);

		// or, instead we can draw each blob individually,
		// this is how to get access to them:
		for (int i = 0; i < contourFinder.nBlobs; i++){
			contourFinder.blobs[i].draw(360,540);
		}

		// finally, a report:

		ofSetColor(0xffffff);
		char reportStr[1024];
		sprintf(reportStr, "bg subtraction and blob detection\npress ' ' to capture bg\nthreshold %i (press: +/-)\nnum blobs found %i", threshold, contourFinder.nBlobs);
		ofDrawBitmapString(reportStr, 20, 600);
	}
    //---//
    
    // Draw the crosshair on top:
    target->display();
    
    // and the droppings on top of that:
    for(int i=0;i<droppings.size();i++){
        droppings[i]->display();
    }
    
    // display the camera warping for setup/debugging:
    if(isWarpingSetupMode==true){
        drawMatrix();
    }
}

bool testApp::hitTest(int _x, int _y){
    // scan the difference image and determine if we've hit or missed objects
    // called grayDiff
    ofSetColor(0xffffff);
    
    float sourceXf = _x / (float)ofGetWidth();
    float sourceYf = _y / (float)ofGetHeight();

    int locTarget = (sourceXf * 320) + (sourceYf * 240) * 320;
    unsigned char* grayDiffPixels = grayDiff.getPixels();
    
    // looking for a white pixel to indicate a foreground object.
    if(grayDiffPixels[locTarget] > 250)
        return true;
    else
        return false;
}


//--------------------------------------------------------------
void testApp::keyReleased(int key){
    // reset the flag when the key has been released
    keyFirstPress = false;
}

void testApp::keyPressed  (int key){
    int targetStepIncrement = kTargetStepIncrement;
    if(!keyFirstPress) {
        // start the timer on the first press:
        keyFirstPress = true;
        keyPressedTimer = ofGetElapsedTimeMillis();
    } else {
        // accelerate the step with duration of the key press:
        float keyPressedDuration = ofGetElapsedTimeMillis() - keyPressedTimer;
        targetStepIncrement = (int)kTargetStepIncrement + pow(2,keyPressedDuration/kTargetAccelerationDelay);
        // clamp to maximum step value:
        targetStepIncrement = (targetStepIncrement > kTargetStepMax) ? kTargetStepMax : targetStepIncrement;
    }
    
	switch (key){
        case 'r':
            // reset the corners for warping
            resetCorners();
            break;
        case 'w':
            // warping setup mode toggle:
            isWarpingSetupMode = !isWarpingSetupMode;
            break;
        case 'c':
            // clear all the droppings:
            droppings.clear();
            break;
		case 'd':
			if(debugVision) debugVision = false;
			else debugVision = true;
			break;
        case 'D':
			debugVisionFullscreen = !debugVisionFullscreen;
			break;
		case 'b':
			bLearnBakground = true;
			break;
		case '+':
			threshold ++;
			if (threshold > 255) threshold = 255;
			break;
		case '-':
			threshold --;
			if (threshold < 0) threshold = 0;
			break;
            
            // control for the joystick (using a keyboard emulator)
        case OF_KEY_UP:
            int _y = (target->y - targetStepIncrement + ofGetHeight())%ofGetHeight(); // wrap around
            int _x = target->x;
            target->update(_x,_y);
            break;
            
        case OF_KEY_DOWN:
             _y = (target->y + targetStepIncrement)%ofGetHeight(); // wrap around
             _x = target->x;
            target->update(_x,_y);
            break;
            
        case OF_KEY_LEFT:
             _x = (target->x - targetStepIncrement + ofGetWidth())%ofGetWidth(); // wrap around
             _y = target->y;
            target->update(_x,_y);
            break;
            
        case OF_KEY_RIGHT:
             _x = (target->x + targetStepIncrement)%ofGetWidth(); // wrap around
             _y = target->y;
            target->update(_x,_y);
            break;
            
        case ' ':
            _x = target->x;
            _y = target->y;
            droppings.push_back(new Dropping(_x,_y));
            break;

	}
}

//--------------------------------------------------------------
void testApp::mouseMoved(int x, int y ){
    // update the crosshair when the mouse is moved
    // TODO: move this to the keyboard? for the joystick
    target->update(x,y);
}

//--------------------------------------------------------------
void testApp::mouseDragged(int x, int y, int button){
    // adjust the warp:
    if(isWarpingSetupMode){
        float scaleX = (float)x / ofGetWidth();
		float scaleY = (float)y / ofGetHeight();
		
		if(whichCorner >= 0){
			corners[whichCorner].x = scaleX;
			corners[whichCorner].y = scaleY;			
		}
    }
}

//--------------------------------------------------------------
void testApp::mousePressed(int x, int y, int button){
    
    // warping code:
    if(isWarpingSetupMode){
        float smallestDist = 1.0;
        whichCorner = -1;
        
        for(int i = 0; i < 4; i++){
            float distx = corners[i].x - (float)x/ofGetWidth();
            float disty = corners[i].y - (float)y/ofGetHeight();
            float dist  = sqrt( distx * distx + disty * disty);
            
            if(dist < smallestDist && dist < 0.1){
                whichCorner = i;
                smallestDist = dist;
            }
        }
    } else
        droppings.push_back(new Dropping(x,y));
}

//--------------------------------------------------------------
void testApp::mouseReleased(int x, int y, int button){
    // warping code:
    whichCorner = -1;
}

//--------------------------------------------------------------
void testApp::windowResized(int w, int h){

}


// the magic warping code:
void testApp::calculateMatrix(){
    
	//lets make a matrix for openGL
	//this will be the matrix that peforms the transformation
	GLfloat myMatrix[16];
    
	//we set it to the default - 0 translation
	//and 1.0 scale for x y z and w
	for(int i = 0; i < 16; i++){
		if(i % 5 != 0) myMatrix[i] = 0.0;
		else myMatrix[i] = 1.0;
	}
    
	//we need our points as opencv points
	//be nice to do this without opencv?
	CvPoint2D32f cvsrc[4];
	CvPoint2D32f cvdst[4];	
    
	//we set the warp coordinates
	//source coordinates as the dimensions of our window
	cvsrc[0].x = 0;
	cvsrc[0].y = 0;
	cvsrc[1].x = ofGetWidth();
	cvsrc[1].y = 0;
	cvsrc[2].x = ofGetWidth();
	cvsrc[2].y = ofGetHeight();
	cvsrc[3].x = 0;
	cvsrc[3].y = ofGetHeight();			
    
	//corners are in 0.0 - 1.0 range
	//so we scale up so that they are at the window's scale
	for(int i = 0; i < 4; i++){
		cvdst[i].x = corners[i].x  * (float)ofGetWidth();
		cvdst[i].y = corners[i].y * (float) ofGetHeight();
	}
    
	//we create a matrix that will store the results
	//from openCV - this is a 3x3 2D matrix that is
	//row ordered
	CvMat * translate = cvCreateMat(3,3,CV_32FC1);
	
	//this is the slightly easier - but supposidly less
	//accurate warping method 
	//cvWarpPerspectiveQMatrix(cvsrc, cvdst, translate); 
    
    
	//for the more accurate method we need to create
	//a couple of matrixes that just act as containers
	//to store our points  - the nice thing with this 
	//method is you can give it more than four points!
	
	CvMat* src_mat = cvCreateMat( 4, 2, CV_32FC1 );
	CvMat* dst_mat = cvCreateMat( 4, 2, CV_32FC1 );
    
	//copy our points into the matrixes
	cvSetData( src_mat, cvsrc, sizeof(CvPoint2D32f));
	cvSetData( dst_mat, cvdst, sizeof(CvPoint2D32f));
    
	//figure out the warping!
	//warning - older versions of openCV had a bug
	//in this function.
	cvFindHomography(src_mat, dst_mat, translate);
	
	//get the matrix as a list of floats
	float *matrix = translate->data.fl;
    
    
	//we need to copy these values
	//from the 3x3 2D openCV matrix which is row ordered
	//
	// ie:   [0][1][2] x
	//       [3][4][5] y
	//       [6][7][8] w
	
	//to openGL's 4x4 3D column ordered matrix
	//        x  y  z  w   
	// ie:   [0][3][ ][6]
	//       [1][4][ ][7]
	//		 [ ][ ][ ][ ]
	//       [2][5][ ][9]
	//       
    
	myMatrix[0]		= matrix[0];
	myMatrix[4]		= matrix[1];
	myMatrix[12]	= matrix[2];
	
	myMatrix[1]		= matrix[3];
	myMatrix[5]		= matrix[4];
	myMatrix[13]	= matrix[5];	
	
	myMatrix[3]		= matrix[6];
	myMatrix[7]		= matrix[7];
	myMatrix[15]	= matrix[8];	
    
	
	//finally lets multiply our matrix
	//wooooo hoooo!
	glMultMatrixf(myMatrix);
}

void testApp::drawMatrix(){
	//lets draw a bounding box
	ofNoFill();
	ofSetColor(0xFF00FF);
    ofSetRectMode(OF_RECTMODE_CORNER);
	ofRect(1, 1, ofGetWidth()-2, ofGetHeight()-2);
	
	//some text
	ofSetColor(0x000000);
	font1.drawString("grab corners to warp openGL graphics", 28, 33);
    
	ofSetColor(0xFF00FF);
	font1.drawString("grab corners to warp openGL graphics", 30, 30);
}

void testApp::resetCorners(){
    corners[0].x = 0.0;
	corners[0].y = 0.0;
    
	corners[1].x = 1.0;
	corners[1].y = 0.0;
    
	corners[2].x = 1.0;
	corners[2].y = 1.0;
	
	corners[3].x = 0.0;
	corners[3].y = 1.0;
}
