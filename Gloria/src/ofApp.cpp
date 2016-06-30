#include "ofApp.h"

int syphonRowHeight = 70;
int syphonRowMargin = 32;
float syphonRowWidth = syphonRowHeight * 4.75;

void ofApp::setup() {
    
    ofEnableAlphaBlending();
    oscReceiver.setup(OSCRECEIVEPORT);
    oscSenderOne.setup(OSCCLIENTONE, OSCSENDPORT);
    
    ofSetLogLevel(OF_LOG_NOTICE);
    ofSetFrameRate(TARGET_FRAMERATE);
    ofSetVerticalSync(true);
    glEnable(GL_LINES);
    
    ofSetWindowTitle("Gloria 2016");
    
    fadeManager = make_shared<ofxParameterFader>();
    
    ofAddListener(globalParams.parameterChangedE(), this, &ofApp::paramsChanged);
    
    directory.setup();
    
    //register for our directory's callbacks
    ofAddListener(directory.events.serverAnnounced, this, &ofApp::serverAnnounced);
    ofAddListener(directory.events.serverUpdated, this, &ofApp::serverUpdated);
    ofAddListener(directory.events.serverRetired, this, &ofApp::serverRetired);
    dirIdx = -1;
    
    syphonIn = new ofxSyphonClient();
    syphonIn->setup();
    
    settings.loadFile("appSettings.xml");
    dirIdx = settings.getValue("syphonInputSelected", -1);
    
    mapping = new Mapping();
    mapping->load("mapping.xml", "input1.svg");
    
    mapping->loadFixtures("fixtures.xml", "LedInput.svg");
    
    for(auto s : scenes) {
        
        s->outputManager = &outputManager;
        s->mapping  = mapping;
        s->syphonIn = syphonIn;
        s->setupScene();
    }
    
    /*globalParameters.add(drawMapping.set("Draw mapping", true));
    
    mainGui.setup(globalParameters);
    mainGui.setName("Gloria");
    */
    
    for(int i=0; i<scenes.size(); i++) {
        // layout scene gui panels horizontally
        
        int x = settings.getValue(scenes[i]->name+"_panel_pos_x", 0);
        int y = settings.getValue(scenes[i]->name+"_panel_pos_y", 1);
        
        scenes[i]->panel.setPosition(x,y);
    }
    
    // Syphon merger (bluemac hack)
    syphonMerger.setName("BlueMac");
    syphonMergerFbo.allocate(OUTWIDTH, OUTHEIGHT);
}

void ofApp::serverAnnounced(ofxSyphonServerDirectoryEventArgs &arg)
{
    for( auto& dir : arg.servers ){
        ofLogNotice("ofxSyphonServerDirectory Server Announced")<<" Server Name: "<<dir.serverName <<" | App Name: "<<dir.appName;
        
        bool found = false;
        
        
        for(int i=0; i<syphonInputs.size(); i++) {
            if(syphonInputs[i].getServerName() == dir.serverName && syphonInputs[i].getApplicationName() == dir.appName) {
                
                syphonInputs[i].setup();
                syphonInputs[i].set(dir.serverName, dir.appName);
                
                found = true;
            }
        }
        if(!found) {
            ofxSyphonClient client;
            client.setup();
            client.set(dir.serverName, dir.appName);
            
            syphonInputs.push_back(client);
        }
    }
    //dirIdx = 0;
    selectSyphonInput(dirIdx);
}

void ofApp::serverUpdated(ofxSyphonServerDirectoryEventArgs &arg)
{
    for( auto& dir : arg.servers ){
        ofLogNotice("ofxSyphonServerDirectory Server Updated")<<" Server Name: "<<dir.serverName <<" | App Name: "<<dir.appName;
        
        for(int i=0; i<syphonInputs.size(); i++) {
            if(syphonInputs[i].getServerName() == dir.serverName && syphonInputs[i].getApplicationName() == dir.appName) {
                
                syphonInputs[i].setup();
                syphonInputs[i].set(dir.serverName, dir.appName);
            }
        }
        
    }
    //dirIdx = 0;
}

