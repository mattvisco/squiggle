#include "ofApp.h"

void squiggle::setup(ofPoint origin, int width) {
    squiggle::width = width;
    pos = origin;
    hairSkel.setup(150, 1, pos);
    makeSquiggleShape();
}

void squiggle::updateParticlePos() {
    
    vector<particle> hParticles = hairSkel.particles;
    
    float t = ofGetElapsedTimef();
    for(int i = 0; i < hParticles.size()-1; i++) {
        ofPoint vect = hParticles[i+1].pos - hParticles[i].pos;
        ofPoint compare(0,0,1); // Get orthogonal in 2d plane
        ofPoint perp = vect.getPerpendicular(compare);
        ofPoint leftPoint = vect + perp * -width/2;
        ofPoint rightPoint = vect + perp * width/2;
        
        particles[i].pos.set(hParticles[i].pos + leftPoint - ofMap(ofNoise(i,t,ofRandom(100)),0,1,-20,20));
        particles[particles.size()-1 - i].pos.set(hParticles[i].pos + rightPoint + ofMap(ofNoise(i,t,ofRandom(100)),0,1,-20,20));
    }
}

void squiggle::updateObjects(vector<CircleObj>& objects) {
    circles.clear();
    addObjects(objects);
}

void squiggle::addObjects(vector<CircleObj>& objects) {
    for(auto & obj : objects) {
        circles.push_back(obj);
    }
}

void squiggle::makeSquiggleShape() {
    ofPolyline squiggle;
    ofPoint currPos;
    
    vector<ofPoint> springPairs;
    
    vector<particle> hParticles = hairSkel.particles;
    
    vector<particle> leftSide;
    vector<particle> rightSide;
    // Make Particles
    for(int i = 0; i < hParticles.size()-1; i++) {
        ofPoint vect = hParticles[i+1].pos - hParticles[i].pos;
        ofPoint compare(0,0,1);
        ofPoint perp = vect.getPerpendicular(compare);
        ofPoint leftPoint = vect + perp * -width;
        ofPoint rightPoint = vect + perp * width;
        
        particle p1;
//        leftSide.push_back(p1);
        particles.push_back(p1);
        particle p2;
        rightSide.push_back(p2);
//        particles.push_back(p2);
    }
    
//    particles.insert(particles.end(), rightSide.begin(), rightSide.end());
    for(int i = rightSide.size()-1; i >= 0; i--) {
        particles.push_back(rightSide[i]);
    }
    
    // Make Springs
    for (int i = 0; i < particles.size(); i++){
        spring s;
        s.particleA = &particles[i];
        s.particleB = &particles[(i+1) % particles.size()];
        s.distance = 5;
        s.springiness = 0.1;
        springs.push_back(s);
    }
    
}

void squiggle::moveTo(float x, float y) {
    pos = ofPoint(x,y);
}

void squiggle::updateParticles() {
        for (int i = 0; i < particles.size(); i++){
            particles[i].resetForce();
        }
    
        float f = ofGetElapsedTimef();
        for (int i = 0; i < particles.size(); i++){
            for (int j = 0; j < i; j++){
    
                float x1 = sin(i/40.0 + f*1) * 2 + 2;
                float y1 = sin(j/40.0  + f + PI) * 2 + 2;
    
//                particles[i].addRepulsionForce( particles[j], (x1 + y1), 0.9);
//                particles[i].addAttractionForce(particles[j], 10, 0.1);
//                particles[i].addRepulsionForce(particles[j], 1, 0.9);
            }
        }
    
        for (int i = 0; i < springs.size(); i++){
//            springs[i].update();
        }
    
    
        for (int i = 0; i < particles.size(); i++){
            particles[i].addDampingForce();
            particles[i].update();
        }
}

void squiggle::update() {
    hairSkel.setFirstPosition(pos);
    hairSkel.addForce(ofPoint(0, 50)); // Gravity
//    cout << circles[0]->origin.x << endl;
    hairSkel.update(circles);
//    hairSkel.update();
    updateParticlePos();
//    updateParticles(); // Not Working Ñ wasn't doing much anyway
}

