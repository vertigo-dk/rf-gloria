//
//  ChaoticAttractor.cpp
//  Gloria
//
//  Created by Johan Bichel Lindegaard on 6/18/15.
//
//

#include "ChaoticAttractor.h"


void ChaoticAttractor::setup(){
    
    name = "Chaotic";
    oscAddress = "/cha";
    
    params.add(drawLines.set("drawLines", false),
        iterate.set("iterate", false),
        scale.set("scale", ofVec2f(1,1), ofVec2f(0,0), ofVec2f(2,2)),
               maxIterations.set("maxIterations", 10000, 200, 4000000),
               lineWidth.set("lineWidth", 10,1,20),
               discardNonChaos.set("discardNonChaos", true));
    
}

void ChaoticAttractor::update(){
    
    if(discardNonChaos) {
        if(!attractor.drawIt) {
            attractor = Attractor();
        }
    }
    
    if(attractor.I > maxIterations) {
        //cout<<attractor.type<<endl;
        //if(searchChaos)
        attractor = Attractor();
    }
    
    if(iterate) {
    for(int i=0; i<1000; i++) {
        
        attractor.iterate();
            if(attractor.type == "chaotic") {
            //searchChaos = false;
        }
    }
    }
}

void ChaoticAttractor::draw(){;
    
    ofClear(0,0,0,0);
    
    //fbo.begin();
    ofBackground(255,255,255);
    ofSetColor(255,255,255,255);
    
    ofPushMatrix();
    ofTranslate(OUTWIDTH/2, OUTHEIGHT/2);
    ofScale(OUTWIDTH*scale.get().x,OUTHEIGHT*scale.get().y);
    
    glEnable(GL_VERTEX_PROGRAM_POINT_SIZE);
    
    ofSetLineWidth(lineWidth.get());
    if(drawLines) {
        attractor.mesh.setMode(OF_PRIMITIVE_LINES);
    } else {
        glPointSize(10);
        attractor.mesh.setMode(OF_PRIMITIVE_POINTS);
    }
    
    attractor.draw();
    
    ofPopMatrix();
    
    //ofDrawBitmapString(ofToString(attractor.I) + " " + attractor.type + " " + ofToString(attractor.seed), 20,20);
    //fbo.end();
    
    ofSetColor(255,255,255,255);
    //fbo.draw(0,0,OUTWIDTH,OUTHEIGHT);

 
}

