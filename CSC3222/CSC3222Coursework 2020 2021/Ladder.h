#pragma once
#include "CollisionVolume.h"
namespace NCL {
	namespace Maths {
		class Vector2;
	}
	namespace CSC3222 {
		class Ladder : public SimObject {
		public:
			Ladder();
			~Ladder();
			Ladder(double width, double height);

			void DrawObject(GameSimsRenderer& r) override;
			bool UpdateObject(float dt) override;

		private:
			float width;
			float height;
		};
	}
}
