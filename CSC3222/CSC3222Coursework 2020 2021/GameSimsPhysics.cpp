#include "GameSimsPhysics.h"
#include "RigidBody.h"
#include "CollisionVolume.h"
#include "../../Common/Vector2.h"

using namespace NCL;
using namespace CSC3222;

GameSimsPhysics::GameSimsPhysics()	{

}

GameSimsPhysics::GameSimsPhysics(FruitWizardGame* game) : GameSimsPhysics() {
	this->game = game;
}

GameSimsPhysics::~GameSimsPhysics()	{

}

void GameSimsPhysics::Update(float dt) {
	Integration(dt);
	CollisionDetection(dt);
	CollisionResolution(dt);
	IntegrateVelocity(dt);

	for (int i = 0; i < allBodies.size(); i++) {
		allBodies[i]->force = Vector2(0, 0);
	}
}

void GameSimsPhysics::FixedUpdate(float realDT){
	timeRemaining += realDT; 

	const float subTimeDelta = 1.0 / 120.0; 

	while (timeRemaining > subTimeDelta) {
		Update(subTimeDelta);
		timeRemaining -= subTimeDelta; 
	}
}

void GameSimsPhysics::AddRigidBody(RigidBody* b) {
	allBodies.emplace_back(b);
}

void GameSimsPhysics::RemoveRigidBody(RigidBody* b) {
	auto at = std::find(allBodies.begin(), allBodies.end(), b);

	if (at != allBodies.end()) {
		//maybe delete too?
		allBodies.erase(at);
	}
}

void GameSimsPhysics::AddCollider(CollisionVolume* c) {
	allColliders.emplace_back(c);
}

void GameSimsPhysics::RemoveCollider(CollisionVolume* c) {
	auto at = std::find(allColliders.begin(), allColliders.end(), c);

	if (at != allColliders.end()) {
		//maybe delete too?
		allColliders.erase(at);
	}
}

void GameSimsPhysics::Integration(float dt) {
	for (int i = 0; i < allBodies.size(); i++) {
		Vector2 acceleration = allBodies[i]->force * allBodies[i]->inverseMass;
		allBodies[i]->velocity += acceleration * dt;
		allBodies[i]->velocity *= allBodies[i]->damping;
	}
}

void GameSimsPhysics::IntegrateVelocity(float dt) {
	for (int i = 0; i < allBodies.size(); i++) {
		allBodies[i]->position += allBodies[i]->velocity * dt;
	}
}

void GameSimsPhysics::CollisionDetection(float dt) {
	for (int i = 0; i < allColliders.size(); ++i) {
		for (int j = i + 1; j < allColliders.size(); ++j) {
			CollisionVolume::CollisionPair* pair = new CollisionVolume::CollisionPair{};
			pair->object1 = allColliders[i];
			pair->object2 = allColliders[j];
			if (pair->object1->getPosition() != Vector2(0,0) && pair->object2->getPosition() != Vector2(0, 0)) {
				if (pair->object1->checkCollision(pair->object2, *pair)) {
					if (pair->penetration != 0) {
						allCollisionPairs.emplace_back(pair);
					}
				}
			}
		} 
	}
}

void GameSimsPhysics::CollisionResolution(float dt) {
	for (int i = 0; i < allCollisionPairs.size(); i ++) {
		if (game->HandleCollisionPair(allCollisionPairs[i])) {
			CollisionVolume::CollisionPair* pair = allCollisionPairs[i];
			RigidBody* object1 = allCollisionPairs[i]->object1->simObject;
			RigidBody* object2 = allCollisionPairs[i]->object2->simObject;

			// projection
			float totalMass = object1->inverseMass + object2->inverseMass;
			
			object1->SetPosition(object1->GetPosition() - (pair->normal * pair->penetration * (object1->inverseMass / totalMass)));
			object2->SetPosition(object2->GetPosition() + (pair->normal * pair->penetration * (object2->inverseMass / totalMass)));

			// impulses

			Vector2 relativeVelocity = object2->velocity - object1->velocity;
			float dotRelVelAndColNormal = Vector2::Dot(relativeVelocity, pair->normal);
			float coefficientOfRestitution = (object1->elasticity + object2->elasticity) / 2;
			float e = -1 * (1 + coefficientOfRestitution);
			float impulse = (e * dotRelVelAndColNormal) / totalMass;

			object1->SetVelocity(object1->GetVelocity() - (pair->normal * impulse * object1->inverseMass));
			object2->SetVelocity(object2->GetVelocity() + (pair->normal * impulse * object2->inverseMass));

		}
	}
	allCollisionPairs.clear();
}