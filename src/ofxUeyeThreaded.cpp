#include "ofxUeyeThreaded.h"

ofxUeyeThreaded::ofxUeyeThreaded() {
	this->width = 0;
	this->height = 0;
	this->frameNew = false;
	this->useTexture = true;
}

ofxUeyeThreaded::~ofxUeyeThreaded() {
	this->close();
}

bool ofxUeyeThreaded::init(int deviceOrCameraID, bool useCameraID, int colorMode) {
	close();
	if (!camera.init(deviceOrCameraID, useCameraID))
		return false;
	else {
		initBase();
		return true;
	}
}

bool ofxUeyeThreaded::init(const ofxUeyeDevice& device) {
	close();
	if (!camera.init(device))
		return false;
	else
		initBase();
}

void ofxUeyeThreaded::initBase() {
	this->camera.setUseTexture(false);
	
	camera.capture();
	pixels = camera.getPixelsRef();
	texture.allocate(pixels);
	this->width = pixels.getWidth();
	this->height = pixels.getHeight();

	this->startThread(true, false);
}

void ofxUeyeThreaded::close() {
	if (this->camera.isOpen()) {
		if (this->isThreadRunning())
			stopThread(false);
		this->camera.close();
	}
}

void ofxUeyeThreaded::copyPixelsTo(ofPixels& pixels) {
	this->lock();
	pixels = this->pixels;
	this->unlock();
}

void ofxUeyeThreaded::draw(float x, float y) {
	this->texture.draw(x, y);
}

void ofxUeyeThreaded::draw(float x, float y, float width, float height) {
	this->texture.draw(x, y, width, height);
}

float ofxUeyeThreaded::getWidth() {
	return this->width;
}

float ofxUeyeThreaded::getHeight() {
	return this->height;
}

void ofxUeyeThreaded::update() {
	if (this->useTexture)
		texture.loadData(pixels);
}

void ofxUeyeThreaded::capture() {
	this->frameNewAtUpdate = this->frameNew;
	this->frameNew = false;
}

unsigned char* ofxUeyeThreaded::getPixels() {
	return this->pixels.getPixels();
}

ofPixels& ofxUeyeThreaded::getPixelsRef() {
	return this->pixels;
}

ofPixels ofxUeyeThreaded::getPixelsCopy() {
	ofPixels buffer;
	this->copyPixelsTo(buffer);
	return buffer;
}

bool ofxUeyeThreaded::isFrameNew() {
	return this->frameNewAtUpdate;
}

ofTexture& ofxUeyeThreaded::getTextureReference() {
	return this->texture;
}

void ofxUeyeThreaded::setUseTexture(bool useTexture) {
	this->useTexture = useTexture;
	if (useTexture)
		this->texture.allocate(this->pixels);
}

ofxUeye& ofxUeyeThreaded::getCamera() {
	return this->camera;
}

void ofxUeyeThreaded::apply(const ofxUeyePreset& preset) {
	preset.apply(this->getCamera());
}

void ofxUeyeThreaded::getFreshFrame() {
	lock();
	camera.capture();
	pixels = camera.getPixelsRef();
	unlock();
}

ofPixels ofxUeyeThreaded::getFreshFrameCopy() {
	lock();
	camera.capture();
	ofPixels copy = camera.getPixelsRef();
	unlock();
	return copy;
}

void ofxUeyeThreaded::threadedFunction() {
	while (this->isThreadRunning()) {
		if (!camera.capture())
			continue;
		lock();
		pixels = camera.getPixelsRef();
		frameNew = true;
		unlock();
		ofSleepMillis(1);
	}
}