//
//  ofxUeye.cpp
//  ofxUeye
//
//  Elliot Woods
//	http://www.kimchiandchips.com
//
#include "ofxUeye.h"

//---------------------------------------------------------------
//ofxUeyeDevice
//---------------------------------------------------------------
//
#pragma mark ofxUeyeDevice

ofxUeyeDevice::ofxUeyeDevice() {
}

ofxUeyeDevice::ofxUeyeDevice(const UEYE_CAMERA_INFO& info) {
	this->cameraID = info.dwCameraID;
	this->deviceID = info.dwDeviceID;
	this->sensorID = info.dwSensorID;
	this->isUse = info.dwInUse == (DWORD)1;
	this->serial = string(info.SerNo);
	this->model = string(info.Model);
	this->status = info.dwStatus; 
}

string ofxUeyeDevice::toString() const {
	stringstream output;
	output << this->cameraID << "\t";
	output << this->deviceID << "\t";
	output << this->sensorID << "\t";
	output << (this->isUse ? "active" : "inactive") << "\t";
	output << this->serial << "\t";
	output << this->model << "\t";
	output << this->status;
	return output.str();
}
//
//---------------------------------------------------------------


//---------------------------------------------------------------
//ofxUeyeSensor
//---------------------------------------------------------------
//
ofxUeyeSensor::ofxUeyeSensor(const SENSORINFO& info) {
	this->sensor = info.SensorID;
	this->model = string(info.strSensorName);
	
	switch (info.nColorMode) {
	case IS_COLORMODE_BAYER:
		this->color = Bayer;
		break;
	case IS_COLORMODE_MONOCHROME:
		this->color = Monochrome;
		break;
	case IS_COLORMODE_CBYCRY:
		this->color = CBYCRY;
		break;
	}

	this->width = info.nMaxWidth;
	this->height = info.nMaxHeight;
	this->masterGain = info.bMasterGain;
	this->redGain = info.bRGain;
	this->greenGain = info.bGGain;
	this->blueGain = info.bBGain;
	this->globalShutter = info.bGlobShutter;
	this->pixelSize = (float)info.wPixelSize * 1e-8;
}

ofxUeyeSensor::ofxUeyeSensor() {
	this->sensor = NULL;
	this->width = 0;
	this->height = 0;
}

string ofxUeyeSensor::toString() const {
	stringstream output;
	output << "sensor:\t\t\t\t" << this->sensor << endl;
	output << "model:\t\t\t\t" << this->model << endl;
	output << "color mode:\t\t\t" << this->getColorMode() << endl;
	output << "width:\t\t\t\t" << this->width << endl;
	output << "height:\t\t\t\t" << this->height << endl;
	output << "supports master gain:\t\t" << this->masterGain << endl;
	output << "supports red gain:\t\t" << this->redGain << endl;
	output << "supports green gain:\t\t" << this->greenGain << endl;
	output << "supports blue gain:\t\t" << this->blueGain << endl;
	output << "supports global shutter:\t" << this->globalShutter<< endl;
	return output.str();
}

string ofxUeyeSensor::getColorMode() const {
	switch (this->color) {
	case Bayer:
		return "Bayer";
	case Monochrome:
		return "Monochrome";
	case CBYCRY:
		return "CBYCRY";
	default:
		return "Error";
	}
}
//
//---------------------------------------------------------------

//---------------------------------------------------------------
//ofxUeye
//---------------------------------------------------------------
//
#pragma mark ofxUeye

////
//constructor
ofxUeye::ofxUeye() {
	this->cameraID = 0;
	this->useTexture = true;
	this->color = false;
	this->open = false;
}

ofxUeye::~ofxUeye() {
	this->close();
}

////
//list devices

vector<ofxUeyeDevice> ofxUeye::listDevices() {
	const vector<ofxUeyeDevice> devices = ofxUeye::getDeviceList();

	cout << "////" << endl;
	cout << "//ofxUeye::listDevices (" << devices.size() << " found)" << endl;
	cout << "////" << endl;
	cout << "//" << endl;
	for (int i = 0; i < devices.size(); i++)
		cout << devices[i].toString() << endl;
	cout << "//" << endl;
	cout << "////" << endl;
	cout << endl;

	return devices;
}

vector<ofxUeyeDevice> ofxUeye::getDeviceList() {
	vector<ofxUeyeDevice> devices;
	int cameraCount;
	UEYE_CAMERA_LIST* list = 0;
	try {
		if (is_GetNumberOfCameras(&cameraCount) != IS_SUCCESS)
			throw;

		list = (UEYE_CAMERA_LIST*) malloc(sizeof(DWORD) + cameraCount * sizeof(UEYE_CAMERA_INFO));
		list->dwCount = cameraCount;

		if (is_GetCameraList(list) != IS_SUCCESS)
			throw;

		for (int i=0; i<cameraCount; i++) {
			UEYE_CAMERA_INFO& info(list->uci[i]);
			devices.push_back(ofxUeyeDevice(list->uci[i]));
		}

	} catch (...) {
		ofLogError() << "ofxUeye::getDevies() error whilst enumerating cameras.";
		return devices;
	}

	if (list != 0)
		free(list);
	return devices;
}

