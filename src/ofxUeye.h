//
//  ofxUeye.h
//  ofxUeye
//
//  Elliot Woods
//	http://www.kimchiandchips.com
//
#pragma once
#include "ofMain.h"
#include "uEye.h"

struct ofxUeyeDevice {
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

class ofxUeye : public ofBaseVideoDraws {
public:
	ofxUeye();
	~ofxUeye();
	static vector<ofxUeyeDevice> listDevices(); ///<list to console
	static vector<ofxUeyeDevice> getDevices(); ///<list to vector

	bool init(int deviceID=0, int colorMode=IS_SET_CM_Y8);
	bool init(const ofxUeyeDevice& device);
	bool initGrabber(int width, int height, int deviceID=0); ///<init device and open
	void close();
	bool isOpen() const;
	bool startFreeRunCapture();
	void stopFreeRunCapture();

	const ofxUeyeSensor& getSensor() const;
	int getSensorWidth() const;
	int getSensorHeight() const;

	bool open(int width, int height);
	float setOptimalCameraTiming(); ///<returns fps
	void setPixelClock(int speedMHz);

	void capture();

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
	HIDS cameraID;
	char* data;
	int dataID;
	ofxUeyeSensor sensor;

	void allocate();
	ofPixels pixels;
	bool useTexture;
	bool color;
	ofTexture texture;

	int maxClock;
};