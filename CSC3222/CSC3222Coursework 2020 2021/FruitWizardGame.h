#pragma once
#include <vector>
#include "CollisionVolume.h"

namespace NCL::CSC3222 {
	class GameMap;
	class SimObject;
	class GameSimsRenderer;
	class GameSimsPhysics;
	class TextureBase;
	class TextureManager;
	class PlayerCharacter;

	class FruitWizardGame {
	public:
		FruitWizardGame();
		~FruitWizardGame();

		void Update(float dt);

		void AddNewObject(SimObject* object);

		bool HandleCollisionPair(CollisionVolume::CollisionPair* pair);

		void pixieMode();
		void placeFroggo();
		void placeFruit(int numberOfFruit);
		void redistributeFruit();
		void placeNewGuard();
		void placeGuards(int numberOfFruit);
		void placeDust();

		int noOfFruit = 16; 
		int fruitLeft = 16; 
		int guardNumber = 4;
		int guardNumberLeft = 0;
		int getLives() { return lives; }
		void ResetGame();

		int dustCount;
		int magicCount;
		int currentScore;

		std::vector<SimObject*> pixies;
		std::vector<SimObject*> guards;
	protected:
		void InitialiseGame();

		TextureManager*		texManager;
		GameSimsRenderer*	renderer;
		GameSimsPhysics*	physics;
		GameMap* currentMap;

		PlayerCharacter* player;

		float gameTime;

		int lives;

		bool gameReset = false;
		float gameResetTimer = 0.0f;
		float gameResetTimerLength = 0.5f;

		float pixieDustTimer = 0.0f;
		float pixieDustTimerLength = 30.0f;

		std::vector<SimObject*> gameObjects;
		std::vector<SimObject*> newObjects;
	};
}