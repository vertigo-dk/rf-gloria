//
//  Gloria
//
// Vibeke Bertelsen 2014
//

#pragma once

#include "ofMain.h"
#include "ContentScene.h"
#include "mapping.h"
#include "Particle.h"

class CurlyFur : public ContentScene {

public:
    
    //particle
    ofParameter<float> plength{"length", 1, 1, 10};
    ofParameter<float> density{"density", 50, 50, 500};
    ofParameter<float> totalCount{"totalcount", 500, 500, 5000};
    ofParameter<float> fade{"fade", 1, 1, 10};
    
    ofParameterGroup params{
        "curly",
        enabled,
        plength,
        density,
        totalCount,
        fade
    };
    
    CurlyFur() {
        ContentScene::params = params;
    }
    
    void setup();
    void draw();
    void debugDraw();
    void update();
    
    //void setGui();
    
    void createParticle();
    
    //Particles
    vector<Particle> particles;
    
    //misc
    ofFbo myfbo;

};
