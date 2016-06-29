//
//  QuickTrail.cpp
//  Gloria
//
//  Created by Johan Bichel Lindegaard on 02/07/2013.
//
//

#include "PerlinWaves.h"

void PerlinWaves::setup(){
    
}

void PerlinWaves::update(){
    time += speed.get();
}

void PerlinWaves::draw(){;
    
    ofClear(0,0,0,0);
    
    ofSetLineWidth(4);
    glEnable(GL_LINES);
    
     for(int i =0; i<mapping->triangles.size();i++) {
         
         
         
         ofSetColor( color.get().r, color.get().g, color.get().b,
                    ofNoise((time.y/10) - mapping->triangles[i]->centroid.y
                            / ( scatter.get().y*OUTHEIGHT),
                    (time.x/10) - mapping->triangles[i]->centroid.x
                    / ( scatter.get().x*OUTWIDTH) ) *255 );
   
         
         mapping->triangles[i]->mesh.draw();
         
         /*
         ofSetColor( 255, 255, 255,
                    ofNoise( (time.x/10) - mapping->triangles[i]->centroid.x
                            / ( scatter.get().x*OUTWIDTH) ) *255 );
         
         mapping->triangles[i]->mesh.draw();
         */
     }
}

void PerlinWaves::parseOscMessage(ofxOscMessage *m){
    
        
}