void ofApp::serverRetired(ofxSyphonServerDirectoryEventArgs &arg)
{
    for( auto& dir : arg.servers ){
        ofLogNotice("ofxSyphonServerDirectory Server Retired")<<" Server Name: "<<dir.serverName <<" | App Name: "<<dir.appName;
        
        for(int i=0; i<syphonInputs.size(); i++) {
            if(syphonInputs[i].getServerName() == dir.serverName && syphonInputs[i].getApplicationName() == dir.appName) {
            }
        }
        
        
    }
    //dirIdx = 0;
}

//--------------------------------------------------------------
void ofApp::update() {
    
    fadeManager->update();
    
    while(oscReceiver.hasWaitingMessages()){
        
        // get the next message
        ofxOscMessage m;
        oscReceiver.getNextMessage(m);
        
        for(auto s : scenes) {
            fadeManager->parseOscMessageForParameterGroup(m, &s->params);
        }
        
        //fadeManager->parseOscMessageForParameterGroup(m, &globalParams);

        // When this has been tested and lemur updated, remove code above
        
    }
    
    // Scenes
    for( auto s : scenes) {
        s->updateScene();
    }
}

void ofApp::draw() {
    // Syphon merger
    syphonMergerFbo.begin();
    ofClear(0, 0, 0);
    ofSetColor(255);
    ofFill();
    for(int i=0; i<syphonInputs.size(); i++) {
        if(syphonInputs[i].getApplicationName() == "Black Syphon"){
            if(syphonInputs[i].getServerName() == "DeckLink SDI 4K (2)"){
                syphonInputs[i].draw(0, 0, OUTWIDTH/2, OUTHEIGHT);
            }
            if(syphonInputs[i].getServerName() == "DeckLink SDI 4K (1)"){
                syphonInputs[i].draw(OUTWIDTH/2, 0, OUTWIDTH/2, OUTHEIGHT);
            }
        }
    }
    syphonMergerFbo.end();
    syphonMerger.publishTexture(&syphonMergerFbo.getTexture());
    
    // Draw scene fbo's
    ofPushStyle();
    ofNoFill();
    for(int i=0; i<scenes.size(); i++) {
        ofSetColor(255);
        ofFill();
        ofPushStyle();
        scenes[i]->drawScene();
        ofPopStyle();
    }
    ofPopStyle();
    
    ofDisableDepthTest();
    ofBackground(0, 0, 0);
    ofSetColor(255,255,255,255);
    
    float dX = 10;
    float dY = 10;
    float rowHeight = 120;
    float colWidth = 400;
    
    
    for(auto s : scenes) {
        
        ofSetColor(255,255,255,255);
        ofPushMatrix();
        ofTranslate(dX,dY);
        
        for(auto syphon : syphonInputs) {
            if(syphon.getServerName() == s->name) {
                
                ofSetColor(255,255,255,255);
                if(syphonIn->getServerName() == syphon.getServerName() && syphonIn->getApplicationName() == syphon.getApplicationName()) {
                    ofSetColor(255,20,20,255);
                }
                
                ofDrawBitmapString(s->name, 0, 5);

                ofSetColor(255,255,255,255);

                syphon.draw(0, 10, 400, 100);
                
                
                
                if(mRelease) {
                    if(ofRectangle(dX,dY,400,100).inside(mReleasePos)) {
                        
                        
                        
                        selectSyphonInput(syphon);
                        mRelease = false;
                    }
                }
                
            } else if (syphon.getServerName() == s->name + "_LED") {
                
                ofSetColor(255,255,255,255);
                ofDrawBitmapString("LED", 400, 5);
                syphon.draw(400, 10, 72,128);
                
            }
        }
        ofPopMatrix();
        dY += rowHeight;
        
    }
    
    for(auto syphon : syphonInputs) {
        
        if(syphon.getApplicationName().find("Gloria") == std::string::npos) {
         
            ofPushMatrix();
            ofTranslate(dX,dY);
            
            ofSetColor(255,255,255,255);
            if(syphonIn->getServerName() == syphon.getServerName() && syphonIn->getApplicationName() == syphon.getApplicationName()) {
                ofSetColor(255,180,180,255);
            }
            ofDrawBitmapString(syphon.getApplicationName() + ":" + syphon.getServerName() , 0, 5);
            
            ofSetColor(255,255,255,255);
            syphon.draw(0, 10, syphon.getWidth()/15, syphon.getHeight()/15);
            
            if(mRelease) {
                if(ofRectangle(dX,dY,400+dX,100+dY).inside(mReleasePos)) {
                    selectSyphonInput(syphon);
                    mRelease = false;
                }
            }
            
            ofPopMatrix();
            dY += rowHeight;
        }
    }
    
    float scale = 0.08;
    /*ofPushMatrix();{
        ofTranslate(ofGetWidth()-scale*OUTWIDTH-40, 40);
        
        ofSetColor(255,255,255,255);
        ofNoFill();
        ofSetLineWidth(1);
        
        for(int i=0; i<scenes.size(); i++) {
            
            ofPushMatrix();
            
            ofTranslate(0, (i*scale*scenes[i]->fbo.getHeight())+30);
            
            if(scenes[i]->enabled){
                ofSetColor(255);
            } else {
                ofSetColor(255,0,0,100);
            }
            
            ofDrawRectangle(-1, -1, scenes[i]->fbo.getWidth()*scale+2, scenes[i]->fbo.getHeight()*scale+2);
            // fboOut.draw(0, 0);
            ofSetColor(255,255,255,255);
            if(scenes[i]->enabled) {
                scenes[i]->fbo.draw(0,0, scenes[i]->fbo.getWidth()*scale, scenes[i]->fbo.getHeight()*scale);
            }
            ofSetColor(255);
            
            ofDrawBitmapString(scenes[i]->name, ofPoint(0,-3));
            
            if(drawGuide) {
                //ofSetColor(255,255,255,96);
                ofPushMatrix();
                ofScale(scale, scale);
                drawGrid();
                debugDraw();
                ofPopMatrix();
            }
            
            ofPopMatrix();
        }
        
    }ofPopMatrix();*/
    
    ofSetColor(255);
    ofDrawBitmapString("FPS: " + ofToString(ofGetFrameRate()), ofGetWidth()-200, 20);
    
    /*if(mapping->selectedCorner) {
     ofDrawBitmapString("Selected Corner: " + ofToString(mapping->selectedCorner->uid) + " pos: " + ofToString(mapping->selectedCorner->pos), ofGetWidth()-600, 20);
     }*/
    
    /*for(int i=0; i<syphonInputs.size(); i++) {
       ofSetColor(255,255,255);
    
        ofPushMatrix();{
            ofTranslate(20, syphonRowMargin+i*(syphonRowHeight+syphonRowMargin));
            if(i*(syphonRowHeight+syphonRowMargin) > ofGetHeight()-100){
                ofTranslate(syphonRowWidth+20, -ofGetHeight()+40);
            }
            
            syphonInputs[i].draw(0, 10, syphonRowWidth, syphonRowHeight);
            
            if(syphonInputs[i].getApplicationName() == "GloriaDebug"){
                ofSetColor(150, 150, 255);
            }

            if(dirIdx == i) {
                ofSetColor(0,255,0);
            }
            
            ofDrawBitmapString(ofToString(i) + ": " + syphonInputs[i].getApplicationName() + " " + syphonInputs[i].getServerName(), 0, 0);
            
        }ofPopMatrix();
        
        
    }*/
    
    
    mainGui.draw();
    
    for(int i=0; i<scenes.size(); i++) {
        // maybe we need to have this here and sync osc with parameters on top level
        scenes[i]->panel.draw();
    }
    
    
    for(int i=0; i<scenes.size(); i++) {
        scenes[i]->publishSyphonTexture();
    }
    
    //syphonOut.publishTexture(&fboOut.getTexture());
    
    
    mRelease = false;

}

