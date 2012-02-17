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
	camera.init(1, IS_SET_CM_BAYER);

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
}

//--------------------------------------------------------------
void testApp::update(){
	//update blocks the current thread whilst the capture is being performed
	//best to double buffer it against an image processing thread
	camera.update();
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
	drawString("[o] = optimise pixel clock (takes 4 seconds)", y);
	drawString("[s] = toggle scale to window " + string(scaleToWindow ? "[x]" : "[ ]"), y);
	drawString("[d] = toggle draw " + string(enableDraw ? "[x]" : "[ ]"), y);
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
}

//--------------------------------------------------------------
void testApp::keyReleased(int key){

}

//--------------------------------------------------------------
void testApp::mouseMoved(int x, int y ){

}

//--------------------------------------------------------------
void testApp::mouseDragged(int x, int y, int button){

}

//--------------------------------------------------------------
void testApp::mousePressed(int x, int y, int button){

}

//--------------------------------------------------------------
void testApp::mouseReleased(int x, int y, int button){

}

//--------------------------------------------------------------
void testApp::windowResized(int w, int h){

}

//--------------------------------------------------------------
void testApp::gotMessage(ofMessage msg){

}

//--------------------------------------------------------------
void testApp::dragEvent(ofDragInfo dragInfo){ 

}