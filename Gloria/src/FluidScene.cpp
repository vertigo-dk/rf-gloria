//
//  QuickTrail.cpp
//  Gloria
//
//  Created by Johan Bichel Lindegaard on 02/07/2013.
//
//

#include "FluidScene.h"

void FluidScene::setup(){
    
    rotationVal= 0;
    name = "fluid";
    
    float scaleFactor = 1.0;
    
    drawWidth = OUTWIDTH/scaleFactor;
	drawHeight = OUTHEIGHT/scaleFactor;
	
    fluid.allocate(drawWidth, drawHeight, 0.2);
    
    colorFbo.allocate(drawWidth, drawHeight);
    
    //fluid.setObstacles(obstacles);
    fluid.setUseObstacles(false);
    
    //fluid.setPasses(2);

    params.add(gravity.set("gravity", ofVec2f(0,0), ofVec2f(-1,-1), ofVec2f(1,1)),
               
               clear.set("clear", false),
               
               temperature.set("temperature", 0, 0, 20),
               density.set("density", 0, 0, 1),
               dissipation.set("dissipation", 0.99, 0, 1),
               velocityDissipation.set("velocityDissipation", 0.99, 0, 1),
               temperatureDissipation.set("temperatureDissipation", 0.99, 0, 1),pressureDissipation.set("pressureDissipation", 0.99, 0, 1),
               
               emitPos.set("emit", ofVec2f(0.6,0.5), ofVec2f(0,0), ofVec2f(1,1)),
               radius.set("radius", 0, 0, 30),
               
               color.set("color", ofColor(255,255,255,255), ofColor(0,0,0,0), ofColor(255,255,255,255)),
               
               emitLinePos.set("emitLine", ofVec2f(0.0,0.0), ofVec2f(-1,-1), ofVec2f(1,1)),
               
               emitLineDir.set("emitLineDir", ofVec2f(0,0), ofVec2f(-1,-1), ofVec2f(1,1)),
               
               
               colorLine.set("colorLine", ofColor(255,255,255,255), ofColor(0,0,0,0), ofColor(255,255,255,255)),
               emitters.set("emitters", 0, 0, 30),
               rotation.set("rotation", 0, 0, 360),
               rotationSpeed.set("rotationSpeed", 0, 0, 1),
               spacing.set("spacing", 0, 0, 1),
               radiusLine.set("radiusLine", 0, 0, 30)
               /*reset.set("reset", false),
               useObstacles.set("useObstacles", false),
               drawObstacles.set("drawObstacles", false),
               addColor.set("addColor", false)*/
               
               );
    

    
   }

void FluidScene::update(){
    // Adding temporal Force
    
    ofSetColor(255, 255, 255, 255);
    
    fluid.dissipation = dissipation;
    fluid.velocityDissipation = velocityDissipation;
    fluid.temperatureDissipation = temperatureDissipation;
    fluid.pressureDissipation = pressureDissipation;
    fluid.setGravity(gravity.get());
    
    
    ofPoint m = emitPos.get() * ofVec2f(drawWidth, drawHeight);
    ofPoint d = (m - oldM)*10.0;
    oldM = m;
    
    fluid.addTemporalForce(m, d, color.get(), radius, temperature, density);
    
    ofVec2f center = ofVec2f(drawWidth/2, drawHeight/2);
    
    rotationVal += rotationSpeed*30/ofGetFrameRate();
    
    if(rotationVal>=360) {
        rotationVal = 0;
    }
    
    
    for(int i =0; i<emitters; i++) {
        
        ofVec2f pos = ofVec2f((drawWidth*spacing)/(emitters+1)*(i+1), drawHeight*0.5);
        
        pos.x = pos.x + (drawWidth-(drawWidth*spacing))/2;
        
        pos = pos.getRotated(rotationVal, center);
        pos = pos.getRotated(rotation, center);
        //pos = pos.getRotated(rotation, center);
        
        pos += emitLinePos.get() * ofVec2f(drawWidth, drawHeight);
        
        //pos += emit2Pos;
        
        //pos.x = sin(rotation) * drawWidth/(emitters+1)*(i+1);
        //pos.y = cos(rotation) * drawHeight*0.9;
        
        fluid.addTemporalForce(
                               ofVec2f(pos), emitLineDir.get()*10.0, colorLine.get(), radiusLine, temperature, density);
        
    }
    
    if(clear) {
        fluid.clear();
        //fluid.setObstacles(obstacles);
        clear.set(false);
    }
    
    
    /*if(drawObstacles) {
        ofSetColor(255,255,255,255);
        obstacles.draw(0,0,OUTWIDTH, OUTHEIGHT);
        //fluid.drawVelocity();
    }*/
    
    
    //
    
    
    style = ofGetStyle();
    fluid.update();
    
    
    if(newFrame) {
        fbo.begin();
        fluid.draw(0,0,OUTWIDTH,OUTHEIGHT);
        fbo.end();
        
        newFrame = false;
    }
    
    ofSetStyle(style);
}

void FluidScene::draw(){
    
    
    newFrame = true;
    //ofDisableBlendMode();
    //ofDisableBlendMode();
    //ofPushStyle();
    
    //syphonIn->getTexture().draw(0,0,OUTWIDTH,OUTHEIGHT);
    //ofPopStyle();
}

void FluidScene::parseOscMessage(ofxOscMessage *m){
}

