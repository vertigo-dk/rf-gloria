//
//  Triangles.cpp
//  Gloria
//
//  Created by Jonas Jongejan on 01/07/13.
//
//

#include "Triangles.h"


void Triangles::setup(){
    
    map<Corner*, Corner*> cornerRefs;
    
    for(int i=0;i<mapping->triangles.size();i++){
        InputTriangle * triangle = mapping->triangles[i];
        
        SubTriangle * subTriangle =  new SubTriangle();
        subTriangle->level = 0;
        
        //Create a duplicate of the corners
        for(int j=0;j<3;j++){
            if(cornerRefs.find(triangle->corners[j]) != cornerRefs.end()){
                subTriangle->corners[j] = cornerRefs[triangle->corners[j]];
            } else {
                subTriangle->corners[j] = new Corner();
                subTriangle->corners[j]->pos = triangle->corners[j]->pos;
                subTriangle->corners[j]->origPos = triangle->corners[j]->pos;
                // subTriangle->corners[j]->randomSeed = ofVec3f(0,0,ofRandom(-0.5,0.5));
                
                cornerRefs[triangle->corners[j]] = subTriangle->corners[j];
            }
        }
        
        subTriangle->parentTriangle = subTriangle;
        subTriangle->age = 1000;
        
        subTriangles[mapping->triangles[i]] =  subTriangle ;
        
        center = ofVec2f(1500,500);
        divideRadius = 0;
    }
    
    for(int i=0;i<mapping->triangles.size();i++){
        
    }
    
    //Duplicate the joined corners
    for(int i=0;i<mapping->triangles.size();i++){
        InputTriangle * triangle = mapping->triangles[i];
        SubTriangle * subTriangle = subTriangles[triangle];
        
        for(int j=0;j<3;j++){
            for(int k=0; k<triangle->corners[j]->joinedCorners.size(); k++){
                Corner * c1 = triangle->corners[j]->joinedCorners[k];
                Corner * c2 = cornerRefs[c1];
                subTriangle->corners[j]->joinedCorners.push_back(c2);
            }
        }
    }
    
    //Create division Corners
    for(int i=0;i<mapping->triangles.size();i++){
        InputTriangle * triangle = mapping->triangles[i];
        SubTriangle * subTriangle = subTriangles[triangle];
        
        for(int j=0;j<3;j++){
            subTriangle->corners[j]->createDivisionCorners();
        }
        
    }
    
    //Divide
    for(int i=0;i<mapping->triangles.size();i++){
        InputTriangle * triangle = mapping->triangles[i];
        SubTriangle * subTriangle = subTriangles[triangle];
        
        
        subTriangle->divide(38000,1);
        
    }
    
    
    //
    
    for(int it=0;it<2;it++){
        for(int i=0;i<mapping->triangles.size();i++){
            InputTriangle * triangle = mapping->triangles[i];
            SubTriangle * subTriangle = subTriangles[triangle];
            vector<SubTriangle*> arr = subTriangle->getLowestLevelTriangles();
            
            for(int j=0;j<arr.size();j++){
                SubTriangle * _subTriangle = arr[j];
                
                for(int k=0;k<3;k++){
                    _subTriangle->corners[k]->createDivisionCorners();
                }
                
                _subTriangle->divide(0,it+2);
                
                
            }
        }
    }
    
    
    pointLight.setDiffuseColor( ofColor(255.f, 255.f, 255.f));
    pointLight.setAttenuation(2.);
    pointLight.setDirectional();
    pointLight.rotate(90,0,1,0);
    
    
    fillShader.setGeometryInputType(GL_TRIANGLES);
    fillShader.setGeometryOutputType(GL_TRIANGLES);
    fillShader.setGeometryOutputCount(3);
    
    wireframeShader.setGeometryInputType(GL_TRIANGLES);
    wireframeShader.setGeometryOutputType(GL_TRIANGLES);
    wireframeShader.setGeometryOutputCount(3);
    
    fillShader.load("shaders/TrianglesFill.vert","shaders/TrianglesFill.frag","shaders/TrianglesFill.geom");
    
    wireframeShader.load("shaders/TrianglesWireframe.vert","shaders/TrianglesWireframe.frag","shaders/TrianglesWireframe.geom");
    
    edgeMask.load("edgemask.jpg");
}




void Triangles::divide(SubTriangle * triangle, float sizeGoal){
}

void Triangles::collapse(SubTriangle * triangle){
}


float ease(float t, float b, float c, float d) {
    t /= d/2;
    if (t < 1) return c/2*t*t + b;
    t--;
    return -c/2 * (t*(t-2) - 1) + b;
};