//------------------------------------------------------------
void ofApp::debugDraw() {
    mapping->debugDraw();
}

void ofApp::drawGrid() {
    ofSetLineWidth(1);
    for(int i =0; i<mapping->triangles.size();i++) {
        mapping->triangles[i]->mesh.drawWireframe();
    }
}

void ofApp::selectSyphonInput(ofxSyphonClient client){
    for(int i=0; i<syphonInputs.size(); i++) {
        if(syphonInputs[i].getApplicationName() == client.getApplicationName() &&
           syphonInputs[i].getServerName() == client.getServerName()) {
            selectSyphonInput(i);
            return;
        }
    }
}


void ofApp::selectSyphonInput(int input){
    //if(input > syphonInputs.size() - 1)
    //    input = 0;
    
    if(directory.isValidIndex(input)){
        /*syphonIn->setServerName(directory.getServerList()[dirIdx].serverName);
         syphonIn->setApplicationName(directory.getServerList()[dirIdx].appName);*/
        
        dirIdx = input;
        syphonIn = &syphonInputs[dirIdx];
        for(int i=0; i<scenes.size(); i++) {
            scenes[i]->syphonIn = &syphonInputs[input];
        }
    }
}

//--------------------------------------------------------------
void ofApp::keyPressed(int key){
    
    if(key == 'i') {
        selectSyphonInput(dirIdx+1);
    }
    if(key == 'c') {
        if(lastChangedParam != nullptr) {
            ofGetWindowPtr()->setClipboardString(ofxParameterFader::getOscAddressForParameter(*lastChangedParam));
        }
    }
    
    /*if(key == 'n') {
        mapping->nextCorner();
     }
     if(key == 'm') {
        mapping->prevCorner();
     }
     if(mapping->selectedCorner) {
     if(key == OF_KEY_UP) {
        mapping->selectedCorner->pos.z += 1;
        mapping->updateMeshes();
     }
     if(key == OF_KEY_DOWN) {
        mapping->selectedCorner->pos.z -= 1;
        mapping->updateMeshes();
     }
     }*/
}

