#pragma once
#include "SimObject.h"

namespace NCL::CSC3222 {
	class PlayerCharacter : public SimObject	{
	public:
		PlayerCharacter();
		~PlayerCharacter();

		bool UpdateObject(float dt) override;
		bool playerHit = false;
		bool onLadder = false; 

	protected:
		enum class PlayerState {
			Left,
			Right,
			Attack,
			Fall,
			Die,
			Idle
		};
		PlayerState		currentAnimState;
	};
}