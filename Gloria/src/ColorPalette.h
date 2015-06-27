#pragma once

#include "ofMain.h"
#include "ContentScene.h"
#include "ofxColorExtractor.h"


class ColorPalette : public ContentScene {
public:
    void setup();
    void draw();
    void debugDraw();
    void update();

    void parseOscMessage(ofxOscMessage * m);
    void setGui();

    ofFbo fbo;
    ofPixels pixels;
    ofImage img;

};