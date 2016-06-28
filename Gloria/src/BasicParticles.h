//
//  BasicParticles.h
//  Gloria
//
//  Created by vibber on 02/07/14.
//
//


#pragma once

#include "ofMain.h"
#include "ContentScene.h"
#include "mapping.h"

class BasicParticle {
public:
    void setup();
    void draw();
    void update();
    void SetVelocity(float baseVelocity);
    void SetSize(float size);
    
    float baseVelocity;
    ofVec2f pos,vel,oldpos;
    float angle;
    ofColor color;
    float age;
    float fadeWithAge;
    float radius;
};

/////////////////////////

class BasicParticles : public ContentScene {
    
public:
    
    //particle
    ofParameter<float> pspeed{"speed", 5, 0.1, 50};
    ofParameter<float> psize{"size", 20, 0, 50};
    ofParameter<float> pFadeWithAge{"fadeparticle", 0.03, 0, 1};
    ofParameter<float> pkill{"kill", 10, 10, 1000};
    ofParameter<float> totalCount{"totalcount", 10000, 500, 40000};
    ofParameter<float> fade{"fade", 5, 0, 10};
    ofParameter<bool> trace{"trace", false};
    
    ofParameterGroup params{
        "basicparticles",
        enabled,
        pspeed,
        psize,
        pFadeWithAge,
        pkill,
        totalCount,
        fade,
        trace
    };
    
    BasicParticles() {
        ContentScene::params = params;
    }
    
    void setup();
    void draw();
    void debugDraw();
    void update();
    
    // void setGui();
    
    void createParticle();
    void reassignParticle();
    void updateParticleSettings(BasicParticle& p);
    
    
        
    //Particles
    vector<BasicParticle> particles;
    int iterate;
    
    //syphon to fbo
    ofFbo syphonFbo;
    ofPixels pixels;
    
    //Scene to fbo
    ofFbo pingPongFbo1;
    ofFbo pingPongFbo2;
    bool toggleFbo;
    

};

