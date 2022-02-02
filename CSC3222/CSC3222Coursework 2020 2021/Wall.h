#pragma once
#include "CollisionVolume.h"
namespace NCL {
	namespace Maths {
		class Vector2;
	}
	namespace CSC3222 {
		class Wall : public SimObject {
		public:
			Wall();
			~Wall();
			Wall(double width, double height);

			void DrawObject(GameSimsRenderer& r) override;
			bool UpdateObject(float dt) override;

		private:
			float width;
			float height;
		};
	}
}
