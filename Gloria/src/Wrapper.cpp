//
//  Wrapper.cpp
//  Gloria
//
//  Created by Johan Bichel Lindegaard on 7/1/16.
//
//

#include "Wrapper.hpp"


void Wrapper::setup() {
    
    for(auto t : mapping->triangles) {
        t->modMesh = t->mesh;
    }
    
    
}

void Wrapper::update(){
    
    
    for(auto t : mapping->triangles) {
        
        
        ofPoint p1 = ofPoint(0,0);
        ofPoint p2 = ofPoint(1,0);
        ofPoint p3 = ofPoint(0,1);
        
        
        //
        
        p1.interpolate(ofPoint(0.5,0.5), zoom);
        p2.interpolate(ofPoint(0.5,0.5), zoom);
        p3.interpolate(ofPoint(0.5,0.5), zoom);
        
        p1 += transform.get();
        p2 += transform.get();
        p3 += transform.get();
        
        
        
        
        
        t->modMesh.setTexCoord(0,  p1 );
        t->modMesh.setTexCoord(1,  p2 );
        t->modMesh.setTexCoord(2,  p3 );
      
       
    }
    
    
}

void Wrapper::draw(){;
    
    ofEnableNormalizedTexCoords();
    //glEnable(GL_REPEAT);
    
    //ofSetTextureWrap(GL_REPEAT);
    
    ofClear(0,0,0,0);
    
    ofSetColor(255);
    for(auto t : mapping->triangles) {
        
        ofPushMatrix();
        
        syphonIn->bind();
        
        ofTranslate(t->getCenter());
        ofScale(canvasScale, canvasScale);
        ofTranslate(-t->getCenter());

        
        t->modMesh.draw();
        
        syphonIn->unbind();
        
        ofPopMatrix();
        
    }
    
    ofDisableNormalizedTexCoords();
    
}


void Wrapper::drawFixtures() {
    
}