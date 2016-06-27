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
};

class OutputManager {
public:
    
    OutputManager() {
        for(int i=0; i<MAX_SCENES; i++) {
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
            if(fbo->isFree) return fbo;
        }
        return nullptr;
    }
    
private:
    vector<OutputFbo *> outputFbos;
    
};