//--------------------------------------------------------------
void ofApp::keyReleased(int key){
}

//--------------------------------------------------------------
void ofApp::mouseMoved(int x, int y ){
}

//--------------------------------------------------------------
void ofApp::mouseDragged(int x, int y, int button){
}

//--------------------------------------------------------------
void ofApp::mousePressed(int x, int y, int button){
    

}

//--------------------------------------------------------------
void ofApp::mouseReleased(int x, int y, int button){
    mRelease = true;
    mReleasePos = ofPoint(x,y);
}

//--------------------------------------------------------------
void ofApp::windowResized(int w, int h){
}

//--------------------------------------------------------------
void ofApp::gotMessage(ofMessage msg){
}

//--------------------------------------------------------------
void ofApp::dragEvent(ofDragInfo dragInfo){
}

//--------------------------------------------------------------
void ofApp::exit()
{

    settings.setValue("syphonInputSelected", dirIdx);
    
    for(int i=0;i<scenes.size();i++) {
        
        settings.setValue(scenes[i]->name+"_panel_pos_x", scenes[i]->panel.getPosition().x);
        settings.setValue(scenes[i]->name+"_panel_pos_y", scenes[i]->panel.getPosition().y);
        scenes[i]->exit();
    }
    
    settings.saveFile("appSettings.xml");
    mapping->save();
    
    //delete syphonIn;
    delete mapping;
}