////
//open/close device
bool ofxUeye::init(int cameraOrDeviceID, bool useCameraID, int colorMode) {
	close();
	HIDS hCam = useCameraID ? cameraOrDeviceID : MAX(cameraOrDeviceID, 0) + 1001 | IS_USE_DEVICE_ID;
	ofLogNotice() << "ofxUeye::init Opening camera with " << (useCameraID ? "camera ID " : "device ID ") << cameraOrDeviceID;

	int nRet = is_InitCamera(&hCam, NULL);

	////
	//Upload firmware if neccesary
	////
	//
	if (nRet == IS_STARTER_FW_UPLOAD_NEEDED) {
		int timeNeeded;
		is_GetDuration(hCam, IS_SE_STARTER_FW_UPLOAD, &timeNeeded);

		ofLogNotice() << "ofxUeye::init : Updating camera firmware, wait " << timeNeeded << " seconds.";
		int upload = (HIDS) (((INT)hCam) | IS_ALLOW_STARTER_FW_UPLOAD); 
		nRet = is_InitCamera(&hCam, NULL);		
	}
	//
	////

	if (nRet != IS_SUCCESS) {
		ofLogError() << "ofxUeye::init(" << cameraOrDeviceID <<") failed to init camera";
		this->open = false;
		return false;
	}

	this->hCam = hCam;
	
	if (!useCameraID) {
		CAMINFO camera;
		is_GetCameraInfo(hCam, &camera);
		this->cameraID = (int)camera.Select;
		this->deviceID = cameraOrDeviceID;
	} else {
		this->cameraID = cameraOrDeviceID;
		this->deviceID = -1;
	}

	SENSORINFO sensor;
	is_GetSensorInfo(hCam, &sensor);
	this->sensor = ofxUeyeSensor(sensor);
	
	is_SetColorMode(hCam, colorMode);
	if (colorMode == IS_SET_CM_RGB8)
		this->color = true;
	else
		this->color = false;
	allocate();

	is_SetAllocatedImageMem(hCam, this->sensor.width, this->sensor.height, this->color ? 24 : 8, (char*) this->pixels.getPixels(), &dataID);
	is_SetImageMem(hCam, (char*) this->pixels.getPixels(), dataID);
	ofLogNotice() << "ofxUeye::init : Camera " << cameraOrDeviceID << " initialised successfully.";

	this->open = true;

	return true;
}

bool ofxUeye::init(const ofxUeyeDevice& device, int colorMode) {
	return init(device.cameraID, true, colorMode);
}

void ofxUeye::close() {
	if (!this->isOpen())
		return;
	ofLogNotice() << "ofxUeye::close : closing camera " << hCam;
	is_ExitCamera(hCam);
	this->open = false;
}

bool ofxUeye::isOpen() const {
	return this->open;
}

bool ofxUeye::startFreeRunCapture() {
	if (!this->isOpen()) {
		ofLogError() << "ofxUeye::startFreeRunCapture : cannot begin freerun capture as no device is intialised";
		return false;
	}

	return is_CaptureVideo(hCam, IS_DONT_WAIT) == IS_SUCCESS;
}

void ofxUeye::stopFreeRunCapture() {
	if (!this->isOpen()) {
		ofLogError() << "ofxUeye::stopFreeRunCapture : cannot stop freerun as no camera is initialised";
		return;
	}
}

////
//identity
int ofxUeye::getCameraID() const {
	return this->cameraID;
}

int ofxUeye::getDeviceID() const {
	if (this->deviceID == -1)
		ofLogWarning("ofxUeye") << "Camera was not initialised with device ID, so device ID is not known";
	return this->deviceID;
}

////
//sensor
const ofxUeyeSensor& ofxUeye::getSensor() const {
	return this->sensor;
}

int ofxUeye::getSensorWidth() const {
	return this->getSensor().width;
}

int ofxUeye::getSensorHeight() const {
	return this->getSensor().height;
}

////
//set properties
float ofxUeye::setOptimalCameraTiming() {
	if (!this->isOpen()) {
		ofLogError() << "ofxUeye::setOptimalCameraTiming : no device intialised, call ofxUeye::init first please";
		return 0.0f;
	}

	double fps;
	int error = is_SetOptimalCameraTiming(this->hCam, IS_BEST_PCLK_RUN_ONCE, 4000, &maxClock, &fps);

	switch (error) {
	case IS_SUCCESS:
		return fps;
	case IS_NO_SUCCESS:
		ofLogError() << "ofxUeye::setOptimalCameraTiming failed";
		return 0;
	case IS_AUTO_EXPOSURE_RUNNING:
		ofLogError() << "ofxUeye::setOptimalCameraTiming cannot be performed whilst auto exposure is running";
		return 0;
	case IS_INVALID_PARAMETER:
		ofLogError() << "ofxUeye::setOptimalCameraTiming timeout parameter is invalid";
		return 0;
	case IS_TRIGGER_ACTIVATED:
		ofLogError() << "ofxUeye::setOptimalCameraTiming cannot be performed in trigger mode";
		return 0;
	}
	return fps;
}

