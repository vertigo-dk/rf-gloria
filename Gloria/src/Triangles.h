//
//  LampWalker.h
//  gloriamap
//
//  Created by Jonas Jongejan on 30/06/13.
//
//

#pragma once

#include "ofMain.h"
#include "ContentScene.h"
#include "ofxAutoReloadedShader.h"

class SubTriangle : public InputTriangle{
public:
    
    vector<SubTriangle*> subTriangles;
    float age;
    SubTriangle * parentTriangle;
    bool die;
    int level;
    float drawLevel;
    float drawLevelGoal;
    float noise, noiseSeed;
    
    float ageDifference;
    
    SubTriangle(){
        ageDifference = ofRandom(0.7,0.0);
        age = 0;
        
        drawLevel = 0;
        drawLevelGoal = 2;
    }
    
    ofVec3f normal(){

        ofVec3f n = (getPos(1) - getPos(0)).cross(( getPos(2) - getPos(0) )).getNormalized();
        if(n.z < 0){
            n *= -1.;
        }
        return  n;
    }
    
    int numTriangles(){
        int ret = 1;
        for(int i=0;i<subTriangles.size();i++){
            ret += subTriangles[i]->numTriangles();
        }
        return ret;
    }
    
    void update(){
        
        //if(level == 0){
        /*if(drawLevelGoal > drawLevel){
            drawLevel += 1.0/MAX(10,ofGetFrameRate()) * ageDifference;
        }
        if(drawLevelGoal < drawLevel){
            drawLevel -= 1.0/MAX(10,ofGetFrameRate()) * ageDifference;
        }*/
        drawLevel = drawLevelGoal;
//        drawLevel += (drawLevelGoal - drawLevel)*0.2 *1.0/MAX(10,ofGetFrameRate()) * ageDifference;
       // }
        
        for(int i=0;i<3;i++){
            ofVec3f anchor;
            if(corners[i]->anchor1){
                anchor = corners[i]->anchor1->pos * corners[i]->anchorRatio;
                anchor += corners[i]->anchor2->pos * (1-corners[i]->anchorRatio);
                anchor *= ofVec3f(0,0,1);
            }
            ofVec3f p =corners[i]->origPos + anchor;
            
           /* float a = 0;
            if(drawLevel >= level-2 ) {
                if(drawLevel >= level-1){
                    a = 1;
                } else {
                    a = ofClamp(drawLevel-floorf(drawLevel), 0, 1);
                }

            }
           
            p.z += corners[i]->randomSeed.z  * 1000 * a;
            */
            
            corners[i]->pos = p;
            
            /*if(level <= drawLevel+1){
                float f = MIN(1,MAX(0,drawLevel-level));
                ofVec3f dest = (p + ofVec3f(0,0,100. * f * corners[i]->randomSeed.z ));
                
                corners[i]->pos += (dest - corners[i]->pos )*0.1;
               // corners[i]->pos.z = ofRandom(-100,100);


            } else {
//                corners[i]->pos = p;
            }*/
            

       //     corners[i]->pos = dest ;
        }
        
        int c = subTriangles.size();
        for(int i=0;i<c;i++){
           // subTriangles[i]->drawLevel = drawLevel;
          /*  if(subTriangles[i]->drawLevel > drawLevel && drawLevel > drawLevelGoal){
                drawLevel = subTriangles[i]->drawLevel;
            }*/
            subTriangles[i]->drawLevelGoal = drawLevelGoal;
            subTriangles[i]->noise = noise;
            subTriangles[i]->noiseSeed = noiseSeed;
            subTriangles[i]->update();
        }
        age += ageDifference*1.0/ofGetFrameRate();
    }
    
    ofVec3f getPos(int i){
        
        
        float f = MIN(1,MAX(0,drawLevel-level));
        /*srand(corners[i]->randomSeed.x*100 + noiseSeed*100.);
        float rx = (rand()%100)*noise * corners[i]->randomSeed.x;

        srand(corners[i]->randomSeed.y*100+ noiseSeed*100.);
       float ry = (rand()%100)*noise * corners[i]->randomSeed.y;
        */
        return corners[i]->getPos(f);// + ofVec2f(rx, ry);
    }
    
