#include "ofApp.h"

void squiggle::setup(ofPoint origin, int width) {
    squiggle::width = width;
    hairSkel.setup(150, 1, origin);
    makeSquiggleShape();
}

void squiggle::updateParticlePos() {
    
    vector<particle> hParticles = hairSkel.particles;
    
    for(int i = 0; i < hParticles.size()-1; i++) {
        ofPoint vect = hParticles[i+1].pos - hParticles[i].pos;
        ofPoint compare(0,0,1); // Get orthogonal in 2d plane
        ofPoint perp = vect.getPerpendicular(compare);
        ofPoint leftPoint = vect + perp * -width/2;
        ofPoint rightPoint = vect + perp * width/2;
        
//        particles[i*2].pos.set(hParticles[i].pos + leftPoint);
//        particles[i*2+1].pos.set(hParticles[i].pos + rightPoint);
        particles[i].pos.set(hParticles[i].pos + leftPoint);
        particles[particles.size()-1 - i].pos.set(hParticles[i].pos + rightPoint);
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
        s.distance = 15;//(s.particleA->pos-s.particleB->pos).length();
        s.springiness = 0.01;
        springs.push_back(s);
    }
    
}

void squiggle::moveTo(float x, float y) {
    squigPos = ofPoint(x,y);
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
    
                particles[i].addRepulsionForce( particles[j], (x1 + y1), 0.1);
    //            particles[i].addAttractionForce(particles[j], 50, 0.1);
    //            particles[i].addRepulsionForce(particles[j], 10, 0.9);
            }
        }
    
        for (int i = 0; i < springs.size(); i++){
            springs[i].update();
        }
    
    
        for (int i = 0; i < particles.size(); i++){
            particles[i].addDampingForce();
            particles[i].update();
        }
}

void squiggle::update() {
    hairSkel.setFirstPosition(squigPos);
    hairSkel.addForce(ofPoint(0, 50)); // Gravity
    hairSkel.update();
    updateParticlePos();
//    updateParticles();
}

float getSlope(ofPoint p1, ofPoint p2) {
    return (p2.y - p1.y) / (p2.x - p1.x);
}

float getYIntercept(ofPoint pt, float slope) {
    return pt.y - slope * pt.x;
}

float getYValue(ofPoint pt, ofPoint low, ofPoint high) {
    float slope = getSlope(low, high);
    float b = getYIntercept(low, slope);
    return pt.x * slope + b;
}

bool particleBelow(ofPoint pt, ofPoint low, ofPoint high) {
    return pt.y > getYValue(pt, low, high);
}

bool highToLow(particle p1, particle p2) {
    if (p1.pos.x > p2.pos.x){
//        if( p1.pos.x - p2.pos.x > -0.001 ||
//           (p1.pos.x - p2.pos.x <= -0.001 && p1.pos.y <= p2.pos.y) ) {
//            return true;
//        } else {
//            return false;
//        }
        return true;
    } else {
        return false;
    }
}

//bool lowToHigh(particle p1, particle p2) {
//    if (p2.pos.x - p1.pos.x >= -0.1){
//        if( p2.pos.x - p1.pos.x > -0.001 ||
//           (p2.pos.x - p1.pos.x <= -0.001 && p2.pos.y <= p1.pos.y) ) {
//            return true;
//        } else {
//            return false;
//        }
//    } else {
//        return false;
//    }
//}

bool lowToHigh(particle p1, particle p2) {
    return highToLow(p2, p1);
}

void orderParticlesOpenPolygon(vector<particle> &particles, bool debug) {
    particle low;
    low.pos.set(99999, 0);
    int lowIndex = -1;
    particle high;
    high.pos.set(-99999,0);
    int highIndex = -1;
    int index = 0;
    for(auto & particle : particles) {
        if(low.pos.x - particle.pos.x >= -0.1) {
            if( low.pos.x - particle.pos.x > -0.001 ||
               (low.pos.x - particle.pos.x <= -0.001 && particle.pos.y >= low.pos.y) ) {
                low = particle;
                lowIndex = index;
            }
        }
        if(particle.pos.x - high.pos.x >= -0.1) {
            if( particle.pos.x - high.pos.x > -0.001 ||
               (particle.pos.x - high.pos.x <= -0.001 && particle.pos.y <= high.pos.y) ) {
                high = particle;
                highIndex = index;
            }
        }
        index++;
    }
    vector<particle> lowParticles;
    vector<particle> highParticles;
    for(int i = 0; i < particles.size(); i++) {
        if(i != lowIndex && i != highIndex) {
            particle particle = particles[i];
            if(particleBelow(particle.pos, low.pos, high.pos)) lowParticles.push_back(particle);
            else highParticles.push_back(particle);
        }
    }
    // Sort Particles Array
    ofSort(lowParticles, highToLow); // Sort from highest x to lowest
    ofSort(highParticles, lowToHigh); // Sort from lowest x to highest
    
    // Create newly ordered open polygon
    vector<particle> newParticles;
    newParticles.push_back(low);
    newParticles.insert(newParticles.end(), highParticles.begin(), highParticles.end());
    newParticles.push_back(high);
    newParticles.insert(newParticles.end(), lowParticles.begin(), lowParticles.end());
    
    particles = newParticles;
    
    if(debug) {
        ofSetColor(255,0,0);
        ofDrawCircle(low.pos, 3);
        ofSetColor(0,255,0);
        for(auto & part : highParticles) {
            ofDrawCircle(part.pos, 1);
        }
        ofSetColor(255,0,0);
        ofDrawCircle(high.pos, 3);
        ofSetColor(0,0,255);
        for(auto & part : lowParticles) {
            ofDrawCircle(part.pos, 1);
        }
    }
}

void squiggle::draw() {
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
    //     line = line.getResampledByCount(2000);
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
}




//--------------------------------------------------------------
void ofApp::setup(){
    squig.setup(ofPoint(100,100), 30);
    
    ofSetVerticalSync(true);
    
    cam.initGrabber(640, 480);
    
    tracker.setup();
}

//--------------------------------------------------------------
void ofApp::update(){
    
//    cam.update();
//    if(cam.isFrameNew()) {
//        tracker.update(toCv(cam));
//        position = tracker.getPosition();
//        scale = tracker.getScale();
//        orientation = tracker.getOrientation();
//        rotationMatrix = tracker.getRotationMatrix();
//    }
    
//    ofPolyline eye = tracker.getObjectFeature(tracker.LEFT_EYE);
//    cout << eye.getVertices()[0] << endl;
    squig.moveTo(mouseX, mouseY);
    squig.update();
}

//--------------------------------------------------------------
void ofApp::draw(){
    ofBackground(255, 253, 208);
    
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
    
    squig.draw();
}

//--------------------------------------------------------------
void ofApp::keyPressed(int key){
    squig.DEBUG = !squig.DEBUG;
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
