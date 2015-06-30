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
    
    ofSetWindowTitle("Gloria 2015");
    
    
    
    
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
    selectSyphonInput(dirIdx);
    
    mapping = new Mapping();
    mapping->load("mapping.xml", "input1.svg");
    
    // effects scenes
    // Set up the scenes, all scenes is a subclass of SceneContent, don't call draw, setup and update directly it is taken care of thorugh the scene.
    
    scenes.push_back(new FluidScene());
    scenes.push_back(new QuickTrail());
    scenes.push_back(new Triangles());
    scenes.push_back(new PerlinWaves());
    scenes.push_back(new BasicParticles());
    //scenes.push_back(new ColorPalette());
    scenes.push_back(new ChaoticAttractor());
    scenes.push_back(new PetriDish());
    scenes.push_back(new CurlyFur());
    
    
    // we composite in millumin via syphon, uncomment this to compsite locally + section where scenes are drawn into fbo's
    /*ofFbo::Settings fboSettings;
     fboSettings.height = OUTHEIGHT;
     fboSettings.width  = OUTWIDTH;
     fboSettings.numSamples = 4;
     fboSettings.useDepth   = false;
     
     fboOut.allocate(fboSettings);
     fboOut.setUseTexture(true);
     
     fboOut.begin();
     ofBackground(0,0,0,255);
     fboOut.end();
     */
    
    
    for(int i=0; i<scenes.size(); i++) {
        scenes[i]->mapping  = mapping;
        scenes[i]->syphonIn = syphonIn;
        scenes[i]->oscSender = &oscSenderOne;
        scenes[i]->oscReceiver = &oscReceiver;
        scenes[i]->setupScene(OUTWIDTH, OUTHEIGHT, i);
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
    
    while(oscReceiver.hasWaitingMessages()){
        
        // get the next message
        ofxOscMessage m;
        oscReceiver.getNextMessage(&m);
        
        //cout<<m.getAddress()<<endl;
        for(int i=0; i<scenes.size();i++) {
            scenes[i]->parseSceneOscMessage(m);
        }
    }
    
    // Scenes
    for(int i=0; i<scenes.size(); i++) {
        scenes[i]->updateScene();
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
    

    
    for(int i=0; i<syphonInputs.size(); i++) {
       ofSetColor(255,255,255);
    
        ofPushMatrix();{
            ofTranslate(20, syphonRowMargin+i*(syphonRowHeight+syphonRowMargin));
            if(i*syphonRowHeight > ofGetHeight()-100){
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
        
        
    }
    
    
    mainGui.draw();
    
    for(int i=0; i<scenes.size(); i++) {
        // maybe we need to have this here and sync osc with parameters on top level
        scenes[i]->panel.draw();
    }
    
    
    for(int i=0; i<scenes.size(); i++) {
        if(scenes[i]->enabled) {
            scenes[i]->publishSyphonTexture();
        }
    }
    
    //syphonOut.publishTexture(&fboOut.getTexture());
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

void ofApp::selectSyphonInput(int input){
    if(input > syphonInputs.size() - 1)
        input = 0;
    
    if(directory.isValidIndex(input)){
        /*syphonIn->setServerName(directory.getServerList()[dirIdx].serverName);
         syphonIn->setApplicationName(directory.getServerList()[dirIdx].appName);*/
        
        dirIdx = input;
        //syphonIn = &syphonInputs[dirIdx];
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
    for(int i=0; i<syphonInputs.size(); i++) {
        if(x < syphonRowWidth+20){
            if(syphonRowMargin+i*(syphonRowHeight+syphonRowMargin) < y && syphonRowMargin+(i+1)*(syphonRowHeight+syphonRowMargin) > y){
                selectSyphonInput(i);
            }
        }
    }
}

//--------------------------------------------------------------
void ofApp::mouseReleased(int x, int y, int button){
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
        delete scenes[i];
        
    }
    
    
    settings.saveFile("appSettings.xml");
    mapping->save();
    
    delete syphonIn;
    delete mapping;
}