ofColor pickColor(hair hair) {
    ofPoint basePoint(1,0,0);
    int stepSize = hair.particles.size() / 3;
    ofPoint section1 = hair.particles[stepSize].pos - hair.particles[0].pos;
    ofPoint section2 = hair.particles[stepSize*2].pos - hair.particles[stepSize].pos;
    ofPoint section3 = hair.particles[stepSize*3 - 1].pos - hair.particles[stepSize*2].pos;
    float angle1 = section1.angle(basePoint);
    float angle2 = section2.angle(basePoint);
    float angle3 = section3.angle(basePoint);
    int r = ofMap(angle1, -180, 0, 0, 255, true);
    int g = ofMap(angle2, 0, 180, 0, 255, true);
    int b = ofMap(angle3, 0, 180, 0, 255, true);
    return ofColor(r,g,b);
}

void squiggle::draw() {
    if(!off) { // Quuick way to test if my logic works Ñ change it later!!
        ofPolyline line;

        for (auto a : particles){
            line.addVertex(a.pos);
        }
        
        if(DEBUG) {
            ofSetColor(255, 0, 0);
            line.draw();
            ofSetColor(255, 0, 0);
            hairSkel.draw();
        } else {
            line.setClosed(true);
    //         line = line.getResampledByCount(1000);
            line = line.getSmoothed(25);
            
            squig.clear();
            for (auto vertex : line.getVertices()){
                squig.lineTo(vertex);
            }
            squig.close();
            squig.setStrokeColor(ofColor::black);
            
            ofColor color = pickColor(hairSkel);
            squig.setFillColor(color);
            
            squig.setFilled(true);
            squig.setStrokeWidth(3);
            squig.draw();
        }
    }
}




//--------------------------------------------------------------
void ofApp::setup(){
    
    //Set Up Circle Object
    CircleObj circ{ofPoint(100, 100), 50};
    CircleObj circ2{ofPoint(200, 100), 50};
    circles.push_back(circ);
    circles.push_back(circ2);
    
    //Set Up Squiggles
    for(int i = 0; i < 150; i++) {
        squiggle squig;
        ofPoint pos(ofRandom(0,ofGetWidth()),ofRandom(0,ofGetHeight()));
        squig.setup(pos, 30);
        squig.addObjects(circles);
        squiggles.push_back(squig);
        particle p;
        p.setInitialCondition(pos.x, pos.y, 0, 0, 0, 0);
        vfParticles.push_back(p);
    }
    
    // Set up vector field
    VF.setupField(60,40,ofGetWidth(), ofGetHeight());
    VF.randomizeField(50.0);
    
    // ofXFaceTrack
//    ofSetVerticalSync(true);
//    cam.initGrabber(640, 480);
//    tracker.setup();
}

bool inCircle(CircleObj obj, ofPoint pt) {
    float dist = sqrt(pow((obj.origin.x - pt.x),2) + pow((obj.origin.y - pt.y),2));
    if( dist < obj.radius) return true;
    else return false;
}

