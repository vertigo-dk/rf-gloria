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
    
    ofParameter<ofVec2f> gravity{"gravity", ofVec2f(0,0), ofVec2f(-1,-1), ofVec2f(1,1)};
    ofParameter<ofVec2f> emitPos{"emit", ofVec2f(0.6,0.5), ofVec2f(0,0), ofVec2f(1,1)};
    
    ofParameter<ofVec2f> emitLinePos{"emitLine", ofVec2f(0.0,0.0), ofVec2f(-1,-1), ofVec2f(1,1)};
    ofParameter<ofVec2f> emitLineDir{"emitLineDir", ofVec2f(0,0), ofVec2f(-1,-1), ofVec2f(1,1)};
    
    ofParameter<bool> clear{"clear", false};
    
    ofParameter<bool> drawObstacles{"drawObstacles", false};
    ofParameter<bool> useObstacles{"useObstacles", false};
    
    ofParameter<ofColor> color{"color", ofColor(255,255,255,255), ofColor(0,0,0,0), ofColor(255,255,255,255)};
    ofParameter<ofColor> colorLine{"colorLine", ofColor(255,255,255,255), ofColor(0,0,0,0), ofColor(255,255,255,255)};
    
    ofParameter<float> density{"density", 0, 0, 1};
    ofParameter<float> temperature{"temperature", 0, 0, 20};
    ofParameter<float> radius{"radius", 0, 0, 30};
    
    ofParameter<float> radiusLine{"radiusLine", 0, 0, 30};
    
    ofParameter<float> emitters{"emitters", 0, 0, 30};
    
    ofParameter<float> rotation{"rotation", 0, 0, 360};
    ofParameter<float> rotationSpeed{"rotationSpeed", 0, 0, 1};
    
    ofParameter<float> noiseDisplaceLine{};
    
    ofParameter<float> dissipation{"dissipation", 0.99, 0, 1};
    ofParameter<float> velocityDissipation{"velocityDissipation", 0.99, 0, 1};
    ofParameter<float> temperatureDissipation{"temperatureDissipation", 0.99, 0, 1};
    ofParameter<float> pressureDissipation{"pressureDissipation", 0.99, 0, 1};
    
    ofParameter<bool> reset{"reset", false};
    ofParameter<float> spacing{"spacing", 0, 0, 1};
    
    ofParameter<float> speed{"speed", 0, 0,1};
    
    ofParameterGroup params {"fluid",
        enabled,
        gravity,
        emitPos,
        emitLinePos,
        emitLineDir,
        clear,
        //drawObstacles,
        //useObstacles,
        color,
        colorLine,
        density,
        temperature,
        radius,
        radiusLine,
        emitters,
        rotation,
        rotationSpeed,
        noiseDisplaceLine,
        dissipation,
        velocityDissipation,
        temperatureDissipation,
        pressureDissipation,
        reset,
        spacing,
        speed
    };
    
    FluidScene() {
        ContentScene::params = params;
    }
    
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
    
    
    float rotationVal;
    
    
    ofFbo colorFbo;
    
    ofStyle style;
    
    bool newFrame;
    
};
