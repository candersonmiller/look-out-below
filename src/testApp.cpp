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
    
    font1.loadFont("type/frabk.ttf",18);
}

//--------------------------------------------------------------
void testApp::update(){
	ofBackground(100,100,100);
    
    // update the list of droppings:
    for(int i=0;i<droppings.size();i++){
        droppings[i]->update();
        
        if(droppings[i]->discard){
            droppings.erase(droppings.begin() + i);
            // sanity check:
            if(i>= droppings.size())
                break;
        }
    }
    
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

    ofSetRectMode(OF_RECTMODE_CORNER);
    
	// draw the incoming, the grayscale, the bg and the thresholded difference
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

    //---//
    
    // Draw the crosshair on top:
    target->display();
    
    // and the droppings on top of that:
    for(int i=0;i<droppings.size();i++){
        droppings[i]->display();
    }
}


//--------------------------------------------------------------
void testApp::keyPressed  (int key){

	switch (key){
		case ' ':
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
}

//--------------------------------------------------------------
void testApp::mousePressed(int x, int y, int button){
    droppings.push_back(new Dropping(x,y));
}

//--------------------------------------------------------------
void testApp::mouseReleased(int x, int y, int button){

}

//--------------------------------------------------------------
void testApp::windowResized(int w, int h){

}

