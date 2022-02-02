#include "Wall.h"
#include "CollisionVolume.h"

using namespace NCL;
using namespace CSC3222;

Wall::Wall() {
	this->width = 0;
	this->height = 0;
	inverseMass = 0.0f;
	elasticity = 0.0f;
}

Wall::Wall(double width, double height) {
	this->width = width;
	this->height = height;
	collider = new CollisionVolume(this, CollisionVolume::Shapes::QUADRILATERAL, CollisionVolume::Types::WALL, width, height);
	inverseMass = 0.0f;
	elasticity = 0.0f;
}

void Wall::DrawObject(GameSimsRenderer& r) {

}

bool Wall::UpdateObject(float dt) {
	collider->setPosition(this->GetPosition());
	return true;
}