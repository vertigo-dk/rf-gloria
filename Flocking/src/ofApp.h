#pragma once

/*
 *  testApp.h
 *  openFrameworks
 *
 *  ofxFlocking created by Jeffrey Crouse on 3/30/10.
 *  Updated for of10 by Frederik Tollund
 *
 */

#include "ofMain.h"
#include "ofxFlocking.h"
#include "ofxOsc.h"

class ofApp : public ofBaseApp{
	
public:
	void setup();
	void update();
	void draw();
	
	void mouseDragged(int x, int y, int button);
	
	ofxFlocking flock;
	ofxOscSender osc;
	ofxOscReceiver oscReceiver;
	bool enable = false;
};
