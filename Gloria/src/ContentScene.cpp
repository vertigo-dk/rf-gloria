//
//  ContentScene.cpp
//  Gloria
//
//  Created by Johan Bichel Lindegaard on 27/06/14.
//
//

#include "ContentScene.h"


void ContentScene::setupScene() {
    
    name = params.getName();
    enabled.addListener(this, &ContentScene::enableToggled);
    
    height = OUTHEIGHT;
    width  = OUTWIDTH;
    
    setup();
    
    ofAddListener(params.parameterChangedE(),this,&ContentScene::parameterChanged);
    
    ofFile file(name+"_settings.xml");
    panelSettingsPath = file.path();
    
    panel.setup(params, panelSettingsPath);
    
    //setSceneParameters();
    
    syphonOut.setName(name);
    syphonOutFixture.setName(name + "_LED");
    
    if(!file.exists()) {
        file.create();
        panel.saveToFile(file.path());
    } else {
        panel.loadFromFile(file.path());
    }
    
    if(enabled.get()) {
        enabled.set(true);
    }
}

void ContentScene::enableToggled(bool & e) {
    
    if(e) {
        
        // check if we have memory
        // max scenes enabled setting
        
        if(!fbo) fbo = outputManager->getFreeOutputFbo();
        
        if(fbo != nullptr) {
                        
            ofFbo::Settings settings;
            settings.width      = width;
            settings.height     = height;
            settings.numSamples = numSamples;
            settings.useDepth   = false;
            settings.internalformat = GL_RGBA;
            
            fbo->allocate(settings);
            
            fbo->begin();
            ofClear(0,0,0,0);
            fbo->end();
            
            if(hasFixtureOutput) {
                ofFbo::Settings fs;
                fs.width      = LEDOUTWIDTH;
                fs.height     = LEDOUTHEIGHT;
                fs.numSamples = 1;
                fs.useDepth   = false;
                fs.internalformat = GL_RGBA;
                
                fbo->fixtureFbo.allocate(fs);
                
                fbo->fixtureFbo.begin();
                ofClear(0,0,0,0);
                fbo->fixtureFbo.end();
            }

            enable();

        } else {
            enabled.set(false);
        }
        
    } else {
        disable();
        if(fbo != nullptr) {
            
            fbo->begin();
            ofClear(0,0,0,0);
            fbo->end();
            
            ofFill();
            syphonOut.publishTexture(&fbo->getTexture());
            
            if(hasFixtureOutput) {
                fbo->fixtureFbo.begin();
                ofClear(0,0,0,0);
                fbo->fixtureFbo.end();
                syphonOutFixture.publishTexture(&fbo->fixtureFbo.getTexture());
            }
            
            fbo->isFree = true;
            
            fbo = nullptr;
        }
    }
    
}

void ContentScene::parameterChanged( ofAbstractParameter & parameter ){
    
    // this check might not be good for BOOL clears for instance
    // but necesarry to preserve interpolation on Lemur sliders
    // clientside
    // what happens if multiple interactions occur before this?
    
    // TODO: update to use parameterSender
    
    /*if(lastOscUpdatedParam == parameter.getEscapedName()) {
        
    } else {
        oscSender->sendParameter(parameter);
    }*/
}

void ContentScene::parseSceneOscMessage(ofxOscMessage & m){

}

void ContentScene::updateScene() {
    if(enabled.get() && fbo) {
        update();
    }
}

void ContentScene::drawScene() {
    if(enabled.get() && fbo) {
        
        if(fbo) {
            ofPushMatrix();
            ofPushStyle();
            fbo->begin();
            draw();
            fbo->end();
            ofPopStyle();
            ofPopMatrix();
        }
        
        if(hasFixtureOutput) {
            
            ofPushMatrix();
            ofPushStyle();
            fbo->fixtureFbo.begin();
            drawFixtures();
            fbo->fixtureFbo.end();
            ofPopStyle();
            ofPopMatrix();
        }
    }
    
    
}

void ContentScene::publishSyphonTexture() {
    if (enabled.get()) {
        
        if(fbo) {
            ofFill();
            syphonOut.publishTexture(&fbo->getTexture());
            
            if(hasFixtureOutput) {
                syphonOutFixture.publishTexture(&fbo->fixtureFbo.getTexture());
            }
        }
        

    }
}


void ContentScene::exit() {
    
    ofFile file;
    file.open(panelSettingsPath);
    ofBuffer buf; buf.set("");
    file.writeFromBuffer(buf);
    
    panel.saveToFile(panelSettingsPath);
}
