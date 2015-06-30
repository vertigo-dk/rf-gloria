//
//  Gloria
//
//  Created by vibber on 02/07/14.
//
//


#pragma once

#include "ofMain.h"
#include "ContentScene.h"
#include "mapping.h"
#include "ofxFX.h"
#include "ofxGrayScott.h"

#include <math.h>


struct Walker2 {
    ofPolyline line;
    //vector<ofVec2f> points;
    vector<Corner*> corners;
    
    Corner * dst;
};


///////////////////////////

class Qt {
    
public:
    
    void qtsetup();
    void qtdraw();
    void qtupdate(float speed, Mapping * mapping);
    
    vector<Walker2> walkers;
    
    int numtrails = 5;
    float length = 10;
    int long lastadded;
    
    float circleRadius = 5;
    
    bool random;
    bool clear = false;
        
    float time;
    float interval = 1;
    float animSpeed;
    
    
};

//////////////////

class PetriDish : public ContentScene {
    
public:
    
    void setup();
    void draw();
    void debugDraw();
    void update();
    void mousePressed(int x, int y, int button);
    
    void initOutputPlane(ofTexture texref);
    void drawOutput(ofTexture inputTex);
    
    float outWidth, outHeight;
    
    ofParameter<float> growingCells, eatingCells;
    ofParameter<float> trailLength;
    ofParameter<float> speed;
    ofParameter<bool> preset1;
    ofParameter<bool> preset2;
    ofParameter<bool> preset3;
    ofParameter<bool> eraseAll;
    
    ofPlanePrimitive plane;
    
    //SHADER
    ofShader shader;
    
    //Gray-scott effect
    ofxGrayScott    gray;
    
    //Quicktrails
    Qt qtScene;
};

