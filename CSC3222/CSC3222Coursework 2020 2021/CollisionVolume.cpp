#include "CollisionVolume.h"
#include <Math.h>

using namespace NCL;
using namespace CSC3222;

CollisionVolume::CollisionVolume()
{
}

CollisionVolume::CollisionVolume(SimObject* simObj, CollisionVolume::Shapes shape, CollisionVolume::Types type, float width, float height)
{
	this->simObject = simObj;
	this->shape = shape;
	this->type = type;
	this->width = width;
	this->height = height;
}

CollisionVolume::CollisionVolume(SimObject* simObj, CollisionVolume::Shapes shape, CollisionVolume::Types type, float radius)
{
	this->simObject = simObj;
	this->shape = shape;
	this->type = type;
	this->radius = radius;
}

CollisionVolume::~CollisionVolume()
{
}

CollisionVolume::Shapes CollisionVolume::getShape()
{
	return shape; 
}

CollisionVolume::Types CollisionVolume::getType()
{
	return type;
}

float CollisionVolume::getRadius()
{
	return radius;
}

float CollisionVolume::getHeight()
{
	return height;
}

float CollisionVolume::getWidth()
{
	return width;
}

Vector2 CollisionVolume::getPosition()
{
	return position;
}

void CollisionVolume::setPosition(Vector2 position)
{
	this->position = position;
}

bool CollisionVolume::checkCollision(CollisionVolume* object, CollisionPair& pair)
{
	// Two circles
	if ((getShape() == Shapes::CIRCLE) && (object->getShape() == Shapes::CIRCLE))
	{
		// Calculate distance 
		float dx = getPosition().x - object->getPosition().x; 
		float dy = getPosition().y - object->getPosition().y;
		float distance = sqrt((dx * dx) + (dy * dy));

		// Check if greater than the two radii
		if (distance <= getRadius() + object->getRadius())
		{
			float p = ((getRadius() + object->getRadius()) - distance);
			pair.penetration = p;
			Vector2 normVector = (object->getPosition() - this->getPosition());
			pair.normal = normVector.Normalised();
			return true;
		}
		else
		{
			return false; 
		}
	}

	// Two quadrilaterals 

	// Get the top left corner of first rectangle
	float c1X = (getPosition().x - getWidth() / 2);
	float c1Y = (getPosition().y - getHeight() / 2);

	// Get top left corner of second 
	float c2X = (object->getPosition().x - object->getWidth() / 2);
	float c2Y = (object->getPosition().y - object->getHeight() / 2);

	if ((getShape() == Shapes::QUADRILATERAL) && (object->getShape() == Shapes::QUADRILATERAL))
	{
		// Check if any sides are overlapping 
		if ((c1X + getWidth() >= c2X) &&
			(c1X <= c2X + object->getWidth()) &&
			(c1Y + getHeight() >= c2Y) &&
			(c1Y <= c2Y + object->getHeight()))
		{

			Vector2 maxC1 = this->getPosition() + Vector2(this->getWidth() / 2, this->getHeight() / 2);
			Vector2 minC1 = this->getPosition() - Vector2(this->getWidth() / 2, this->getHeight() / 2);

			Vector2 maxC2 = object->getPosition() + Vector2(object->getWidth() / 2, object->getHeight() / 2);
			Vector2 minC2 = object->getPosition() - Vector2(object->getWidth()/2, object->getHeight()/2);


			float distances[4] = {
				(maxC2.x - minC1.x),
				(maxC1.x - minC2.x),
				(maxC2.y - minC1.y),
				(maxC1.y - minC2.y)
			};

			static  const  Vector2  faces[4] = 
			{
				Vector2(-1, 0), Vector2(1, 0),
				Vector2(0, -1), Vector2(0, 1)
			};

			float pen = FLT_MAX;
			Vector2 mostPenAxis;

			for (int i = 0; i < 4; i++) {
				if (distances[i] < pen) {
					pen = distances[i];
					mostPenAxis = faces[i];
				}
			}

			pair.penetration = pen;
			pair.normal = mostPenAxis; 
			return true;
		}
		else
		{
			return false;
		}
	}

	// A circle and a quadrilateral 
	if (((getShape() == Shapes::CIRCLE) && (object->getShape() == Shapes::QUADRILATERAL)) 
			|| ((getShape() == Shapes::QUADRILATERAL) && (object->getShape() == Shapes::CIRCLE)))
	{
		CollisionVolume* object1;
		CollisionVolume* object2; 

		Vector2 relativePos = Vector2(0, 0);
		// Place in correct order
		if (getShape() == Shapes::CIRCLE)
		{
			object1 = this;
			object2 = object; 
			relativePos = object2->getPosition() - object1->getPosition();
		}
		else
		{
			object1 = object;
			object2 = this; 
			relativePos = object1->getPosition() - object2->getPosition();
		}

		// Detect collision
		float testX = object1->getPosition().x; 
		float testY = object1->getPosition().y; 

		// Get top left of quadrilateral 
		float rX = object2->getPosition().x - (object2->getWidth() / 2);
		float rY = object2->getPosition().y - (object2->getHeight() / 2);

		// Decide which edges to check 
		if (object1->getPosition().x < rX)
		{
			testX = rX;
		}
		else if (object1->getPosition().x > rX + object2->getWidth())
		{
			testX = rX + object2->getWidth(); 
		}

		if (object1->getPosition().y < rY)
		{
			testY = rY; 
		}
		else if (object1->getPosition().y > rY + object2->getHeight())
		{
			testY = rY + object2->getHeight(); 
		}
		
		// Work out distance from edge 
		float distX = object1->getPosition().x - testX; 
		float distY = object1->getPosition().y - testY;
		float distance = sqrt((distX * distX) + (distY * distY));

		// Check if circle far away enough from edge 
		if (distance <= object1->getRadius())
		{
			Vector2 closestPointOnBox = Vector2(clamp(relativePos.x, -object2->getWidth() / 2, object2->getWidth() / 2),
				clamp(relativePos.y, -object2->getHeight() / 2, object2->getHeight() / 2));
			Vector2 localPoint = (relativePos - closestPointOnBox);

			pair.penetration = object1->getRadius() - localPoint.Length();
			pair.normal = localPoint.Normalised(); 
			return true;
		}
		else {
			return false; 
		}
	}

	else 
	{
		return false; 
	}
}

float CollisionVolume::clamp(float n, float lower, float upper) {
	return std::max(lower, std::min(n, upper));
}
