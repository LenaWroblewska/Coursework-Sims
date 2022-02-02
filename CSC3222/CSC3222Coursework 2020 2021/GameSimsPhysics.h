#pragma once
#include <vector>
#include "../../Common/Vector2.h"
#include "FruitWizardGame.h"

namespace NCL {
	namespace CSC3222 {
		class RigidBody;
		class CollisionVolume;

		class GameSimsPhysics	{
		public:
			GameSimsPhysics();
			GameSimsPhysics(FruitWizardGame* game);
			~GameSimsPhysics();

			void Update(float dt);
			void FixedUpdate(float dt);

			void AddRigidBody(RigidBody* b);
			void RemoveRigidBody(RigidBody* b);

			void AddCollider(CollisionVolume* c);
			void RemoveCollider(CollisionVolume* c);

		protected:
			void Integration(float dt);
			void IntegrateVelocity(float dt);
			void CollisionDetection(float dt);
			void CollisionResolution(float dt);

			float timeRemaining = 0.0f;

			std::vector<RigidBody*>			allBodies;
			std::vector<CollisionVolume*>	allColliders;
			std::vector<CollisionVolume::CollisionPair*> allCollisionPairs;

			FruitWizardGame* game = nullptr;
		};
	}
}

