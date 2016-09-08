#include "spring.h"

//---------------------------------------------------------------------
spring::spring(){
	particleA = NULL;
	particleB = NULL;
}

//---------------------------------------------------------------------
void spring::update(){
	if ((particleA == NULL) || (particleB == NULL)){
		return;
	}
	
	ofPoint pta = particleA->pos;
	ofPoint ptb = particleB->pos;
	
	float theirDistance = (pta - ptb).length();
	float springForce = (springiness * (distance - theirDistance));
	ofPoint frcToAdd = (pta-ptb).normalize() * springForce;
	
	particleA->addForce(frcToAdd.x, frcToAdd.y, frcToAdd.z);
	particleB->addForce(-frcToAdd.x, -frcToAdd.y, -frcToAdd.z);
}


//---------------------------------------------------------------------
void spring::draw(){
	
	if ((particleA == NULL) || (particleB == NULL)){
		return;
	}
	
	ofDrawLine(particleA->pos.x, particleA->pos.y, particleA->pos.z, particleB->pos.x, particleB->pos.y, particleB->pos.z);
}