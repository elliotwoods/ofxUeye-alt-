#pragma once
#include "ofxUEye.h"

class ofxUeyePreset_5480SL : public ofxUeyePreset {
public:
	void apply(ofxUeye& camera) const {
		camera.setPixelClock(96);
		
		//presume 120Hz, use 4 frames (30Hz)
		camera.setExposure(1000.0f / 120.0f * 4.0f);
		camera.setGain(0.0f);
		camera.setHWGamma(false);
		camera.setGamma(1.0f);
		camera.setRotation(-1);
	}
};