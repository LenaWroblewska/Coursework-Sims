#pragma once
#include "SimObject.h"

namespace NCL {
	namespace Maths {
		class Vector2;
	}
	namespace CSC3222 {
		class Spell : public SimObject {
		public:
			Spell(Vector2 direction);
			~Spell();

			void DrawObject(GameSimsRenderer &r) override;
			bool UpdateObject(float dt) override;
			bool hitEnemy = false;

			int bounces = 0;

		private:
			Vector2 spellForce; 
			float forceMultiplier;
		};
	}
}

