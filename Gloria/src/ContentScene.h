#pragma once

#include "ofMain.h"
#include "Defines.h"
#include "ofxOsc.h"
#include "ofxGui.h"
#include "mapping.h"
#include "ofxSyphon.h"

class ContentScene {
    
public:
    
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
    
    ofParameterGroup params;
    ofxPanel panel;
    
    // vector<ofxOscSender *> oscClients;
    ofxOscSender   * oscSender;
    ofxOscReceiver * oscReceiver;
    
    int index;
    string name;
    string lastOscUpdatedParam = "";
    
    ofFbo fbo;
    //bool enabled;
    ofParameter<bool>  enabled;
    
    string panelSettingsPath;
    
    float speed;
    
    int width;
    int height;
    int numSamples = 4;
    
    virtual void init();
    virtual void setup();
    virtual void update();
    virtual void draw();
    virtual void exit();
    virtual void setGui();
    virtual void parseOscMessage(ofxOscMessage * m);
    
    void parameterChanged(ofAbstractParameter & parameter);
    
    void setSceneGui();
    void setSceneParameters();
    void addSlider();
    void checkMsg();
    void parseSceneOscMessage(ofxOscMessage & m);
    void setupScene(int _width, int _height, int _i);
    void updateScene();
    void drawScene();
    
    void publishSyphonTexture();
};
