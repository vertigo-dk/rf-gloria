//
//  Output.hpp
//  Gloria
//
//  Created by Johan Bichel Lindegaard on 6/27/16.
//
//

#pragma once

#include "ofMain.h"
#include "Defines.h"

class OutputFbo : public ofFbo {
public:
    bool isFree;
    
    OutputFbo() {
        isFree = true;
    }
    
    ofFbo fixtureFbo;
};

class OutputManager {
public:
    
    OutputManager(int max_fbos=MAX_SCENES) {
        for(int i=0; i<max_fbos; i++) {
            outputFbos.push_back(new OutputFbo());
        }
    }

    ~OutputManager() {
        for( auto fbo : outputFbos) {
            delete fbo;
        }
        outputFbos.clear();
    }
    
    OutputFbo * getFreeOutputFbo() {
        for( auto fbo : outputFbos) {
            if(fbo->isFree) {
                fbo->isFree = false;
                return fbo;
            }
        }
        return nullptr;
    }
    
private:
    vector<OutputFbo *> outputFbos;
    vector<OutputFbo *> outputFixtureFbos;
    
};