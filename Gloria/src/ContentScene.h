#pragma once

#include "ofMain.h"
#include "Defines.h"
#include "ofxOsc.h"
#include "ofxGui.h"
#include "Mapping.h"
#include "ofxSyphon.h"
#include "Output.hpp"

class ContentScene {
    
public:
    ofParameter<bool> enabled {"enabled", true};
    ofParameterGroup params {"untitled", enabled};
    
    OutputManager * outputManager;
    
    ContentScene(){
        updatingParameter = false;
        
    }
    
    virtual ~ContentScene(){
        ofRemoveListener(params.parameterChangedE(),this,&ContentScene::parameterChanged);
    };
    
    bool updatingParameter;
    
    Mapping * mapping;
    ofxSyphonClient * syphonIn;
    ofxSyphonServer syphonOut;
    ofxSyphonServer syphonOutFixture;
    
    ofxPanel panel;
    
    // vector<ofxOscSender *> oscClients;
    ofxOscSender   * oscSender;
    ofxOscReceiver * oscReceiver;
    
    int index;
    string name;
    string lastOscUpdatedParam = "";
    
    OutputFbo * fbo = nullptr;
    
    string panelSettingsPath;
    
    float speed;
    
    int width;
    int height;
    int numSamples = 4;
    
    virtual void init() {};
    virtual void setup() {};
    virtual void update() {};
    virtual void draw() {};
    virtual void exit();
    virtual void setGui() {};
    virtual void parseOscMessage(ofxOscMessage * m) {};
    
    virtual void enable() {};
    virtual void disable() {};
    
    void parameterChanged(ofAbstractParameter & parameter);
    
    ofParameterGroup & getParameters() {
        return params;
    }
    
    void enableToggled(bool & e);
    
    void parseSceneOscMessage(ofxOscMessage & m);
    void setupScene();
    void updateScene();
    void drawScene();
    
    void publishSyphonTexture();

private:

    
};
