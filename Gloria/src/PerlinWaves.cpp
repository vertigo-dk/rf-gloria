//
//  QuickTrail.cpp
//  Gloria
//
//  Created by Johan Bichel Lindegaard on 02/07/2013.
//
//

#include "PerlinWaves.h"
#include "ofxEasing.h"

void PerlinWaves::setup() {
	for (int i = 0; i < 200; i++) {
		myRectangle myRect;
		myRect.set(ofRandom(0, LEDOUTWIDTH), ofRandom(0, LEDOUTHEIGHT), 10, 10);
		myRect.bDraw = true;
		randomRects.push_back(myRect);
	}
}

void PerlinWaves::update() {
	if (speed.get().x > 0) {
		time.x +=
		ofxeasing::map(speed.get().x, 0, 1, 0, 1, ofxeasing::quart::easeIn);
	} else {
		time.x +=
		ofxeasing::map(speed.get().x, -1, 0, -1, 0, ofxeasing::quart::easeOut);
	}
	
	if (speed.get().y > 0) {
		time.y +=
		ofxeasing::map(speed.get().y, 0, 1, 0, 1, ofxeasing::quart::easeIn);
	} else {
		time.y +=
		ofxeasing::map(speed.get().y, -1, 0, -1, 0, ofxeasing::quart::easeOut);
	}
	
	if (speedLED.get().x > 0) {
		timeLED.x +=
		ofxeasing::map(speedLED.get().x, 0, 1, 0, 1, ofxeasing::quart::easeIn);
	} else {
		timeLED.x += ofxeasing::map(speedLED.get().x, -1, 0, -1, 0,
									ofxeasing::quart::easeOut);
	}
	
	if (speedLED.get().y > 0) {
		timeLED.y +=
		ofxeasing::map(speedLED.get().y, 0, 1, 0, 1, ofxeasing::quart::easeIn);
	} else {
		timeLED.y += ofxeasing::map(speedLED.get().y, -1, 0, -1, 0,
									ofxeasing::quart::easeOut);
	}
	prob = ofxeasing::map(propability.get(), 0, 1, 0, 1, ofxeasing::quart::easeOut);
}

void PerlinWaves::draw() {
	ofClear(0, 0, 0, 0);
	
	ofSetLineWidth(4);
	glEnable(GL_LINES);
	
	for (int i = 0; i < mapping->triangles.size(); i++) {
		float alpha;
		
		if (concentric) {
			alpha = ofNoise((time.x / 10) - mapping->triangles[i]->centroid.distance(
																					 ofPoint(OUTWIDTH / 2, OUTHEIGHT * 0.82)) /
							(scatter.get().x * OUTHEIGHT));
		} else {
			alpha = ofNoise((time.y / 10) - mapping->triangles[i]->centroid.y /
							(scatter.get().y * OUTHEIGHT),
							(time.x / 10) - mapping->triangles[i]->centroid.x /
							(scatter.get().x * OUTWIDTH));
		}
		
		ofSetColor(color.get().r, color.get().g, color.get().b, alpha * 255);
		
		if (ofRandom(1) > prob)
			mapping->triangles[i]->bDraw = !mapping->triangles[i]->bDraw;
		if (mapping->triangles[i]->bDraw)
			mapping->triangles[i]->mesh.draw();
	}
}

void PerlinWaves::parseOscMessage(ofxOscMessage *m) {}
