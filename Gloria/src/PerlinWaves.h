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

class PerlinWaves : public ContentScene {

public:
    
    void setup();
    void draw();
    void debugDraw();
    void update();
    
    void parseOscMessage(ofxOscMessage * m);
    
    ofParameter<ofVec2f> speed;
    ofParameter<ofVec2f> scatter;
    ofVec2f time;
    
    //ofParameter<float> rotation;
};
