#pragma once
#include "ofxUeye.h"
#include "ofThread.h"

class ofxUeyeThreaded : public ofThread, public ofBaseUpdates, public ofBaseDraws, public ofBaseHasPixels, public ofBaseHasTexture {
public:
	ofxUeyeThreaded();
	~ofxUeyeThreaded();
	bool init(int deviceOrCameraID=0, bool useCameraID=false, int colorMode=IS_SET_CM_Y8);
	bool init(const ofxUeyeDevice& device);

	void close();
	void copyPixelsTo(ofPixels& pixels);
	
	ofxUeye& getCamera();
	const ofxUeye& getCamera() const;
	void apply(const ofxUeyePreset& preset);
	void getFreshFrame(); ///<for when you absolutely need a new frame, e.g. structured light
	ofPixels getFreshFrameCopy(); ///<capture the fresh frame and return a copy

	void setThreadPaused(bool paused);

	////
	//ofBaseVideoDraws
	////
	//
	//ofBaseDraws
	void draw(float x,float y);
	void draw(float x,float y,float w, float h);
	float getHeight();
	float getWidth();
	//ofBaseUpdates
	void update();
	void capture(); //capture without tex update
	//ofBaseHasPixels
	unsigned char* getPixels();
	ofPixels& getPixelsRef();
	ofPixels getPixelsCopy(); //added for threaded use
	//ofBaseVideo
	bool isFrameNew(); //+close() above
	//ofBaseHasTexture
	ofTexture& getTextureReference();
	void setUseTexture(bool useTexture);
	//
	///


protected:
	void initBase();
	void threadedFunction();

	ofPixels pixels;
	ofxUeye camera;

	bool useTexture;
	ofTexture texture;

	float width, height;
	bool frameNew;
	bool frameNewAtUpdate;

	bool threadPaused;
};