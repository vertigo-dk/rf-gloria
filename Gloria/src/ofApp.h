#pragma once

#include "ofMain.h"
#include "Defines.h"
#include "ofxSyphon.h"
#include "ofxOsc.h"
#include "ContentScene.h"
#include "LampWalker.h"
#include "QuickTrail.h"
#include "Triangles.h"
#include "PerlinWaves.h"
#include "ofxGui.h"
#include "Mapping.h"
#include "FluidScene.h"
#include "BasicParticles.h"
#include "petriDish.h"
#include "ChaoticAttractor.h"

class ofApp : public ofBaseApp {
public:

    void setup();
    void update();
    void draw();
    void debugDraw();
    void drawGrid();
    void exit();

    void keyPressed  (int key);
    void keyReleased(int key);
    void mouseMoved(int x, int y );
    void mouseDragged(int x, int y, int button);
    void mousePressed(int x, int y, int button);
    void mouseReleased(int x, int y, int button);
    void windowResized(int w, int h);
    void dragEvent(ofDragInfo dragInfo);
    void gotMessage(ofMessage msg);

    ofxOscReceiver oscReceiver;
    ofxOscSender oscSenderOne;
    
    //ofxSyphonServer syphonOut;
    ofxSyphonClient * syphonIn; // selected syphon in
    vector<ofxSyphonClient> syphonInputs;
    
    // TODO: list of syphonClients
    ofxSyphonServerDirectory directory;
    void serverAnnounced(ofxSyphonServerDirectoryEventArgs &arg);
    void serverUpdated(ofxSyphonServerDirectoryEventArgs &args);
    void serverRetired(ofxSyphonServerDirectoryEventArgs &arg);
    int dirIdx;

    ofColor bg;
    //ofFbo fboOut;
    Mapping * mapping;
    ofxXmlSettings XML;
    
    // gui
    ofParameterGroup globalParameters; 
    ofxPanel mainGui;
    ofParameter<bool> drawMapping;
    
    bool hideGUI;
    bool drawGuide;
    bool drawSyponIn;
    bool drawOutput;
    bool outputCombined;
    bool drawMask;
    
    vector<ContentScene*> scenes;
};
