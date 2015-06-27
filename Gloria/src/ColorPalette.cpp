//
//  ColorPalette.cpp
//  Gloria
//
//  Created by Jonas Jongejan on 21/06/15.
//
//

#include "ColorPalette.h"

void ColorPalette::setup(){
    name = "ColorPalette";
    oscAddress = "/colorPalette";

    
    
    fbo.allocate(100, 100);

}

void ColorPalette::update(){
  //    fbo.clear();
     fbo.begin();
    ofClear(0);
        syphonIn->draw(0, 0, 100, 100);
     fbo.end();

    fbo.getTexture().readToPixels(pixels);
    img.setFromPixels(pixels);

}

void ColorPalette::draw(){
    img.draw(0,0,OUTWIDTH,OUTHEIGHT);
   //
    
    vector<ofColor> palette = ofxColorExtractor::getColorPalette(pixels, 2);
    
    img.draw(0,0,OUTWIDTH,OUTHEIGHT);
    
    ofPushMatrix();
    for(int i=0;i<palette.size();i++){
        ofSetColor(palette[i]);
        ofDrawRectangle(0,0,200,200);
        ofTranslate(200, 0);
    }
    ofPopMatrix();
}


void ColorPalette::parseOscMessage(ofxOscMessage *m){
    
}
void ColorPalette::setGui(){
    
}