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
    if(updatingParameter) return;
    oscSender->sendParameter(parameter);
}


void ContentScene::setSceneGui(){
}

void ContentScene::parseSceneOscMessage(ofxOscMessage & m){
    
    bool isScene = false;
    //string loweraddr = ofToLower(m.getAddress());
	vector<string> adrSplit = ofSplitString(m.getAddress(), "/");
	string rest = ofSplitString(m.getAddress(), "/"+adrSplit[1])[1];
	
    //cout<<adrSplit[1]<<"   "<<rest<<endl;
    
    if(ofToLower(adrSplit[1]) == "scene"+ofToString(index) || ofToLower("/"+adrSplit[1]) == oscAddress || ofToLower(adrSplit[1]) == ofToLower(name)) {
        
        adrSplit[1] = name;
        
        isScene = true;
    } /*else {
        
        // check the underscore formatting
        
        adrSplit = ofSplitString(loweraddr, "_");
        if(adrSplit.size() > 1) {
            
        rest = adrSplit[1];
            
            // also allow full name here
            
        if (adrSplit[0] == "scene"+ofToString(index) || adrSplit[0] == oscAddress || adrSplit[0] == ofToLower(name)) {
            
            // have it match the original / formatting
            rest = "/" + rest;
            isScene = true;
        }
        }
    }*/
    
    
    if(isScene) {
        
        updatingParameter = true;
        
        oscReceiver->getParameter(params);
        ofAbstractParameter * p = &params;
        
        /// potential issue with lemur sending address/z val
        /// anything here goes address/fesr/sfdgfsdg/fdgsdfg
        /// still hits address
        
        for(unsigned int i=0;i<adrSplit.size();i++){
            
            if(p) {
                if(adrSplit[i]==p->getEscapedName()){
                    if(p->type()==typeid(ofParameterGroup).name()){
                        if(adrSplit.size()>=i+1){
                            p = &static_cast<ofParameterGroup*>(p)->get(adrSplit[i+1]);
                        }
                    }else if(p->type()==typeid(ofParameter<int>).name() && m.getArgType(0)==OFXOSC_TYPE_INT32){
                        p->cast<int>() = m.getArgAsInt32(0);
                    }else if(p->type()==typeid(ofParameter<float>).name() && m.getArgType(0)==OFXOSC_TYPE_FLOAT){
                        p->cast<float>() = m.getArgAsFloat(0);
                    }else if(p->type()==typeid(ofParameter<double>).name() && m.getArgType(0)==OFXOSC_TYPE_DOUBLE){
                        p->cast<double>() = m.getArgAsDouble(0);
                    }else if(p->type()==typeid(ofParameter<bool>).name() && (m.getArgType(0)==OFXOSC_TYPE_TRUE
                                                                             || m.getArgType(0)==OFXOSC_TYPE_FALSE
                                                                             || m.getArgType(0)==OFXOSC_TYPE_INT32
                                                                             || m.getArgType(0)==OFXOSC_TYPE_INT64
                                                                             || m.getArgType(0)==OFXOSC_TYPE_FLOAT
                                                                             || m.getArgType(0)==OFXOSC_TYPE_DOUBLE
                                                                             || m.getArgType(0)==OFXOSC_TYPE_STRING
                                                                             || m.getArgType(0)==OFXOSC_TYPE_SYMBOL)){
                        p->cast<bool>() = m.getArgAsBool(0);
                    }else if(m.getArgType(0)==OFXOSC_TYPE_STRING){
                        p->fromString(m.getArgAsString(0));
                    }
                }
            }
        }
        
        
        // backward compatibility with lemur style adresses
        
        
        if(params.contains(adrSplit[2])) {
            
            string type = params.get(adrSplit[2]).type();
            
            if(typeid(ofParameter<ofVec2f>).name() == type) {
                
                ofParameter<ofVec2f> * p = &params.get(adrSplit[2]).cast<ofVec2f>();
                
                if(adrSplit.size() > 2) {
                
                    if(adrSplit[3] == "x") {
                        p->set(ofVec2f(m.getArgAsFloat(0), p->get().y));
                    }
                    else if(adrSplit[3] == "y") {
                        p->set(ofVec2f(p->get().x, m.getArgAsFloat(0)));
                    }
                }
                
            } else if(typeid(ofParameter<ofVec3f>).name() == type) {
                
                ofParameter<ofVec3f> * p = &params.get(adrSplit[2]).cast<ofVec3f>();
                
                if(adrSplit[3] == "x") {
                    p->set(ofVec3f(m.getArgAsFloat(0), p->get().y, p->get().z));
                }
                else if(adrSplit[3] == "y") {
                    p->set(ofVec3f(p->get().x, m.getArgAsFloat(0), p->get().z));
                }
                else if(adrSplit[3] == "z") {
                    p->set(ofVec3f(p->get().x, p->get().y, m.getArgAsFloat(0)));
                }
            }
        }
            updatingParameter = false;
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