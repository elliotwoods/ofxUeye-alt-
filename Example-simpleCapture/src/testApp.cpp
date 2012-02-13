#include "testApp.h"

//--------------------------------------------------------------
void testApp::setup(){
	
	ofSetLogLevel(OF_LOG_NOTICE);

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
	camera.init(640, 480, 1);

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
}

//--------------------------------------------------------------
void testApp::draw(){
	camera.draw(0, 0);
}

//--------------------------------------------------------------
void testApp::keyPressed(int key){
	camera.update();

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