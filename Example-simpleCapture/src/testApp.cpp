#include "testApp.h"

//--------------------------------------------------------------
void testApp::setup(){
	
	ofSetLogLevel(OF_LOG_NOTICE);
	maxFps = 0.0f;
	scaleToWindow = true;
	enableDraw = true;

	////
	//NOTICE!
	////
	//For each section (1,2..)
	//	select only one option (a or b or ..).
	
	//Here we perform all options to show all possibilities
	//In your own app, choose one of each as appropriate for your app

	//
	////


	////
	//1. list cameras
	////
	//

	//a. print out the list of cameras
	//ofxUeye::listDevices();

	//b. get the list of cameras;
	//vector<ofxUeyeDevice> deviceList = ofxUeye::getDevices(); 

	//c. print out the camera details, and get a count of the cameras
	int cameraCount = ofxUeye::listDevices().size();

	//d. print out the camera details, and get a list of the cameras
	//vector<ofxUeyeDevice> deviceList2 = ofxUeye::listDevices();

	//
	////


	if (cameraCount==0) {
		ofLogError() << "No cameras detected!";
		return;
	}
	

	////
	//2. setup the camera
	////
	//

	//a. use first camera
	//camera.init(640, 480);

	//b. use specfic camera ID. uEye ID's start at 1
	camera.init(1);

	//c. use a specific device from the camera list
	//camera.init(deviceList[0]);

	//
	////

	////
	//3. report sensor data
	////
	//
	cout << camera.getSensor().toString();
	//
	////

	camera.setExposure(70);
	this->updateGainAndExposure();
}

//--------------------------------------------------------------
void testApp::update(){
	//update blocks the current thread whilst the capture is being performed
	//best to double buffer it against an image processing thread
	camera.update();

	if (ofGetFrameNum() % 100 == 0)
		this->updateGainAndExposure();
}

//--------------------------------------------------------------

void drawString(string text, int& y) {
	ofPushStyle();
	ofSetColor(200, 100, 100);
	ofRect(10, y-15, text.length() * 8 + 10, 20);
	ofSetColor(255);
	ofDrawBitmapString(text, 15, y);
	ofPopStyle();
	y+=20;
}

void testApp::draw(){
	if (scaleToWindow)
		camera.draw(0, 0, ofGetWidth(), ofGetHeight());
	else
		camera.draw(0, 0);

	int y = 20; //increments automatically
	drawString(string("app fps = ") + ofToString(ofGetFrameRate(), 1), y);
	drawString(string("max camera fps = ") + ofToString(maxFps, 1), y);
	drawString("resolution = " + ofToString(camera.getWidth(),0) + "x" + ofToString(camera.getHeight(),0), y);
	drawString("", y);
	drawString("[f] = fullscreen", y);
	drawString("[o] = optimise pixel clock (takes 4 seconds, or more if there's a problem)", y);
	drawString("[s] = toggle scale to window " + string(scaleToWindow ? "[x]" : "[ ]"), y);
	drawString("[d] = toggle draw " + string(enableDraw ? "[x]" : "[ ]"), y);
	drawString("[9] = set pixel clock speed to 96MHz", y);	
	drawString("", y);
	drawString("[m] = reinitialise in mono mode", y);	
	drawString("[b] = reinitialise in bayer mode (colour cameras only)", y);	
	drawString("[c] = reinitialise in colour mode (colour cameras only)", y);	
	drawString("", y);
	drawString("[UP] / [DOWN] = change exposure (" + ofToString(this->exposure) + ", auto=" + (this->exposureAuto ? "true" : "false") + ")", y);
	drawString("[LEFT] / [RIGHT] = change gain (" + ofToString(this->gain) + ", auto=" + (this->gainAuto ? "true" : "false") + ")", y);

}

//--------------------------------------------------------------
void testApp::keyPressed(int key){
	if (key == 'f')
		ofToggleFullscreen();
	if (key == 'o') 
		maxFps = camera.setOptimalCameraTiming();
	if (key == 's')
		scaleToWindow ^= true;
	if (key == 'd') {
		enableDraw ^= true;
		if (enableDraw)
			camera.setUseTexture(true);
		else
			camera.setUseTexture(false);
	}
	if (key == '9')
		camera.setPixelClock(96);

	if (key == 'm')
		camera.init(1, IS_SET_CM_Y8);
	if (key == 'b')
		camera.init(1, IS_SET_CM_BAYER);
	if (key == 'c')
		camera.init(1, IS_SET_CM_RGB8);

	if (key == OF_KEY_LEFT)
		nudgeGain(-10);
	if (key == OF_KEY_RIGHT)
		nudgeGain(+10);

	if (key == OF_KEY_UP)
		nudgeExposure(+5);
	if (key == OF_KEY_DOWN)
		nudgeExposure(-5);
}

//---------
void testApp::updateGainAndExposure() {
	this->gain = camera.getGain();
	this->exposure = camera.getExposure();
	this->gainAuto = camera.getAutoGain();
	this->exposure = camera.getAutoExposure();
}

//---------
void testApp::nudgeExposure(float amount) {
	camera.setExposure(exposure + amount);
	updateGainAndExposure();
}

//---------
void testApp::nudgeGain(float amount) {
	camera.setGain(gain + amount);
	this->updateGainAndExposure();
}