//--------------------------------------------------------------
void ofApp::update(){
    
    float x1 = MIN(ofGetWidth(),MAX(mouseX + 100,0));
    float y1 = MIN(ofGetHeight(),MAX(mouseY + 200, 0));
    circles[0].origin = ofPoint(x1, y1);
//    cout << circles[0]->origin.x << endl;
    
    float x2 = MIN(ofGetWidth(),MAX(mouseX + 300,0));
    float y2 = MIN(ofGetHeight(),MAX(mouseY + 200, 0));
    circles[1].origin = ofPoint(x2, y2);
    
    // Move squiggle with move
    int index = 0;
    for(auto & squig : squiggles) {
        squig.updateObjects(circles);
        float x = MIN(ofGetWidth(),MAX(mouseX + ((index % 10)*50),0));
        float y = MIN(ofGetHeight(),MAX(mouseY + floor(index/10)*50, 0));
//        for(auto & circle : circles) {
//            if(inCircle(circle, ofPoint(x,y))) squig.off = true;
//        }
        squig.moveTo(x, y);
        // Mirror motion
//        if(index > squiggles.size()/2) squig.moveTo( ofGetWidth() - mouseX - ((index % 10)*40), ofGetHeight() - mouseY - floor(index/10)*40);
//        else squig.moveTo(mouseX + ((index % 10)*40), mouseY + floor(index/10)*40);
        squig.update();
        index++;
    }

    // Move squiggle with noise
//    float time = ofGetElapsedTimef();
//    ofSeedRandom(0);
//    for(int i = 0; i < squiggles.size(); i++) {
//        float x = ofMap(ofNoise((i*100), time/10, ofRandom(100)),0, 1, 0, ofGetWidth());
//        float y = ofMap(ofNoise(i*2, time/6, ofRandom(100)),0, 1, 0, ofGetHeight());
//        squiggles[i].moveTo(x, y);
//        squiggles[i].update();
//    }
    
    // Move squiggle with vector field
//    for(auto & squig : squiggles) {
//        ofPoint frc;
//        frc = VF.getForceFromPos(squig.pos.x, squig.pos.y);
//        squig.hairSkel.particles[0].addForce(frc);
//        ofPoint pPos = squig.hairSkel.particles[0].getUpdatePos();
//        squig.moveTo(pPos.x, pPos.y);
////        frc = frc * 10;
////        squig.moveTo(squig.pos.x + frc.x, squig.pos.y + frc.y);
//        squig.update();
//    }
    
    // ofXFaceTrack
    //    cam.update();
    //    if(cam.isFrameNew()) {
    //        tracker.update(toCv(cam));
    //        position = tracker.getPosition();
    //        scale = tracker.getScale();
    //        orientation = tracker.getOrientation();
    //        rotationMatrix = tracker.getRotationMatrix();
    //    }
    //    if(tracker.getFound()) {
    //    ofPolyline eye = tracker.getObjectFeature(tracker.LEFT_EYE);
    //    cout << eye.getVertices()[0] << endl;
    //    }
}

//--------------------------------------------------------------
void ofApp::draw(){
    
    ofBackground(0, 0, 0);
//    ofBackground(255, 253, 208);
    
//    ofSetColor(255);
//    cam.draw(0, 0);
//    ofDrawBitmapString(ofToString((int) ofGetFrameRate()), 10, 20);
//    
//    if(tracker.getFound()) {
//        ofSetLineWidth(1);
//        tracker.draw();
//        
//        ofSetupScreenOrtho(640, 480, -1000, 1000);
//        ofTranslate(640 / 2, 480 / 2);
//        
//        ofPushMatrix();
//        ofScale(5,5,5);
//        tracker.getObjectMesh().drawWireframe();
//        ofPopMatrix();
//        
//        applyMatrix(rotationMatrix);
//        ofScale(5,5,5);
//        tracker.getObjectMesh().drawWireframe();
//    }
    
    for(auto & squig : squiggles) {
        squig.draw();
    }
    
    
    
//    ofSetColor(255, 255, 255);
//    for(auto & circle : circles) {
//        ofDrawCircle(circle.origin, circle.radius);
//    }
    
//    ofEnableAlphaBlending();
//    ofSetColor(0,130,130, 200);
//    VF.draw();
}

//--------------------------------------------------------------
void ofApp::keyPressed(int key){
    if (key == ' '){
        drawingStyle ++;
        drawingStyle %= 4;
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
    switch (drawingStyle){
        case 0: VF.addInwardCircle((float)x, (float)y, 200, 0.3f);
            break;
        case 1: VF.addOutwardCircle((float)x, (float)y, 100, 0.3f);
            break;
        case 2: VF.addClockwiseCircle((float)x, (float)y, 100, 0.3f);
            break;
        case 3: VF.addCounterClockwiseCircle((float)x, (float)y, 100, 0.3f);
            break;
    }
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