    float getLowestAge(){
        float _age = age;
        for(int i=0;i<subTriangles.size();i++){
            if(_age > subTriangles[i]->getLowestAge()){
                _age = subTriangles[i]->getLowestAge();
            }
        }
        return _age;
    }
    
    vector<SubTriangle*> getLowestLevelTriangles(){
        vector<SubTriangle*> ret;
        
        if(subTriangles.size() > 0){
            for(int i=0;i<subTriangles.size();i++){
                vector<SubTriangle*> sub = subTriangles[i]->getLowestLevelTriangles();
                ret.insert(ret.end(), sub.begin(), sub.end());
            }
        } else {
            ret.push_back(this);
        }
        
        return ret;
    }
    
    int getLevels(){
        int ret = 1;
        for(int i=subTriangles.size()-1;i>=0;i--){
            if(ret < 1+subTriangles[i]->getLevels()){
                ret = 1+subTriangles[i]->getLevels();
            }
        }
        return ret;
    }
    
    float triangleSize(){
        float a = (corners[1]->pos - corners[0]->pos).length();
        float b = (corners[2]->pos - corners[0]->pos).length();
        float c = (corners[2]->pos - corners[1]->pos).length();
        
        float s = (a+b+c)/2.0;
        return sqrt(s*(s-a)*(s-b)*(s-c));
//       return  0.5 * (corners[1]->pos - corners[0]->pos).length() * (corners[2]->pos - corners[0]->pos).length();
    }
    
    float getSmallestSize(){
        float _size = triangleSize();
        for(int i=0;i<subTriangles.size();i++){
            if(_size > subTriangles[i]->triangleSize()){
                _size = subTriangles[i]->triangleSize();
            }
        }
        return _size;
    }
    
    void divide(float minSize, int _level){
        
        if(triangleSize() > minSize){
            //Division corners

            Corner * d1 = corners[0]->division[corners[1]];
            Corner * d2 = corners[0]->division[corners[2]];
            Corner * d3 = corners[1]->division[corners[2]];
            
            
            //Z
            /* d1->pos.z += ofRandom(-6,6);
             d2->pos.z += ofRandom(-6,6);
             d3->pos.z += ofRandom(-6,6);
             */
            //Create the 4 new triangles
            {
                SubTriangle * newTriangle = new SubTriangle();
                newTriangle->level = _level;
                newTriangle->corners[0] = corners[0];
                newTriangle->corners[1] = corners[0]->division[corners[1]];
                newTriangle->corners[2] = corners[0]->division[corners[2]];
                
                newTriangle->parentTriangle = this;
                
                
                
                subTriangles.push_back(newTriangle);
            }
            
            {
                SubTriangle * newTriangle = new SubTriangle();
                newTriangle->level = _level;
                newTriangle->corners[0] = corners[1];
                newTriangle->corners[1] = corners[1]->division[corners[2]];
                newTriangle->corners[2] = corners[1]->division[corners[0]];
                
                newTriangle->parentTriangle = this;
                
                
                
                subTriangles.push_back(newTriangle);
            }
            {
                SubTriangle * newTriangle = new SubTriangle();
                newTriangle->level = _level;
                newTriangle->corners[0] = corners[2];
                newTriangle->corners[1] = corners[2]->division[corners[0]];
                newTriangle->corners[2] = corners[2]->division[corners[1]];
                
                newTriangle->parentTriangle = this;
                
                
                
                subTriangles.push_back(newTriangle);
            }
            {
                SubTriangle * newTriangle = new SubTriangle();
                newTriangle->level = _level;
                newTriangle->corners[0] = corners[1]->division[corners[2]];
                newTriangle->corners[1] = corners[0]->division[corners[1]];
                newTriangle->corners[2] = corners[0]->division[corners[2]];
                
                newTriangle->parentTriangle = this;
                
                
                
                subTriangles.push_back(newTriangle);
            }
            
            
            //Update joined corners
            for(int i=0;i<3;i++){
                for(int j=0;j<2;j++){
                    corners[i]->joinedCorners.erase(std::remove(corners[i]->joinedCorners.begin(),
                                                                corners[i]->joinedCorners.end(),
                                                                corners[(int)ofWrap(j+i+1,0,3)]),
                                                    corners[i]->joinedCorners.end());
                }
                
                corners[i]->joinedCorners.push_back(corners[i]->division[corners[(int)ofWrap(i+1,0,3)]]);
                corners[i]->joinedCorners.push_back(corners[i]->division[corners[(int)ofWrap(i+2,0,3)]]);
            }
            
            d1->joinedCorners.push_back(d2);
            d1->joinedCorners.push_back(d3);
            
            d2->joinedCorners.push_back(d3);
            d2->joinedCorners.push_back(d1);
            
            d3->joinedCorners.push_back(d1);
            d3->joinedCorners.push_back(d2);
        }
    }
};

