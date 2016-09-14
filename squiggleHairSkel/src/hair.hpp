//
//  hair.hpp
//  squiggleHairSkel
//
//  Created by Matthew Visco on 9/7/16.
//
//

#ifndef hair_hpp
#define hair_hpp

#include "ofMain.h"
#include "particle.h"

struct CircleObj {
    ofPoint origin;
    int radius;
};

class hair{
    public:
        hair();
        void setup(int num, float d, ofPoint pos);
        void addForce(ofPoint f);
        void setFirstPosition(ofPoint pos);
        void update();
        void update(vector<CircleObj> objects);
        void draw();
    public:
        float len;
        vector<particle> particles;
        ofPoint color;
};

#endif /* hair_hpp */
