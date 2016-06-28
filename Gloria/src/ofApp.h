#pragma once

#include "Defines.h"
#include "ofMain.h"

// Addons
#include "ofxGui.h"
#include "ofxSyphon.h"
#include "ofxOsc.h"
#include "ofxParameterFader.hpp"

#include "Mapping.h"
#include "ContentScene.h"
#include "Output.hpp"

// Scenes
#include "LampWalker.h"
#include "QuickTrail.h"
#include "Triangles.h"
#include "PerlinWaves.h"
#include "FluidScene.h"
#include "BasicParticles.h"
//#include "petriDish.h"
//#include "ChaoticAttractor.h"
#include "CurlyFur.h"

class ofApp : public ofBaseApp {
public:
    
    ofApp() {
        
        globalParams.setName("gloria");
        
        scenes.push_back(make_shared<FluidScene>());
        scenes.push_back(make_shared<QuickTrail>());
        scenes.push_back(make_shared<Triangles>());
        scenes.push_back(make_shared<PerlinWaves>());
        scenes.push_back(make_shared<BasicParticles>());
        //scenes.push_back(make_shared<ChaoticAttractor>());
        //scenes.push_back(make_shared<PetriDish>());
        scenes.push_back(make_shared<CurlyFur>());
        
        for( auto s : scenes) {
            globalParams.add(s->getParameters());
        }
                
    }
    
    ofParameterGroup sceneParams;
    ofParameterGroup globalParams;
    
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
    
    shared_ptr<ofxParameterFader> fadeManager;

    ofAbstractParameter * lastChangedParam = nullptr;
    
    //ofxSyphonServer syphonOut;
    ofxSyphonClient * syphonIn; // selected syphon in
    vector<ofxSyphonClient> syphonInputs;

    // Syphon Merger
    ofxSyphonServer syphonMerger;
    ofFbo syphonMergerFbo;
    
    // TODO: list of syphonClients
    ofxSyphonServerDirectory directory;
    void serverAnnounced(ofxSyphonServerDirectoryEventArgs &arg);
    void serverUpdated(ofxSyphonServerDirectoryEventArgs &args);
    void serverRetired(ofxSyphonServerDirectoryEventArgs &arg);
    int dirIdx;

    ofColor bg;
    //ofFbo fboOut;
    Mapping * mapping;
    ofxXmlSettings settings;
    
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
    
    OutputManager outputManager;
    
    vector<shared_ptr<ContentScene>> scenes;
    
    void selectSyphonInput(int input);
    
    vector<ofxPanel *> scenePanels;
    
    void paramsChanged(ofAbstractParameter & p) {
        lastChangedParam = &p;
    }
    
};
