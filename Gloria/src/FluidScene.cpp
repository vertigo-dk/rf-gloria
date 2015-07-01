//
//  QuickTrail.cpp
//  Gloria
//
//  Created by Johan Bichel Lindegaard on 02/07/2013.
//
//

#include "FluidScene.h"

void FluidScene::setup(){
    
    name = "fluid";
    
    float scaleFactor = 1.0;
    
    drawWidth = OUTWIDTH/scaleFactor;
	drawHeight = OUTHEIGHT/scaleFactor;
	
    fluid.allocate(drawWidth, drawHeight, 0.2);
    
    //  Set obstacle
    //
    fluid.begin();
    
    ofSetColor(0,0);
    ofSetColor(255);
    ofDrawCircle(drawWidth*0.5, drawHeight*0.35, 200);
    fluid.end();
    
    obstacles.allocate(drawWidth, drawHeight);
   
    
    //fluid.setObstacles(obstacles);
    fluid.setUseObstacles(false);
    
    fluid.setPasses(8);

    params.add(gravity.set("gravity", ofVec2f(0,0), ofVec2f(-1,-1), ofVec2f(1,1)),
               emitPos.set("emit", ofVec2f(0.5,0.5), ofVec2f(0,0), ofVec2f(1,1)),intensity.set("intensity", 0, 0, 30),
               dissipation.set("dissipation", 0.99, 0, 1),
            velocityDissipation.set("velocityDissipation", 0.99, 0, 1),
            temperatureDissipation.set("temperatureDissipation", 0.99, 0, 1),pressureDissipation.set("pressureDissipation", 0.99, 0, 1),clear.set("clear", false));
    
    drawObstacles = true;
   
}

void FluidScene::update(){
    // Adding temporal Force
   
    fluid.dissipation = dissipation;
    fluid.velocityDissipation = velocityDissipation;
    fluid.temperatureDissipation = temperatureDissipation;
    fluid.pressureDissipation = pressureDissipation;
    
    
    fluid.setGravity(gravity.get());

    ofPoint m = emitPos.get() * ofVec2f(drawWidth, drawHeight);
    ofPoint d = (m - oldM)*10.0;
    oldM = m;
    
    fluid.addTemporalForce(m, d, ofFloatColor(0.8, 0.8, 0.8), intensity);
    
    // add velocity or add color based on image
    
    //fluid.addTemporalForce(m, d, ofFloatColor(1, 1, 1),16.0f);
    //fluid.addTemporalForce(m, d, ofFloatColor(c.x,c.y,0.5)*sin(ofGetElapsedTimef()),3.0f);
    //  Update
    //

    
    if(clear) {
        fluid.clear();
        fluid.setObstacles(obstacles);
        clear.set(false);
    }
    
    ofPushStyle();
    //if(ofGetFrameNum() % 2 == 1) {
        fluid.update();
    //}
 
    ofPopStyle();
}

void FluidScene::draw(){;
    
    ofClear(0,0,0);
    
    ofSetColor(255,255,255,255);
    
    if(drawObstacles) {
        ofSetColor(255,255,255,255);
        obstacles.draw(0,0,OUTWIDTH, OUTHEIGHT);
        //fluid.drawVelocity();
    }
    
    
    
    fluid.draw(0,0,OUTWIDTH,OUTHEIGHT);
    //syphonIn->getTexture().draw(0,0,OUTWIDTH,OUTHEIGHT);
    

}

void FluidScene::parseOscMessage(ofxOscMessage *m){
}

