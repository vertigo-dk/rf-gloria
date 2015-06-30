//
//  ContentScene.cpp
//  Gloria
//
//  Created by Johan Bichel Lindegaard on 27/06/14.
//
//

#include "ContentScene.h"

void ContentScene::init() {
}
void ContentScene::setup(){
}
void ContentScene::update(){
}
void ContentScene::draw() {
}
void ContentScene::setGui() {
}
void ContentScene::parseOscMessage(ofxOscMessage *m) {
}

void ContentScene::setupScene(int _width, int _height, int _i) {
    index = _i;
    
    ofFbo::Settings settings;
    
    height = _height;
    width  = _width;
    
    settings.height     = _height;
    settings.width      = _width;
    settings.numSamples = numSamples;
    settings.useDepth   = false;
    settings.internalformat = GL_RGBA;
    
    fbo.allocate(settings);
    
    fbo.begin();
    ofClear(0,0,0,0);
    fbo.end();
    
    params.add(enabled.set("Enabled", false));
    
    setup();
    
    params.setName(name);
    ofAddListener(params.parameterChangedE(),this,&ContentScene::parameterChanged);
    
    ofFile file(name+"_settings.xml");
    
    panel.setup(params, file.path());
    panel.setName(name);
    
    //setSceneParameters();
    
    syphonOut.setName(name);
    
    if(!file.exists()) {
        file.create();
        panel.saveToFile(file.path());
    } else {
        panel.loadFromFile(file.path());
    }
    
}

void ContentScene::setSceneParameters(){
    
}

void ContentScene::parameterChanged( ofAbstractParameter & parameter ){
    
    // this check might not be good for BOOL clears for instance
    // but necesarry to preserve interpolation on Lemur sliders
    // clientside
    // what happens if multiple interactions occur before this?
    
    if(lastOscUpdatedParam == parameter.getEscapedName()) {
        
    } else {
        oscSender->sendParameter(parameter);
    }
}

void ContentScene::setSceneGui(){
}

void ContentScene::parseSceneOscMessage(ofxOscMessage & m){
    
    lastOscUpdatedParam = "";
    
    bool isScene = false;
    
    string adr = m.getAddress();
    ofStringReplace(adr, "_", "/");
    
	vector<string> adrSplit = ofSplitString(adr, "/");
    
    string sceneName = adrSplit[1];
    string paramName = adrSplit[2];
    
    // if there is a paramName/x (x,y,z,r,g,b ...)
    bool hasSpec = (adrSplit.size() > 3);
    string spec = "";
    if(hasSpec) {
        spec = adrSplit[3];
    }
    
    bool sX = (!hasSpec || spec == "x"); // because Lemur defaults to paramname/x - but also sends a touch intensity value on paranname/z
    
    ofAbstractParameter * p = &params;
    
    if(params.getEscapedName() == sceneName) {
        
        if(params.contains(paramName)) {
            lastOscUpdatedParam = paramName;
        
            p = &params.get(paramName);
        
            if(p->type()==typeid(ofParameter<int>).name() && m.getArgType(0)==OFXOSC_TYPE_INT32){
            
                if(sX) p->cast<int>() = m.getArgAsInt32(0);
            
        }else if(p->type()==typeid(ofParameter<float>).name() && m.getArgType(0)==OFXOSC_TYPE_FLOAT){
            
            if(sX) p->cast<float>() = m.getArgAsFloat(0);
            
        }else if(p->type()==typeid(ofParameter<double>).name() && m.getArgType(0)==OFXOSC_TYPE_DOUBLE){
            if(sX) p->cast<double>() = m.getArgAsDouble(0);
            
        }else if(p->type()==typeid(ofParameter<bool>).name() && (m.getArgType(0)==OFXOSC_TYPE_TRUE
                                                                 || m.getArgType(0)==OFXOSC_TYPE_FALSE
                                                                 || m.getArgType(0)==OFXOSC_TYPE_INT32
                                                                 || m.getArgType(0)==OFXOSC_TYPE_INT64
                                                                 || m.getArgType(0)==OFXOSC_TYPE_FLOAT
                                                                 || m.getArgType(0)==OFXOSC_TYPE_DOUBLE
                                                                 || m.getArgType(0)==OFXOSC_TYPE_STRING
                                                                 || m.getArgType(0)==OFXOSC_TYPE_SYMBOL)){
            if(sX) p->cast<bool>() = m.getArgAsBool(0);
            
        } else if(m.getArgType(0)==OFXOSC_TYPE_STRING){
            if(!hasSpec) {
                p->fromString(m.getArgAsString(0));
            }
        } else if(p->type() == typeid(ofParameter<ofVec2f>).name()) {
                
                if(spec == "x") p->cast<ofVec2f>() = ofVec2f(m.getArgAsFloat(0), p->cast<ofVec2f>().get().y);
                
                if(spec == "y") p->cast<ofVec2f>() = ofVec2f(p->cast<ofVec2f>().get().x, m.getArgAsFloat(0));
                
                
            } else if(p->type() == typeid(ofParameter<ofVec3f>).name()) {
                
                if(spec == "x") p->cast<ofVec3f>() = ofVec3f(m.getArgAsFloat(0), p->cast<ofVec3f>().get().y, p->cast<ofVec3f>().get().z);
                
                if(spec == "y") p->cast<ofVec3f>() = ofVec3f(p->cast<ofVec3f>().get().x, m.getArgAsFloat(0), p->cast<ofVec3f>().get().z);
                
                if(spec == "z") p->cast<ofVec3f>() = ofVec3f(p->cast<ofVec3f>().get().x, p->cast<ofVec3f>().get().y, m.getArgAsFloat(0));
                
            } else if(p->type() == typeid(ofParameter<ofColor>).name()) {
                
                if(spec == "r") p->cast<ofColor>() = ofColor(m.getArgAsFloat(0), p->cast<ofColor>().get().g, p->cast<ofColor>().get().b, p->cast<ofColor>().get().a);
                
                if(spec == "g") p->cast<ofColor>() = ofColor(p->cast<ofColor>().get().r, m.getArgAsFloat(0), p->cast<ofColor>().get().b, p->cast<ofColor>().get().a);
                
                if(spec == "b") p->cast<ofColor>() = ofColor(p->cast<ofColor>().get().r, p->cast<ofColor>().get().g, m.getArgAsFloat(0), p->cast<ofColor>().get().a);
                
                if(spec == "a") p->cast<ofColor>() = ofColor(p->cast<ofColor>().get().r, p->cast<ofColor>().get().g, p->cast<ofColor>().get().b, m.getArgAsFloat(0));
                
            }
        }
            
        }
}

void ContentScene::updateScene() {
    if(enabled) {
        update();
    }
}

void ContentScene::drawScene() {
    if(enabled) {
        ofPushMatrix();
        ofPushStyle();
        fbo.begin();
        draw();
        fbo.end();
        ofPopStyle();
        ofPopMatrix();
    }
}

void ContentScene::publishSyphonTexture() {
    if (enabled /*|| _force*/ ) {
        ofFill();
        syphonOut.publishTexture(&fbo.getTexture());
    }
}


void ContentScene::exit() {
    panel.saveToFile(name+"_settings.xml");
}