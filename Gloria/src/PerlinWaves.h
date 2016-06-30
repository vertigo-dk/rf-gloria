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
    
    ofParameter<ofVec2f> speed {"speed", ofVec2f(0, 0), ofVec2f(-1,-1),ofVec2f(1,1)};
    ofParameter<ofVec2f> scatter {"scatter", ofVec2f(0, 0), ofVec2f(0,0),ofVec2f(1,1)};
    ofParameter<ofColor> color {"color", ofColor(0,0,0,0), ofColor(0,0,0,0), ofColor(255,255,255,255)};

    
    ofParameter<ofVec2f> speedLED {"speedled", ofVec2f(0, 0), ofVec2f(-1,-1),ofVec2f(1,1)};
    ofParameter<ofVec2f> scatterLED {"scatterled", ofVec2f(0, 0), ofVec2f(0,0),ofVec2f(1,1)};
    ofParameter<ofColor> colorLED {"colorled", ofColor(0,0,0,0), ofColor(0,0,0,0), ofColor(255,255,255,255)};
    
    ofParameter<ofVec2f> ledRectScale {"ledrectscale", ofVec2f(0, 0), ofVec2f(0,0),ofVec2f(1,1)};
    
    ofParameter<bool> useMappingLED {"usemappingLED", false};
    
    ofParameter<float> numRectsLed {"numrectsled", 0.5,0
,1};
    
    
    ofParameter<bool> concentric {"concentric", true};
  
    ofParameter<bool> concentricLED {"concentricled", true};
    
    ofVec2f time;
    ofVec2f timeLED;
    
    ofParameterGroup params {"perlinwaves",
        enabled,
        speed,
        scatter,
        color,
        concentric,
        speedLED,
        scatterLED,
        colorLED,
        ledRectScale,
        useMappingLED,
        numRectsLed,
        //concentricLED
    };
    
    PerlinWaves() {
        hasFixtureOutput = true;
        
        ContentScene::params = params;
        
        time.x = 0;
        time.y = 0;
        
        timeLED.x = 0;
        timeLED.y = 0;
    }
    
    void setup();
    void draw();
    
    void drawFixtures();
    
    void debugDraw();
    void update();
    
    void parseOscMessage(ofxOscMessage * m);
    
    
    vector<ofRectangle> randomRects;
    
    
};