void ofxUeye::setPixelClock(int speedMHz) {
	if (!this->isOpen()) {
		ofLogError() << "ofxUeye::setPixelClock : no device intialised, call ofxUeye::init first please";
		return;
	}

	int error = is_SetPixelClock(this->hCam, speedMHz);
	
	switch (error) {
	case IS_NO_SUCCESS:
		ofLogError() << "ofxUeye::setPixelClock failed";
	case IS_INVALID_MODE:
		ofLogError() << "ofxUeye::setPixelClock cannot be performed whilst in standby mode";
	case IS_INVALID_PARAMETER:
		ofLogError() << "ofxUeye::setPixelClock clock speed (" << speedMHz << ") is outside supported range for this camera";
	}
	return;
}

int ofxUeye::getPixelClock() const {
	if (!this->isOpen()) {
		ofLogError("ofxUeye") << "getPixelClock: no device intialised, call ofxUeye::init first please";
		return 0;
	}

	return is_SetPixelClock(this->hCam, IS_GET_PIXEL_CLOCK);
}

void ofxUeye::setGain(float gain) {
	if (!this->isOpen()) {
		ofLogError() << "ofxUeye::setGain: no device intialised, call ofxUeye::init first please";
		return;
	}

	int parameter = gain * 100.0f;
	is_SetHWGainFactor(this->hCam, gain, parameter);
}

void ofxUeye::setExposure(float exposure) {
	if (!this->isOpen()) {
		ofLogError() << "ofxUeye::setExposure: no device intialised, call ofxUeye::init first please";
		return;
	}

	//just to note. the old 'obsolete' IDS function was sensible
	//this one is simply stupid. IDS have fucking idiots in their SDK team
	
	//first we check whether fine shutter is supported
	double parameter = exposure;
	is_Exposure(this->hCam, IS_EXPOSURE_CMD_SET_EXPOSURE, &parameter, sizeof(parameter));
}

void ofxUeye::setHWGamma(bool enabled) {
	if (!this->isOpen()) {
		ofLogError() << "ofxUeye::setHWGamma: no device intialised, call ofxUeye::init first please";
		return;
	}

	is_SetHardwareGamma(this->hCam, enabled ? IS_SET_HW_GAMMA_ON : IS_SET_HW_GAMMA_OFF);
}

void ofxUeye::setGamma(float gamma) {
	if (!this->isOpen()) {
		ofLogError() << "ofxUeye::setGamma: no device intialised, call ofxUeye::init first please";
		return;
	}

	is_SetGamma(this->hCam, gamma * 100.0f);
}
////
//capture
bool ofxUeye::capture() {
	if (!isOpen()) {
		ofLogError() << "ofxUeye::capture : cannot capture as device is not open";
		return false;
	}

	int error = is_FreezeVideo(hCam, IS_WAIT);
	switch(error) {
	case IS_NO_SUCCESS:
		ofLogError("ofxUeye") << "capture on camera ID " << cameraID << " failed";
		break;
	case IS_TRANSFER_ERROR:
		ofLogWarning("ofxUeye") << "transfer error on capture on camera ID " << cameraID;
		break;
	case IS_CAPTURE_RUNNING:
		ofLogWarning("ofxUeye") << "cannot capture on camera ID " << cameraID << " as another capture is already in progress";
		break;
	}
	if (this->useTexture)
		texture.loadData(pixels);

	return error == IS_SUCCESS;
}

////
//ofBaseVideoDraws
////
//
void ofxUeye::draw(float x, float y) {
	this->draw(x, y, this->getWidth(), this->getHeight());
}

void ofxUeye::draw(float x, float y, float w, float h) {
	if (!useTexture)
		return;

	if (texture.isAllocated())
		texture.draw(x, y, w, h);
}

float ofxUeye::getWidth() {
	return pixels.getWidth();
}

float ofxUeye::getHeight() {
	return pixels.getHeight();
}

void ofxUeye::update() {
	this->capture();
}

unsigned char* ofxUeye::getPixels() {
	return this->pixels.getPixels();
}

ofPixels& ofxUeye::getPixelsRef() {
	return this->pixels;
}

bool ofxUeye::isFrameNew() {
	ofLogError() << "ofxUeye::isFrameNew not supported";
	return true;
}

ofTexture& ofxUeye::getTextureReference() {
	return this->texture;
}

void ofxUeye::setUseTexture(bool useTexture) {
	this->useTexture = useTexture;

	if (useTexture) {
		if (this->isOpen())
			this->allocate();
	} else {
		this->texture.clear();
	}
}
//
////

////
//protected
////
//
void ofxUeye::allocate() {
	int width = this->sensor.width;
	int height = this->sensor.height;

	this->pixels.allocate(width, height, this->color ? OF_PIXELS_RGB : OF_PIXELS_MONO);
	if (useTexture) {
		this->texture.allocate(width, height, this->color ? GL_RGB : GL_LUMINANCE);
		this->texture.setTextureMinMagFilter(GL_NEAREST, GL_NEAREST);
	}
}