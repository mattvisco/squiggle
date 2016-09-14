//
//  hair.cpp
//  squiggleHairSkel
//
//  Created by Matthew Visco on 9/7/16.
//
//

#include "hair.hpp"


hair::hair()
:len(10)
{
}

void hair::setup(int num, float d, ofPoint pos) {
    float dim = 50;
    len = d;
    float mass = 1;
    for(int i = 0; i < num; ++i) {
        particle p;
        p.setInitialCondition(pos.x, pos.y, pos.z, 0, 0, 0);
        particles.push_back(p);
        pos.y += d;
    }
    
    particles[0].bFixed = true;
}

void hair::addForce(ofPoint f) {
    for(auto & p : particles) {
        p.addForce(f);
    }
}

void hair::setFirstPosition(ofPoint pos) {
    particles[0].pos.set(pos);
}

void hair::update() {
    float dt = 1.0f/20.0f;
    
    // update velocities
    for(auto & p : particles) {
        if(p.bFixed) {
            p.tmp_pos = p.pos;
            continue;
        }
        p.vel = p.vel + dt * (p.frc);
        p.tmp_pos += (p.vel * dt);
        p.resetForce();
        p.vel *= 0.5; // Damping
    }
    
    // solve constraints
    ofPoint dir;
    ofPoint curr_pos;
    for(size_t i = 1; i < particles.size(); ++i) {
        particle &pa = particles[i - 1];
        particle &pb = particles[i];
        curr_pos = pb.tmp_pos;
        dir = pb.tmp_pos - pa.tmp_pos;
        dir.normalize();
        pb.tmp_pos = pa.tmp_pos + dir * len;
        pb.d = curr_pos - pb.tmp_pos; //  - curr_pos;
    }
    
    particles[0].bounceOffCircle(ofPoint(200,200), 50);
    for(size_t i = 1; i < particles.size(); ++i) {
        particle &pa = particles[i-1];
        particle &pb = particles[i];
        if(pa.bFixed) {
            continue;
        }
        pa.vel = ((pa.tmp_pos - pa.pos) / dt) + 0.95 *  (pb.d / dt);
        pa.pos = pa.tmp_pos;
        pa.bounceOffWalls();
    }
    
    particle &last = particles.back();
    last.pos = last.tmp_pos;
    
}

void hair::update(vector<CircleObj> objects) {
    float dt = 1.0f/20.0f;
    
    // update velocities
    for(auto & p : particles) {
        if(p.bFixed) {
            p.tmp_pos = p.pos;
            continue;
        }
        p.vel = p.vel + dt * (p.frc);
        p.tmp_pos += (p.vel * dt);
        p.resetForce();
        p.vel *= 0.5; // Damping
    }
    
    // solve constraints
    ofPoint dir;
    ofPoint curr_pos;
    for(size_t i = 1; i < particles.size(); ++i) {
        particle &pa = particles[i - 1];
        particle &pb = particles[i];
        curr_pos = pb.tmp_pos;
        dir = pb.tmp_pos - pa.tmp_pos;
        dir.normalize();
        pb.tmp_pos = pa.tmp_pos + dir * len;
        pb.d = curr_pos - pb.tmp_pos; //  - curr_pos;
    }
    
    particles[0].bounceOffCircle(ofPoint(200,200), 50);
    for(size_t i = 1; i < particles.size(); ++i) {
        particle &pa = particles[i-1];
        particle &pb = particles[i];
        if(pa.bFixed) {
            continue;
        }
        pa.vel = ((pa.tmp_pos - pa.pos) / dt) + 0.95 *  (pb.d / dt);
        pa.pos = pa.tmp_pos;
        pa.bounceOffWalls();
        for(auto & obj : objects) {
            pa.bounceOffCircle(obj.origin, obj.radius);
        }
    }
    
    particle &last = particles.back();
    last.pos = last.tmp_pos;
    
}

void hair::draw() {
    ofSetColor(0,0,0);
    ofPolyline line;
    for(auto & p : particles) {
        line.addVertex(p.pos);
    }
    line.draw();
}