void Triangles::drawTriangleWireframe(SubTriangle * triangle){
    /*bool subDraw = false;
    for(int i=0;i<triangle->subTriangles.size();i++){
        if(triangle->subTriangles[i]->drawLevel-1.5*triangle->subTriangles[i]->ageDifference  >= triangle->subTriangles[i]->level){
            subDraw = true;
        }
    }
    
    if(subDraw){
        for(int j=0;j<triangle->subTriangles.size();j++){
            drawTriangleWireframe(triangle->subTriangles[j]);
        }
    } else */
    
    float a = triangle->drawLevel - triangle->level- triangle->ageDifference;
    float aaa = MIN(1,MAX(0,a*2.0));
    if(triangle->level == 0){
        aaa = 1;
    }
    
    if(aaa > 0){
        
    
        for(int u=0;u<3;u++){
            ofVec3f pos = triangle->getPos(u) ;
            ofVec3f center = triangle->getCenter();
            
            /*    glTexCoord2d(syphonIn->getWidth()* center.x/OUTWIDTH
             , syphonIn->getHeight()*(OUTHEIGHT-center.y)/OUTHEIGHT);
             */
            glColor4f(wireframeAlpha*color.get().r/255.0,wireframeAlpha*color.get().g/255.0,wireframeAlpha*color.get().b/255.0,aaa);
            glMultiTexCoord2d(GL_TEXTURE0, syphonIn->getWidth()* center.x/OUTWIDTH
                              , syphonIn->getHeight()*(OUTHEIGHT-center.y)/OUTHEIGHT);
            
            glMultiTexCoord2d(GL_TEXTURE1,syphonIn->getWidth()* pos.x/OUTWIDTH
                              , syphonIn->getHeight()*(OUTHEIGHT-pos.y)/OUTHEIGHT);
            
            glMultiTexCoord2d(GL_TEXTURE2,pos.x, pos.y);
            
      
            glVertex3d(pos.x, pos.y, 0/*triangle->corners[u]->pos.z*/);
        }
        
        for(int j=0;j<triangle->subTriangles.size();j++){
            drawTriangleWireframe(triangle->subTriangles[j]);
        }
        
    }
}




void Triangles::drawTriangle(SubTriangle * triangle, float opacity){
    //ofVec3f normal = triangle->normal();
    
    
    /*bool subDraw = false;
    for(int i=0;i<triangle->subTriangles.size();i++){
        if(triangle->subTriangles[i]->drawLevel-1.0*triangle->subTriangles[i]->ageDifference  >= triangle->subTriangles[i]->level){
            subDraw = true;
        }
    }*/
    /*if(!subDraw)*/{
        
        
        
        
        float a = triangle->drawLevel - triangle->level- triangle->ageDifference;
        float aaa = MIN(1,MAX(0,a*2.0));
        if(triangle->level == 0){
            aaa = 1;
        }
       // aaa = 1.0;
        ofVec3f center = triangle->getCenter();
        if(aaa > 0){
            
            //  ofVec3f n = normal * aaa + parentNormal * (1.0-aaa);
            
            ofVec3f trianglePos = center;
            ofVec3f lightDir = ( trianglePos- lightPos);
            
            //Tegn billede 1 pixel pr trekant
            for(int u=0;u<3;u++){
                //glNormal3f(n.x, n.y, n.z);

                ofVec3f pos = triangle->getPos(u) ;
                glColor4f(fillAlpha*color.get().r/255.0,fillAlpha*color.get().g/255.0,fillAlpha*color.get().b/255.0,aaa);
                glMultiTexCoord2d(GL_TEXTURE0, syphonIn->getWidth()* center.x/OUTWIDTH
                                  , syphonIn->getHeight()*(OUTHEIGHT-center.y)/OUTHEIGHT);
                
                glMultiTexCoord2d(GL_TEXTURE1,syphonIn->getWidth()* pos.x/OUTWIDTH
                                  , syphonIn->getHeight()*(OUTHEIGHT-pos.y)/OUTHEIGHT);
                
                glMultiTexCoord2d(GL_TEXTURE2,pos.x, pos.y);
 
                glVertex3d(pos.x, pos.y, 0/*pos.z*/);
            }
            
            for(int j=0;j<triangle->subTriangles.size();j++){
                drawTriangle(triangle->subTriangles[j], opacity);
            }
        }
    }
    
    //if(subDraw){
            //}
}

