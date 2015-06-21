    //
//  QuickTrail.h
//  Gloria
//
//  Created by Johan Bichel Lindegaard on 02/07/2013.
//
//

#pragma once

#include "ofMain.h"
#include "ContentScene.h"
#include "mapping.h"

struct Walker {
    ofPolyline line;
    //vector<ofVec2f> points;
    vector<Corner*> corners;

    Corner * dst;
};

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


class QuickTrail : public ContentScene {

public:
    
    void setup();
    void draw();
    void debugDraw();
    void update();
    
    vector<Walker> walkers;
    
    int numtrails = 3;
    float length;
    int long lastadded;
    
    float circleRadius;
    
    bool random = false;
    bool clear;
    
    void parseOscMessage(ofxOscMessage * m);
    void setGui();
    
    float time;
    float interval = 1;
    
    bool trails = false;
    bool walkersDots = true;
    bool circles = false;
    bool linesPath = false;
    bool linesInters = false;
};
