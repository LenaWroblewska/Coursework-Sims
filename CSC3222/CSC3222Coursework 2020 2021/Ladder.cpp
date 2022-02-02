#include "Ladder.h"
#include "CollisionVolume.h"

using namespace NCL;
using namespace CSC3222;

Ladder::Ladder() {
	this->width = 0;
	this->height = 0;
	inverseMass = 0.0f;
	elasticity = 0.0f;
}

Ladder::Ladder(double width, double height) {
	this->width = width;
	this->height = height;
	collider = new CollisionVolume(this, CollisionVolume::Shapes::QUADRILATERAL, CollisionVolume::Types::LADDER, width, height);
	inverseMass = 0.0f;
	elasticity = 0.0f;
}

void Ladder::DrawObject(GameSimsRenderer& r) {

}

bool Ladder::UpdateObject(float dt) {
	collider->setPosition(this->GetPosition());
	return true;
}