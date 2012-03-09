#pragma once
#include "ofxUEye.h"

class ofxUeyePreset_5480Chessboard : public ofxUeyePreset {
public:
	void apply(ofxUeye& camera) const {
		camera.setPixelClock(96);
		//camera.setExposure(1000.0f / 60.0f * 8.0f); // korean mains is 60Hz, 220V
		camera.setExposure(0.0f); //sets to highest for this framerate
		camera.setGain(0.5f);
	}
};