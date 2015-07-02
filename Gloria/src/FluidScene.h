//
//  Gloria
//
//  Created by Johan Bichel Lindegaard on 02/07/2013.
//
//

#pragma once

#include "ofMain.h"
#include "ContentScene.h"
#include "mapping.h"
#include "ofxFluid.h"

class FluidScene : public ContentScene {
    
public:
    
    void setup();
    void draw();
    void debugDraw();
    void update();
    
    void parseOscMessage(ofxOscMessage * m);
    
    float flowWidth;
    float flowHeight;
    float drawWidth;
    float drawHeight;

    ofFbo inputFbo;
    
    ofFbo obstacles;
    
    ofxFluid fluid;
    
    ofVec2f oldM;
    bool    bPaint, bObstacle, bBounding, bClear, bUpdate;
    
    ofParameter<ofVec2f> gravity;
    ofParameter<ofVec2f> emitPos;
    
    ofParameter<ofVec2f> emitLinePos;
    ofParameter<ofVec2f> emitLineDir;
    
    ofParameter<bool> clear;
    
    ofParameter<bool> drawObstacles;
    ofParameter<bool> useObstacles;
    
    ofParameter<ofColor> color;
    ofParameter<ofColor> colorLine;
    
    ofParameter<float> density;
    ofParameter<float> temperature;
    ofParameter<float> radius;
    
    ofParameter<float> radiusLine;
    
    ofParameter<float> emitters;
    
    ofParameter<float> rotation;
    ofParameter<float> rotationSpeed;
    
    float rotationVal;
    
    ofParameter<float> noiseDisplaceLine;
    
    ofParameter<float> dissipation;
    ofParameter<float> velocityDissipation;
    ofParameter<float> temperatureDissipation;
    ofParameter<float> pressureDissipation;
    
    ofParameter<bool> reset;
    ofParameter<bool> addColor;
    ofParameter<float> spacing;
    
    ofParameter<float> speed;
    
    
    ofFbo colorFbo;
    
    ofStyle style;
    
    bool newFrame;
    
};
