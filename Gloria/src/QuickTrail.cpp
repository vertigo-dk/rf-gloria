//
//  QuickTrail.cpp
//  Gloria
//
//  Created by Johan Bichel Lindegaard on 02/07/2013.
//
//

#include "QuickTrail.h"

// circle intersection from : http://paulbourke.net/geometry/circlesphere/ code from Tim Voght

bool circle_circle_intersection(float x0, float y0, float r0,
                                float x1, float y1, float r1,
                                float *xi, float *yi,
                                float *xi_prime, float *yi_prime)
{
    float a, dx, dy, d, h, rx, ry;
    float x2, y2;
    
    /* dx and dy are the vertical and horizontal distances between
     * the circle centers.
     */
    dx = x1 - x0;
    dy = y1 - y0;
    
    /* Determine the straight-line distance between the centers. */
    //d = sqrt((dy*dy) + (dx*dx));
    d = hypot(dx,dy); // Suggested by Keith Briggs
    
    /* Check for solvability. */
    if (d > (r0 + r1))
    {
        /* no solution. circles do not intersect. */
        return false;
    }
    if (d < fabs(r0 - r1))
    {
        /* no solution. one circle is contained in the other */
        return false;
    }
    
    /* 'point 2' is the point where the line through the circle
     * intersection points crosses the line between the circle
     * centers.
     */
    
    /* Determine the distance from point 0 to point 2. */
    a = ((r0*r0) - (r1*r1) + (d*d)) / (2.0 * d) ;
    
    /* Determine the coordinates of point 2. */
    x2 = x0 + (dx * a/d);
    y2 = y0 + (dy * a/d);
    
    /* Determine the distance from point 2 to either of the
     * intersection points.
     */
    h = sqrt((r0*r0) - (a*a));
    
    /* Now determine the offsets of the intersection points from
     * point 2.
     */
    rx = -dy * (h/d);
    ry = dx * (h/d);
    
    /* Determine the absolute intersection points. */
    *xi = x2 + rx;
    *xi_prime = x2 - rx;
    *yi = y2 + ry;
    *yi_prime = y2 - ry;
    
    return true;
};


void QuickTrail::setup(){
    
    name = "quicktrail";
    
    time = 0;

    walkers.resize(8);
    lastadded = ofGetElapsedTimeMillis();
    
    ofSetCircleResolution(100);
    
    
    params.add(
               trails.set("Trailse", false),
               walkersDots.set("walkersDots", false),
               circles.set("circles", false),
               linesPath.set("linesPath", false),
               linesInters.set("linesInters", false),
               
               speed.set("speed", 0,0,1),
               length.set("length", 0,0,20),
               numtrails.set("numtrails", 0,0,8),
               random.set("random", false),
               clear.set("clear", false),
               circleRadius.set("circleRadius", 0,0,200)
               );
    
   
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
                        
                        ofDrawLine(interpolate, dstPt);
                        ofDrawCircle(interpolate.x, interpolate.y, circleRadius);
                        //ofVertex(walkers[i].points[p].x,w alkers[i].points[p].y);
                        
                    } else {
                        
                        ofDrawCircle(walkers[i].corners[p]->pos.x, walkers[i].corners[p]->pos.y, (p/length*1.0*circleRadius));
                        
                        ofDrawLine(walkers[i].corners[p]->pos.x, walkers[i].corners[p]->pos.y, walkers[i].corners[p-1]->pos.x, walkers[i].corners[p-1]->pos.y);
                        
                    }
                }
                
                
                if(walkersDots){
                    ofFill();

                    if(p > length-1) {
                        
                        
                        ofVec2f srcPt =walkers[i].corners[p-1]->pos;
                        ofVec2f dstPt =walkers[i].corners[p]->pos;
                        
                        ofVec2f interpolate = srcPt + (dstPt - srcPt) * pct;
                        
                        ofDrawCircle(interpolate.x, interpolate.y, circleRadius);
                    } else {
                        ofDrawCircle(walkers[i].corners[p]->pos.x, walkers[i].corners[p]->pos.y, (p/length*1.0*circleRadius));
                    }
                    ofNoFill();
                }
                
                
                if(circles){
                    ofVec2f srcPt =walkers[i].corners[p-1]->pos;
                    ofVec2f dstPt =walkers[i].corners[p]->pos;
                    
                    
                    if(p > length-1) {
                        
                        ofVec2f interpolate = srcPt + (dstPt - srcPt) * pct;
                        
                        ofDrawCircle(interpolate.x, interpolate.y, interpolate.distance(srcPt));
                        
                    } else {
                        
                        float radius = srcPt.distance(dstPt);// + (srcPt - dstPt)/2;
                        
                        ofDrawCircle(dstPt.x, dstPt.y, radius);
        
                    }
                }
                if(linesPath){
                    ofVec2f srcPt =walkers[i].corners[p-1]->pos;
                    ofVec2f dstPt =walkers[i].corners[p]->pos;
                    
                    if(p > length-1) {
                        
                        ofVec2f interpolate = srcPt + (dstPt - srcPt) * pct;

                        ofDrawLine(interpolate.x, interpolate.y, srcPt.x, srcPt.y);
                        
                    } else {
                        
                        ofDrawLine(dstPt.x, dstPt.y, srcPt.x, srcPt.y);
                        
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
                                
                                ofDrawLine(intersection0, intersection1);
                                ofDrawLine(srcPt, intersection1);
                                ofDrawLine(srcPt, intersection0);
                                ofDrawLine(interpolate, intersection1);
                                ofDrawLine(interpolate, intersection0);


                            }
                            
                            
                            ofDrawLine(interpolate.x, interpolate.y, srcPt.x, srcPt.y);
                            
                        } else {
                            
                            float r0 = prevSrcPt.distance(srcPt);
                            float r1 = srcPt.distance(dstPt);
                            
                            ofPoint intersection0;
                            ofPoint intersection1;
                            
                            if(circle_circle_intersection(srcPt.x, srcPt. y, r0, dstPt.x, dstPt.y, r1, &intersection0.x, &intersection0.y, &intersection1.x, &intersection1.y )){
                                
                                ofDrawLine(intersection0, intersection1);
                                ofDrawLine(srcPt, intersection1);
                                ofDrawLine(srcPt, intersection0);
                                ofDrawLine(dstPt, intersection1);
                                ofDrawLine(dstPt, intersection0);
                                
                            }
                            
                        }
                        
                    }
                    
                    
                    
                }
        }
    }
}




void QuickTrail::parseOscMessage(ofxOscMessage *m){

}

/*
void QuickTrail::setGui(){
    
    
    gui->addSlider("/speed/x", 0, 1, &speed);
    
    gui->addSlider("/length/x", 0, 20, &length);
    gui->addIntSlider("/trails/x", 0, 8, &numtrails);
    
    gui->addToggle("/random/x", &random);
    
    gui->addButton("/clear/x", &clear);
    
    gui->addSlider("/circleradius/x", 0, 200, &circleRadius);
    
}*/

