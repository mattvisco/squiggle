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

class hair{
    public:
        hair();
        void setup(int num, float d, ofPoint pos);
        void addForce(ofPoint f);
        void setFirstPosition(ofPoint pos);
        void update();
        void draw();
    public:
        float len;
        vector<particle> particles;
        ofPoint color;
};

#endif /* hair_hpp */
