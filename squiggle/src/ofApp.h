#pragma once

#include "ofMain.h"
#include "particle.h"
#include "spring.h"

class squiggle{
    
public:
    
    float scale;
    float offset;
    vector < particle *  > particles;
    vector < spring > springs;
    
    void makeSquiggleShape(int width, int height);
    
    void setup();
    void update();
    void draw();
    
    bool drag = false;
    ofPoint dragPos;
    float moveForce;
    ofPath squig;
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
    
    bool dragStarted = false;
    ofPoint lastMouse;
    
    squiggle s;
};
