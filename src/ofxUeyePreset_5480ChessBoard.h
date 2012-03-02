#pragma once
#include "ofxUEye.h"

class ofxUeyePreset_5480Chessboard : public ofxUeyePreset {
public:
	void apply(ofxUeye& camera) const {
		camera.setPixelClock(60);
		camera.setExposure(40.0f);
		camera.setGain(0.0f);
	}
};