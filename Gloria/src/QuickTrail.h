    //
//  QuickTrail.h
//  Gloria
//
//  Created by Johan Bichel Lindegaard on 02/07/2013.
//
//

#pragma once

#include "ofMain.h"
#include "ContentScene.h"
#include "mapping.h"

struct Walker {
    ofPolyline line;
    //vector<ofVec2f> points;
    vector<Corner*> corners;

    Corner * dst;
};

class QuickTrail : public ContentScene {

public:
    
    ofParameter<float> speed{"speed", 0,0,1};
    
    ofParameter<bool> trails{"Trailse", false};
    ofParameter<bool> walkersDots{"walkersDots", false};
    ofParameter<bool> circles{"circles", false};
    ofParameter<bool> linesPath{"linesPath", false};
    ofParameter<bool> linesInters{"linesInters", false};
    
    ofParameter<float> circleRadius{"circleRadius", 0,0,200};
    
    ofParameter<bool> random{"random", false};
    ofParameter<bool> clear{"clear", false};
    
    ofParameter<int> numtrails{"numtrails", 0,0,8};
    ofParameter<float> length{"length", 0,0,20};
    
    
    
    ofParameterGroup params{
        "quicktrail",
        enabled,
        speed,
        trails,
        walkersDots,
        circles,
        linesPath,
        linesInters,
        circleRadius,
        random,
        clear,
        numtrails,
        length
    };
    
    QuickTrail() {
        ContentScene::params = params;

    }
    
    void setup();
    void draw();
    void debugDraw();
    void update();
    
    vector<Walker> walkers;
    

    int long lastadded;

    
    void parseOscMessage(ofxOscMessage * m);
    //void setGui();
    
    float time;
    float interval = 1;

};
