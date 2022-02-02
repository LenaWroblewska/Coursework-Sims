#include "RigidBody.h"

using namespace NCL;
using namespace CSC3222;

RigidBody::RigidBody()	{
	inverseMass = 1.0f;
	elasticity  = 0.5f;
	damping = 0.99f;
}


RigidBody::~RigidBody()	{

}

void RigidBody::updateSprings() {
	const float x = 1; 
	const float snappiness = 0.5; 

	Vector2 posDiff = this->GetPosition() - spring->GetPosition();
	Vector2 normal = posDiff.Normalised();

	float force = (posDiff.Length() - 15) * ((-1) * snappiness);
	float relativeVelocity = Vector2::Dot(spring->GetVelocity() - this->GetVelocity(), normal);
	float dampForce = force - x * relativeVelocity;
	
	this->AddForce(normal * dampForce);

}

