#include "ofxUeyeThreaded.h"

ofxUeyeThreaded::ofxUeyeThreaded() {
	this->width = 0;
	this->height = 0;
	this->frameNew = false;
	this->useTexture = true;
	this->threadPaused = false;
}

ofxUeyeThreaded::~ofxUeyeThreaded() {
	this->close();
}

bool ofxUeyeThreaded::init(int deviceOrCameraID, bool useCameraID, int colorMode) {
	close();
	if (!camera.init(deviceOrCameraID, useCameraID, colorMode))
		return false;
	else {
		initBase();
		return true;
	}
}

bool ofxUeyeThreaded::init(const ofxUeyeDevice& device, int colorMode) {
	close();
	if (!camera.init(device, colorMode))
		return false;
	else {
		initBase();
		return true;
	}
}

void ofxUeyeThreaded::initBase() {
	this->camera.setUseTexture(false);
	
	camera.capture();
	this->allocate();

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

void ofxUeyeThreaded::allocate() {
	pixels = camera.getPixelsRef();
	texture.allocate(this->pixels);
	this->width = pixels.getWidth();
	this->height = pixels.getHeight();
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
	if (buffer.isAllocated())
		return buffer;
	else
		return ofPixels();
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

const ofxUeye& ofxUeyeThreaded::getCamera() const {
	return this->camera;
}

void ofxUeyeThreaded::apply(const ofxUeyePreset& preset) {
	preset.apply(this->getCamera());
	if ( this->getCamera().getWidth() != this->getWidth() || this->getCamera().getHeight() != this->getHeight() )
		this->allocate();
}

void ofxUeyeThreaded::getFreshFrame() {
	camera.capture();
	lock();
	pixels = camera.getPixelsRef();
	unlock();
}

ofPixels ofxUeyeThreaded::getFreshFrameCopy() {
	camera.capture();
	lock();
	ofPixels copy = camera.getPixelsRef();
	unlock();
	return copy;
}

void ofxUeyeThreaded::getFreshFrameCopyTo(ofPixels& pixels) {
	camera.capture();
	lock();
	pixels = camera.getPixelsRef();
	unlock();
}


void ofxUeyeThreaded::setThreadPaused(bool threadPaused) {
	this->threadPaused = threadPaused;
}

void ofxUeyeThreaded::threadedFunction() {
	while (this->isThreadRunning()) {
		ofSleepMillis(1);
		if (this->threadPaused)
			continue;
		if (!camera.capture())
			continue;
		lock();
		pixels = camera.getPixelsRef();
		frameNew = true;
		unlock();
	}
}