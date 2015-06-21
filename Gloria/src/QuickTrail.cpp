//
//  QuickTrail.cpp
//  Gloria
//
//  Created by Johan Bichel Lindegaard on 02/07/2013.
//
//

#include "QuickTrail.h"

void QuickTrail::setup(){
    
    name = "QuickTrail";
    oscAddress = "/quicktrail";
    
    time = 0;

    walkers.resize(8);
    lastadded = ofGetElapsedTimeMillis();
    
    ofSetCircleResolution(100);
}

void QuickTrail::update(){
    
    time += speed;
    
    for(int i=0;i<walkers.size();i++) {
        
        if(clear) {
            walkers[i].corners.clear();
            clear = false;
        }
        
        if(walkers[i].corners.empty()) {
            walkers[i].dst = mapping->corners[ofRandom(mapping->corners.size())];
            walkers[i].corners.push_back(mapping->corners[ofRandom(mapping->corners.size())]);
        }
    }
    
    for(int i=0;i<walkers.size() && i<numtrails; i++) {
        if(i<numtrails) {

            if(walkers[i].corners.size() < length) {
                if(random) {
                    walkers[i].dst = mapping->corners[ofRandom(mapping->corners.size())];
                } else {
                    walkers[i].dst = walkers[i].dst->joinedCorners[ofRandom(walkers[i].dst->joinedCorners.size())];
                }
                
                walkers[i].corners.push_back(walkers[i].dst);

                lastadded = time;
            } else {

                if(time - lastadded > interval ) {

                    walkers[i].corners.erase(walkers[i].corners.begin());

                }
            }
        }
    }
}

void QuickTrail::draw(){;
    
    
    
    ofClear(0,0);
    
    ofSetLineWidth(10);
    ofNoFill();

    float pct = ofClamp((time - lastadded) / (interval * 1.), 0,1);

        for(int i=0;i<walkers.size() && i <numtrails;i++) {

            for(int p=1; p<walkers[i].corners.size() && p<length; p++) {
                
                ofSetColor(255,255,255,(p/length*1.0*255));
            


                
                if(trails){
                    if(p > length-1) {
                        
                        
                        ofVec2f srcPt =walkers[i].corners[p-1]->pos;
                        ofVec2f dstPt =walkers[i].corners[p]->pos;
                        
                        ofVec2f interpolate = srcPt + (dstPt - srcPt) * pct;
                        
                        ofLine(interpolate, dstPt);
                        ofCircle(interpolate.x, interpolate.y, circleRadius);
                        //ofVertex(walkers[i].points[p].x,w alkers[i].points[p].y);
                        
                    } else {
                        
                        ofCircle(walkers[i].corners[p]->pos.x, walkers[i].corners[p]->pos.y, (p/length*1.0*circleRadius));
                        
                        ofLine(walkers[i].corners[p]->pos.x, walkers[i].corners[p]->pos.y, walkers[i].corners[p-1]->pos.x, walkers[i].corners[p-1]->pos.y);
                        
                    }
                }
                
                
                if(walkersDots){
                    ofFill();

                    if(p > length-1) {
                        
                        
                        ofVec2f srcPt =walkers[i].corners[p-1]->pos;
                        ofVec2f dstPt =walkers[i].corners[p]->pos;
                        
                        ofVec2f interpolate = srcPt + (dstPt - srcPt) * pct;
                        
                        ofCircle(interpolate.x, interpolate.y, circleRadius);
                    } else {
                        ofCircle(walkers[i].corners[p]->pos.x, walkers[i].corners[p]->pos.y, (p/length*1.0*circleRadius));
                    }
                    ofNoFill();
                }
                
                
                if(circles){
                    ofVec2f srcPt =walkers[i].corners[p-1]->pos;
                    ofVec2f dstPt =walkers[i].corners[p]->pos;
                    
                    
                    if(p > length-1) {
                        
                        ofVec2f interpolate = srcPt + (dstPt - srcPt) * pct;
                        
                        ofCircle(interpolate.x, interpolate.y, interpolate.distance(srcPt));
                        
                    } else {
                        
                        float radius = srcPt.distance(dstPt);// + (srcPt - dstPt)/2;
                        
                        ofCircle(dstPt.x, dstPt.y, radius);
        
                    }
                }
                if(linesPath){
                    ofVec2f srcPt =walkers[i].corners[p-1]->pos;
                    ofVec2f dstPt =walkers[i].corners[p]->pos;
                    
                    if(p > length-1) {
                        
                        ofVec2f interpolate = srcPt + (dstPt - srcPt) * pct;

                        ofLine(interpolate.x, interpolate.y, srcPt.x, srcPt.y);
                        
                    } else {
                        
                        ofLine(dstPt.x, dstPt.y, srcPt.x, srcPt.y);
                        
                    }

                }
                if(linesInters){
                    if(p > 2){
                        ofVec2f prevSrcPt =walkers[i].corners[p-2]->pos;
                        ofVec2f srcPt =walkers[i].corners[p-1]->pos;
                        ofVec2f dstPt =walkers[i].corners[p]->pos;
                        
                        if(p > length-1) {
                            
                            ofVec2f interpolate = srcPt + (dstPt - srcPt) * pct;
                            
                            float r0 = prevSrcPt.distance(srcPt);
                            float r1 = srcPt.distance(interpolate);
                            
                            
                            ofPoint intersection0;
                            ofPoint intersection1;
                            
                            if(circle_circle_intersection(srcPt.x, srcPt. y, r0, interpolate.x, interpolate.y, r1, &intersection0.x, &intersection0.y, &intersection1.x, &intersection1.y )){
                                
                                ofLine(intersection0, intersection1);
                                ofLine(srcPt, intersection1);
                                ofLine(srcPt, intersection0);
                                ofLine(interpolate, intersection1);
                                ofLine(interpolate, intersection0);


                            }
                            
                            
                            ofLine(interpolate.x, interpolate.y, srcPt.x, srcPt.y);
                            
                        } else {
                            
                            float r0 = prevSrcPt.distance(srcPt);
                            float r1 = srcPt.distance(dstPt);
                            
                            ofPoint intersection0;
                            ofPoint intersection1;
                            
                            if(circle_circle_intersection(srcPt.x, srcPt. y, r0, dstPt.x, dstPt.y, r1, &intersection0.x, &intersection0.y, &intersection1.x, &intersection1.y )){
                                
                                ofLine(intersection0, intersection1);
                                ofLine(srcPt, intersection1);
                                ofLine(srcPt, intersection0);
                                ofLine(dstPt, intersection1);
                                ofLine(dstPt, intersection0);
                                
                                
                            }
                            
                        }
                        
                    }
                    
                    
                    
                }
        }
    }
}




void QuickTrail::parseOscMessage(ofxOscMessage *m){

}

void QuickTrail::setGui(){
    
    
    gui->addSlider("/speed/x", 0, 1, &speed);
    
    gui->addSlider("/length/x", 0, 20, &length);
    gui->addIntSlider("/trails/x", 0, 8, &numtrails);
    
    gui->addToggle("/random/x", &random);
    
    gui->addButton("/clear/x", &clear);
    
    gui->addSlider("/circleradius/x", 0, 200, &circleRadius);
    
    
    gui->addToggle("/trails/x", &trails);
    gui->addToggle("/walkersDots/x", &walkersDots);
    gui->addToggle("/circles/x", &circles);
    gui->addToggle("/linesPath/x", &linesPath);
    
    gui->addToggle("/linesInters/x", &linesInters);

    
}
