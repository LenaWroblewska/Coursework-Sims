#pragma once
#include "SimObject.h"
#include <vector>

namespace NCL::CSC3222 {
	class Pixie : public SimObject	{
	public:
		Pixie();
		~Pixie();

		bool UpdateObject(float dt) override;
		bool collected = false;

	protected:
		Vector2 Alignment(std::vector<SimObject*> allBoids);
		Vector2 Separation(std::vector<SimObject*> allBoids);
		Vector2 Avoidance(std::vector<SimObject*> allEnemies);
		void Flocking(float dt, std::vector<SimObject*> allBoids, std::vector<SimObject*> allEnemies);

		float alignmentThreshold = 10.0f;
		float separationThreshold = 10.0f;
		float avoidanceThreshold = 50.0f;
	};
}