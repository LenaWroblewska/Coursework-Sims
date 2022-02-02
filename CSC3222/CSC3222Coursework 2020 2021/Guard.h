#pragma once
#include "SimObject.h"

namespace NCL::CSC3222 {
	class Guard : public SimObject	{
	public:
		Guard(SimObject* player);
		~Guard();

		enum class states {
			WANDER,
			ATTACK,
			STUN
		};

		Guard::states currentState = Guard::states::WANDER;;

		bool UpdateObject(float dt) override;

		void ResetGame();
	protected:
		bool gameReset = false;
		bool moveRight = false;
		bool turned = false;
		SimObject* player;

		Vector2 startingPoint;
		float distanceTravelled;
		Vector2 distanceToPlayer;

		float timer = 0.0f;
		float stunTimer = 5.0f;
	};
}