class Triangles : public ContentScene {
    
public:
    
    

    
    ofParameter<float> syphonOpacity{"Syphon Texture", 0, 0, 1};
    ofParameter<float> syphonMeshDistortion{"Syphon Mesh Distortion", 0, 0, 1};
    ofParameter<float> meshDistortion{"Mesh Distortion", 0, -1, 1};
    
    ofParameter<float> transitionTime{};
    ofParameter<float> divideTriangleSize{"Triangles Size", 0, 5, 0};
    ofParameter<float> divideRadius{"Triangle Divide Radius", 0, 0, 5100};
    ofParameter<bool> divideInvert{"Triangle Divide Invert", false};
    ofParameter<float> wireframeAlpha{"Wireframe alpha", 1, 0, 1};
    ofParameter<float> fillAlpha{"Fill alpha", 1, 0, 1};
    ofParameter<float> wireframeLineWidth{"Wireframe line width", 1, 0, 10};
    
    ofParameter<ofColor> color{ "Color", ofFloatColor(1,1,1,1), ofFloatColor(0,0,0,0), ofFloatColor(1,1,1,1)};

    
  //  ofParameter<float> noise{"Noise Amount", 0, 0, 2};
  //  ofParameter<float> noiseSeedSpeed{"Noise Speed", 0, 0,1};
    
    
    ofParameter<float> light{"Light Amount", 0, 0, 1};
    ofParameter<ofVec3f> lightRotation{"Light Rotation", ofVec3f(90,90,90), ofVec3f(-90,-90,-90) ,ofVec3f(180,90,90)};
    
   
    
    ofParameterGroup params {"triangles",
        enabled,
        divideTriangleSize,
        divideRadius,
        divideInvert,
        syphonOpacity,
        syphonMeshDistortion,
        meshDistortion,
        color,
        fillAlpha,
        wireframeAlpha,
        wireframeLineWidth,
        //noise,
        //noiseSeedSpeed,
        light,
        lightRotation
    };
    
    Triangles() {
        ContentScene::params = params;
    }
    
    
    void setup();
    void draw();
    void debugDraw();
    void update();
    void parseOscMessage(ofxOscMessage *m);

    float pct;
    
    map<InputTriangle*, SubTriangle* > subTriangles;
    void divide(SubTriangle * triangle, float sizeGoal);
    void collapse(SubTriangle * triangle);
    void drawTriangle(SubTriangle * triangle, float opacity);
    void drawTriangleWireframe(SubTriangle * triangle);
    

    ofVec2f center;
    float directTextureOpacity;
    float noiseSeed;
    ofVec3f lightPos;
    ofLight pointLight;
    ofMaterial material;
    
    ofxAutoReloadedShader fillShader;
    ofxAutoReloadedShader wireframeShader;
    
    ofFbo depthFbo;
    ofImage edgeMask;
    
private:
    
    float _lightPhase;
};