void Triangles::draw(){
    ofClear(0);
    ofSetColor(255,255,255);
    
    pointLight.setOrientation(lightRotation);
    if(fillAlpha > 0){
        ofSetColor(color.get(), 255*fillAlpha);
        
        if(syphonIn->isSetup()) syphonIn->bind();

        fillShader.begin();
        //debugShader.setUniformTexture("depthTex", depthFbo.getTexture(), 2);
        fillShader.setUniformTexture("edgemask", edgeMask.getTexture(), 1);
        fillShader.setUniform1f("lightAmount", light);
        fillShader.setUniform1f("textureAmount", syphonOpacity);
        fillShader.setUniform1f("syphonMeshDistortion",syphonMeshDistortion);
        fillShader.setUniform1f("meshDistortion",meshDistortion);
       // material.setShininess(15);
       
        glBlendFunc(GL_ONE, GL_ONE_MINUS_SRC_ALPHA);
        
        ofEnableLighting();

        pointLight.enable();
       // material.begin();

       //ofDisableAlphaBlending();
        glBegin(GL_TRIANGLES);
        for(int i=0;i<mapping->triangles.size();i++){
            drawTriangle(subTriangles[mapping->triangles[i]],1);
        }
        glEnd();
        
        //material.end();
        
        pointLight.disable();
        ofDisableLighting();
        
        fillShader.end();
        if(syphonIn->isSetup()) syphonIn->unbind();
    }
    
    ofNoFill();
    ofEnableAlphaBlending();
    if(wireframeAlpha > 0){
        ofSetColor(color.get(), 255*wireframeAlpha);
        
        wireframeShader.begin();
        
        wireframeShader.setUniformTexture("edgemask", edgeMask.getTexture(), 1);
        wireframeShader.setUniform1f("lightAmount", light);
        wireframeShader.setUniform1f("textureAmount", syphonOpacity);
        wireframeShader.setUniform1f("syphonMeshDistortion",syphonMeshDistortion);
        wireframeShader.setUniform1f("meshDistortion",meshDistortion);
        glBlendFunc(GL_ONE, GL_ONE_MINUS_SRC_ALPHA);

        syphonIn->bind();
        ofEnableLighting();
        
        glLineWidth(wireframeLineWidth);
        glPolygonMode( GL_FRONT_AND_BACK, GL_LINE );
        //    glEnable(GL_LINE_SMOOTH);
        
        glBegin(GL_TRIANGLES);
                glLineWidth(1);
        for(int i=0;i<mapping->triangles.size();i++){
            drawTriangleWireframe(subTriangles[mapping->triangles[i]]);
        }
        glEnd();
        glPolygonMode( GL_FRONT_AND_BACK, GL_FILL );
        ofFill();
        
        syphonIn->unbind();
        wireframeShader.end();
    }
    ofDisableLighting();
    
    
    ofSetColor(255);
    
    ofEnableAlphaBlending();
    //  glDisable(GL_LINE_SMOOTH);
    
}

void Triangles::update(){
    
    if(syphonIn->isSetup()) {
    if(!depthFbo.isAllocated() || depthFbo.getWidth() != syphonIn->getWidth() || depthFbo.getHeight() != syphonIn->getHeight()){
        depthFbo.allocate(syphonIn->getWidth(), syphonIn->getHeight(), GL_RGB);
    }
    }
    
    /*depthFbo.begin();{
     ofFill();
     ofClear(255);
     ofSetColor(255,255,255);
     syphonIn->draw(0, 0, OUTWIDTH, OUTHEIGHT);
     }depthFbo.end();*/
    
    
    
    //noiseSeed += noiseSeedSpeed * 1.0/MAX(10,ofGetFrameRate());
    //if(noiseSeed > 1)
    //    noiseSeed = 0;
    for(int i=0;i<mapping->triangles.size();i++){
        SubTriangle * triangle = subTriangles[mapping->triangles[i]];
        float dist = triangle->getCenter().distance(ofPoint(OUTWIDTH*0.5,OUTHEIGHT));
        
        if(divideInvert){
            triangle->drawLevelGoal = MIN(divideTriangleSize.get(),MAX(1,divideTriangleSize.get() * ((3000-dist)/divideRadius.get())));
        } else {
            triangle->drawLevelGoal = MIN(divideTriangleSize.get(),MAX(1,divideTriangleSize.get() * dist/divideRadius.get()));
        }
        
       // triangle->noise = noise;
        //triangle->noiseSeed = noiseSeed;
        
        triangle->update();
    }
    
    
    //_lightPhase += 2*lightSpeed * 1.0 / MAX(10, MIN(ofGetFrameRate(),60));
    lightPos = ofVec3f(2596+1500*sin(_lightPhase),200,2000);
    
    center.x = 2996;
    center.y = 1200;
    
    pointLight.setPosition(lightPos);
}



void Triangles::parseOscMessage(ofxOscMessage *m){
    ContentScene::parseOscMessage(m);
}