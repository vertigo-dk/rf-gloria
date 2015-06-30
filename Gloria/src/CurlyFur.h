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
    
    //particle
    ofParameter<float> plength;
    ofParameter<float> density;
    ofParameter<float> totalCount;
    ofParameter<float> fade;
};
