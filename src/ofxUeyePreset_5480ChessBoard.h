#pragma once
#include "ofxUEye.h"

class ofxUeyePreset_5480Chessboard : public ofxUeyePreset {
public:
	void apply(ofxUeye& camera) const {
		camera.setPixelClock(60);
		camera.setExposure(16.666f * 2.0f); // korean mains is 60Hz, 220V
		camera.setGain(0.0f);
	}
};