#pragma once
#include "ofMain.h"
#include "ofxCv.h"

namespace ofxColorExtractor {
    vector<ofColor> getColorPalette(ofPixels pixels, int numColors);
    ofColor getComplement(ofColor color);
};