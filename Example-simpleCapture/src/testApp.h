#pragma once

#include "ofMain.h"
#include "ofxUeye.h"

class testApp : public ofBaseApp{

public:
	void setup();
	void update();
	void draw();

	void keyPressed  (int key);

	void updateGainAndExposure();

	ofxUeye camera;
	float maxFps; ///<returned from camera.setOptimalCameraTiming()
	bool scaleToWindow;
	bool enableDraw;

	void nudgeGain(float amount);
	void nudgeExposure(float amount);
	float gain;
	float exposure;
	bool gainAuto;
	bool exposureAuto;
};
