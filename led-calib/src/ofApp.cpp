#include "ofApp.h"

//--------------------------------------------------------------
void ofApp::setup(){
    ofBackgroundGradient(ofColor(20), ofColor(40));
    
    mixingShader.load("shaders/mixingShader");
    xyzMap.allocate(1024, 768, OF_IMAGE_COLOR);
    
    for(int y=0; y<xyzMap.getHeight() ; y++){
        for(int x=0; x<xyzMap.getWidth() ; x++){
            xyzMap.setColor(x, y, ofFloatColor(x / 1024.0, y / 768.0, sin(x/500.0)) );
        }
    
    }
    xyzMap.update();

    input1.load("img.jpg");
    
    gui.setup();
    gui.add(verticalAdd.setup("verticalAdd", 1.0, 0.0, 1.0));
    gui.add(horizontalAdd.setup("horizontalAdd", 1.0, 0.0, 1.0));
}

//--------------------------------------------------------------
void ofApp::update(){
    
}

//--------------------------------------------------------------
void ofApp::draw(){
    mixingShader.begin();
    
    mixingShader.setUniformTexture("xyzMap", xyzMap.getTexture(), 1);
    mixingShader.setUniformTexture("tex1", input1.getTexture(), 2);
    mixingShader.setUniformTexture("tex2", input1.getTexture(), 3);
    
    mixingShader.setUniform1f("verticalAdd", verticalAdd);
    mixingShader.setUniform1f("horizontalAdd", horizontalAdd);
    
    xyzMap.draw(0,0);
    
    mixingShader.end();
    
    
    gui.draw();

}

//--------------------------------------------------------------
void ofApp::keyPressed(int key){
    if(key == ' '){
        mode = !mode;
    }
    
    if(key =='r'){
        mixingShader.load("shaders/mixingShader");
    }
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

}

//--------------------------------------------------------------
void ofApp::mouseEntered(int x, int y){

}

//--------------------------------------------------------------
void ofApp::mouseExited(int x, int y){

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
