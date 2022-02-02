#pragma once
#include "SimObject.h"

namespace NCL {
	namespace CSC3222 {
		class CollisionVolume {
		public:
			
			enum class Shapes {
				QUADRILATERAL,
				CIRCLE,
				UNKNOWN
			};

			enum class Types {
				FROGGO,
				FRUIT,
				GUARD,
				PIXIE,
				DUST,
				PLAYER,
				SPELL,
				WALL, 
				LADDER,
				UNKNOWN
			};

			struct CollisionPair {
				CollisionVolume* object1;
				CollisionVolume* object2;

				float penetration;
				Vector2 normal;
			};

			CollisionVolume();
			CollisionVolume(SimObject* simObj, CollisionVolume::Shapes shape, CollisionVolume::Types type, float width, float height);
			CollisionVolume(SimObject* simObj, CollisionVolume::Shapes shape, CollisionVolume::Types type, float raidus);
			~CollisionVolume();

			float getRadius();
			float getWidth();
			float getHeight();
			Vector2 getPosition();
			void setPosition(Vector2 position);
			Shapes getShape();
			Types getType();
			bool checkCollision(CollisionVolume* object2, CollisionPair& pair);
		
			SimObject* simObject;
		protected:
			Vector2 position;
			float area; 
			float width;
			float height; 
			float radius; 
			Types type;
			Shapes shape;
			float clamp(float n, float lower, float upper);
		};
	}
}

