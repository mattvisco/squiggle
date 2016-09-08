#include "ofApp.h"

void squiggle::makeSquiggleShape(int width, int height) {
    ofPolyline squiggle;
    ofPoint origin = ofPoint(ofGetWidth() / 2, ofGetHeight() * 3/4);
    ofPoint currPos;
    
    int index = 0;
    vector<ofPoint> springPairs;
    
    // Semi-cirlce 1
    for(int i = 0; i < 100; i++) {
        float angle = ofMap(i, 0, 99, 0, PI);
        ofPoint pos = origin + width * ofPoint(cos(angle), sin(angle));
        particle * p = new particle();
        p->setInitialCondition(pos.x,pos.y,pos.z, 0, 0, 0);
        particles.push_back(p);
        if(i == 99) currPos = pos;
        index++;
    }
    
    // Side Left
    for(int i = 0; i < height; i++) {
        ofPoint pos = currPos - ofPoint(0, i);
        particle * p = new particle();
        p->setInitialCondition(pos.x,pos.y,pos.z, 0, 0, 0);
        particles.push_back(p);
        if(i == height-1) currPos = pos;
        if (i >= height-10)dragPoints.push_back(index);
        index++;
        springPairs.push_back(ofPoint(index,0));
    }
    
    // Semi-cirlce 2
    for(int i = 0; i < 100; i++) {
        float angle = ofMap(i, 0, 99, -PI, 0);
        ofPoint pos = currPos + ofPoint(width, 0) + width * ofPoint(cos(angle), sin(angle));
        particle * p = new particle();
        p->setInitialCondition(pos.x,pos.y,pos.z, 0, 0, 0);
        particles.push_back(p);
        if(i == 99) currPos = pos;
        dragPoints.push_back(index);
        index++;
    }
    
    // Side Right
    for(int i = 0; i < height; i++) {
        ofPoint pos = currPos + ofPoint(0, i);
        particle * p = new particle();
        p->setInitialCondition(pos.x,pos.y,pos.z, 0, 0, 0);
        particles.push_back(p);
        if (i <= 10)dragPoints.push_back(index);
        springPairs[i].y = index;
//        springPairs[height-1-i].y = index;
        index++;
    }
    
    // Make Springs
    for (int i = 0; i < particles.size(); i++){
        spring s;
        s.particleA = particles[i];
        s.particleB = particles[(i+1) % particles.size()];
        s.distance = (s.particleA->pos-s.particleB->pos).length();
        s.springiness = 0.9;
        springs.push_back(s);
    }
    
//    for(auto & springPair : springPairs) {
//        spring s;
//        s.particleA = particles[springPair.x];
//        s.particleB = particles[springPair.y];
////        s.distance = 5;
//        s.distance = (s.particleA->pos-s.particleB->pos).length();
//        s.springiness = 0.1;
//        springs.push_back(s);
//    }
    
//    for(auto & springPair : springPairs) {
//        spring s;
//        s.particleA = particles[springPair.x];
//        s.particleB = particles[springPair.y];
//        //        s.distance = 5;
//        s.distance = (s.particleA->pos-s.particleB->pos).length();
//        s.springiness = 0.1;
//        springs.push_back(s);
//    }
    
//    spring s;
//    s.particleA = particles[springPairs[0].x];
//    s.particleB = particles[springPairs[springPairs.size()-1].y];
//    s.distance = (s.particleA->pos-s.particleB->pos).length();
//    s.springiness = 0.8;
//    springs.push_back(s);
//    
//    s;
//    s.particleA = particles[springPairs[springPairs.size()-1].x];
//    s.particleB = particles[springPairs[0].y];
//    s.distance = (s.particleA->pos-s.particleB->pos).length();
//    s.springiness = 0.8;
//    springs.push_back(s);

}

void squiggle::setup() {
    makeSquiggleShape(20, 300);
}

void squiggle::update(){
    
    for (int i = 0; i < particles.size(); i++){
        particles[i]->resetForce();
//        particles[i]->bounceOffWalls();
// if(drag) particles[i]->addAttractionForce(dragPos.x, dragPos.y, dragPos.z, 1000, moveForce);
    }
    
    if(drag ) {
        for(auto& point : dragPoints) {
            particles[point]->pos += move;
//            particles[point]->pos.x += moveForce;
        }
    }
    
    float f = ofGetElapsedTimef();
    for (int i = 0; i < particles.size(); i++){
        for (int j = 0; j < i; j++){
            
            float x1 = sin(i/40.0 + f*1) * 2 + 2;
            float y1 = sin(j/40.0  + f + PI) * 2 + 2;
            
            particles[i]->addRepulsionForce( *particles[j], (x1 + y1), 0.1);
//            particles[i]->addAttractionForce(*particles[j], 50, 0.1);
        }
    }
    
    for (int i = 0; i < springs.size(); i++){
        springs[i].update();
    }
    
    
    for (int i = 0; i < particles.size(); i++){
        particles[i]->addDampingForce();
        particles[i]->update();
    }
}

void squiggle::draw(){
    
    ofPolyline line;
    for (auto a : particles){
        line.addVertex(a->pos);
    }
    line.setClosed(true);
//    line = line.getResampledByCount(2000);
    line = line.getSmoothed(11);
    
    squig.clear();
    for (auto vertex : line.getVertices()){
        squig.lineTo(vertex);
    }
    squig.close();
    squig.setStrokeColor(ofColor::black);
    squig.setFillColor(ofColor::green);
    squig.setFilled(true);
    squig.setStrokeWidth(3);
    squig.draw();
}

void squiggle::debugDraw(){
    
    ofSetColor(255, 0, 0);
    for (auto a : particles){
        ofDrawCircle(a->pos, 1);
    }
    ofSetColor(0, 255, 255);
    for(auto s : springs) {
        s.draw();
    }
}

//--------------------------------------------------------------
void ofApp::setup(){
    s.setup();
}

//--------------------------------------------------------------
void ofApp::update(){
    s.update();
}

//--------------------------------------------------------------
void ofApp::draw(){
    ofBackground(201,255,0);
//    s.draw();
    s.debugDraw();
}

//--------------------------------------------------------------
void ofApp::keyPressed(int key){

}

//--------------------------------------------------------------
void ofApp::keyReleased(int key){

}

//--------------------------------------------------------------
void ofApp::mouseMoved(int x, int y ){

}

//--------------------------------------------------------------
void ofApp::mouseDragged(int x, int y, int button){
    ofPoint currMouse(x,y);
    s.move = (currMouse - lastMouse);
    s.moveForce = (currMouse - lastMouse).length();
    s.dragPos = currMouse;
    lastMouse = currMouse;
}

//--------------------------------------------------------------
void ofApp::mousePressed(int x, int y, int button){
    s.drag = true;
    lastMouse = ofPoint(x,y);
    s.dragPos = lastMouse;
    s.moveForce = 0.0;
}

//--------------------------------------------------------------
void ofApp::mouseReleased(int x, int y, int button){
    s.drag = false;
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
