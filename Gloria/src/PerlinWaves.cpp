//
//  QuickTrail.cpp
//  Gloria
//
//  Created by Johan Bichel Lindegaard on 02/07/2013.
//
//

#include "PerlinWaves.h"

void PerlinWaves::setup(){
    
    
    for(int i=0; i<200; i++) {
        randomRects.push_back(ofRectangle(ofRandom(0,LEDOUTWIDTH),ofRandom(0,LEDOUTHEIGHT), 10, 10));
    }
    
}

void PerlinWaves::update(){
    time += speed.get();
    timeLED += speedLED.get();
    
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


void PerlinWaves::drawFixtures() {
    
    ofClear(0,0,0,0);

    
    if(useMappingLED) {
    
      for(int i =0; i<mapping->fixtures.size();i++) {
          
          ofRectangle r = mapping->fixtures[i]->rect;

          
          float alpha = ofNoise((timeLED.y/10) - r.y
                                / ( scatterLED.get().y*LEDOUTHEIGHT),
                                (timeLED.x/10) - r.x
                                / ( scatterLED.get().x*LEDOUTWIDTH));
          
          
          ofSetColor( color.get().r, color.get().g, color.get().b,
                     alpha*255 );
          
          ofFill();
          
          
          
          ofSetRectMode(OF_RECTMODE_CENTER);
          
          ofDrawRectangle(r.x, r.y,(r.width * ledRectScale.get().x*100 * alpha), (r.height * ledRectScale.get().y*100 * alpha));
          
          
          ofSetRectMode(OF_RECTMODE_CORNER);


      }
        
        
    } else {
        
        int n =0;
        for(auto r : randomRects) {
            n++;
            if(n>numRectsLed.get()*200) break;
            
            
            float alpha = ofNoise((timeLED.y/10) - r.y
                                  / ( scatterLED.get().y*LEDOUTHEIGHT),
                                  (timeLED.x/10) - r.x
                                  / ( scatterLED.get().x*LEDOUTWIDTH));
            
            
            
            ofSetColor( colorLED.get().r, colorLED.get().g, colorLED.get().b,
                       alpha*255);
            
            ofFill();
            
            
            ofSetRectMode(OF_RECTMODE_CENTER);
            
            ofDrawRectangle(r.x, r.y,(r.width * ledRectScale.get().x*100 * alpha), (r.height * ledRectScale.get().y*100 * alpha));
            
            
            ofSetRectMode(OF_RECTMODE_CORNER);


            
        }
        
        
    }
    
}

void PerlinWaves::parseOscMessage(ofxOscMessage *m){
    
    

        
}
