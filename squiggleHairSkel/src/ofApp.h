#pragma once

#include "ofMain.h"

#include "particle.h"
#include "spring.h"
#include "hair.hpp"

#include "ofxCv.h"
using namespace ofxCv;
using namespace cv;

#include "ofxFaceTracker.h"

class squiggle{
    
public:
    int width;
    ofPoint squigPos;
    vector < particle > particles;
    vector < spring > springs;
    
    bool DEBUG = false;
    
    hair hairSkel;
    ofPath squig;
    
    void makeSquiggleShape();
    
    void moveTo(float x, float y);
    void setup(ofPoint origin, int width);
    void updateParticlePos();
    void updateParticles();
    void update();
    void draw();
    
};

class ofApp : public ofBaseApp{

	public:
		void setup();
		void update();
		void draw();

		void keyPressed(int key);
		void keyReleased(int key);
		void mouseMoved(int x, int y );
		void mouseDragged(int x, int y, int button);
		void mousePressed(int x, int y, int button);
		void mouseReleased(int x, int y, int button);
		void mouseEntered(int x, int y);
		void mouseExited(int x, int y);
		void windowResized(int w, int h);
		void dragEvent(ofDragInfo dragInfo);
		void gotMessage(ofMessage msg);
    
    squiggle squig;
    
    ofVideoGrabber cam;
    ofxFaceTracker tracker;
    ofVec2f position;
    float scale;
    ofVec3f orientation;
    ofMatrix4x4 rotationMatrix;
    
    Mat translation, rotation;
    ofMatrix4x4 pose;
};
