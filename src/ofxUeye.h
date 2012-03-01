//
//  ofxUeye.h
//  ofxUeye
//
//  Elliot Woods
//	http://www.kimchiandchips.com
//
#pragma once
#include "ofTexture.h"
#include "ofPixels.h"
#include "ofAppRunner.h"

#include "uEye.h"

#include <string>

using namespace std;

struct ofxUeyeDevice {
	ofxUeyeDevice();
	ofxUeyeDevice(const UEYE_CAMERA_INFO& info);
	string toString() const;

	DWORD cameraID;
	DWORD deviceID;
	DWORD sensorID;
	bool isUse;
	string serial;
	string model;
	DWORD status;
};

struct ofxUeyeSensor {
	typedef enum { Bayer, Monochrome, CBYCRY } colorMode;
	ofxUeyeSensor();
	ofxUeyeSensor(const SENSORINFO& info);
	string toString() const;
	string getColorMode() const;

	WORD sensor;
	string model;
	colorMode color;
	int width;
	int height;
	bool masterGain;
	bool redGain;
	bool greenGain;
	bool blueGain;
	bool globalShutter;
	float pixelSize; ///< size of pixel in meters
};

class ofxUeye;
class ofxUeyePreset {
public:
	virtual void apply(ofxUeye& camera) const = 0;
};

class ofxUeye : public ofBaseVideoDraws {
public:
	ofxUeye();
	~ofxUeye();
	static vector<ofxUeyeDevice> listDevices(); ///<list to console
	static vector<ofxUeyeDevice> getDeviceList(); ///<list to vector

	bool init(int cameraOrDeviceID=0, bool useCameraID=false, int colorMode=IS_SET_CM_Y8);
	bool init(const ofxUeyeDevice& device);
	void close();
	bool isOpen() const;
	bool startFreeRunCapture();
	void stopFreeRunCapture();

	int getCameraID() const;
	int getDeviceID() const;

	const ofxUeyeSensor& getSensor() const;
	int getSensorWidth() const;
	int getSensorHeight() const;

	float setOptimalCameraTiming(); ///<returns fps
	void setPixelClock(int speedMHz);
	int getPixelClock() const;

	void setGain(float gain); //set gain as a percentage
	void setExposure(float exposure); //presume in ms
	void setHWGamma(bool enabled);
	void setGamma(float gamma);

	bool capture();

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
	//ofBaseHasPixels
	unsigned char* getPixels();
	ofPixels& getPixelsRef();
	//ofBaseVideo
	bool isFrameNew(); //+close() above
	//ofBaseHasTexture
	ofTexture& getTextureReference();
	void setUseTexture(bool useTexture);
	//
	////

protected:
	HIDS hCam;
	int deviceID;
	int cameraID;
	int dataID; ///<ID of memory area for captures
	bool open;
	ofxUeyeSensor sensor;

	void allocate();
	ofPixels pixels;
	bool useTexture;
	bool color;
	ofTexture texture;

	int maxClock;
};