//
//  Wrapper.hpp
//  Gloria
//
//  Created by Johan Bichel Lindegaard on 7/1/16.
//
//

#pragma once


#include "ofMain.h"
#include "ContentScene.h"
#include "mapping.h"


class Wrapper : public ContentScene {
    
public:
    ofParameter<ofVec2f> transform {"transform", ofVec2f(1, 1), ofVec2f(-1,-1),ofVec2f(1,1)};
    ofParameter<float> zoom{"zoom", 0, 0, 1};
    ofParameter<float> canvasScale{"canvasScale", 0, 0, 1};
    ofParameter<float> modscale{"transform", 0, 0, 1};
    ofParameter<ofVec2f> ndModTransform {"modtransform", ofVec2f(1, 1), ofVec2f(-1,-1),ofVec2f(1,1)};
    ofParameter<int> modN{"modN", 0, 0, 10};
    ofParameterGroup params {"wrapper",
        enabled,
        zoom,
        transform,
        canvasScale,
    };
    
    Wrapper() {
        ContentScene::params = params;
    }
    
    void setup();
    void draw();
    void update();
};
