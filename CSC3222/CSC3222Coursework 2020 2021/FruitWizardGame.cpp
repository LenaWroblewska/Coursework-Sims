#include "FruitWizardGame.h"
#include "SimObject.h"
#include "GameMap.h"
#include "TextureManager.h"
#include "GameSimsRenderer.h"
#include "GameSimsPhysics.h"
#include "PlayerCharacter.h"
#include "Fruit.h"
#include "Guard.h"
#include "Froggo.h"
#include "Pixie.h"
#include "PixieDust.h"
#include "Spell.h"
#include "CollisionVolume.h"
#include "Wall.h"
#include "Ladder.h"

#include "../../Common/Window.h"
#include "../../Common/TextureLoader.h"

using namespace NCL;
using namespace CSC3222;

FruitWizardGame::FruitWizardGame()	{
	renderer	= new GameSimsRenderer();
	texManager	= new TextureManager();
	physics		= new GameSimsPhysics(this);
	SimObject::InitObjects(this, texManager);
	InitialiseGame();
}

FruitWizardGame::~FruitWizardGame()	{
	delete currentMap;
	delete texManager;
	delete renderer;
	delete physics;
}


void FruitWizardGame::Update(float dt) {
	for (auto i : newObjects) {
		gameObjects.emplace_back(i);
	}
	newObjects.clear();

	gameTime += dt;

	renderer->Update(dt);
	physics->FixedUpdate(dt);
	currentMap->DrawMap(*renderer);

	srand((int)(gameTime * 1000.0f));

	for (auto i = gameObjects.begin(); i != gameObjects.end(); ) {
		(*i)->UpdateAnimFrame(dt);
		if (!(*i)->UpdateObject(dt)) { //object has said its finished with
			delete (*i);
			 i = gameObjects.erase(i);
		}
		else {
			(*i)->DrawObject(*renderer);
			CollisionVolume* collider = (*i)->GetCollider();
			if (collider) {
				if (collider->getShape() == CollisionVolume::Shapes::CIRCLE) {
					renderer->DrawCircle(collider->getPosition(), collider->getRadius(), Vector4(1, 0, 1, 1));
				}
				else if (collider->getShape() == CollisionVolume::Shapes::QUADRILATERAL) {
					renderer->DrawBox(collider->getPosition(), Vector2(collider->getWidth()/2, collider->getHeight()/2), Vector4(1, 0, 1, 1)); 
				}
			}

			++i;
		}
	}	
	renderer->DrawString("Score:" + std::to_string(currentScore), 
		Vector2(32, 12), Vector4(1,1,1,1), 100.0f);
	renderer->DrawString("Lives:" + std::to_string(lives), 
		Vector2(144, 12), Vector4(1, 0,0, 1), 100.0f);
	renderer->DrawString("Magic:" + std::to_string(magicCount), 
		Vector2(256, 12), Vector4(1, 0, 0.5f, 1), 100.0f);
	renderer->DrawString("Dust:"  + std::to_string(dustCount), 
		Vector2(366, 12), Vector4(0.5f, 0.3f, 0.8f, 1), 100.0f);


	/*
	Some examples of debug rendering! 
	These all use coordinates in the same 'space' as the game positions
	*/
	/*
	renderer->DrawBox(Vector2(16,16), Vector2(8, 8), Vector4(1, 0, 0, 1));
	renderer->DrawLine(Vector2(16, 16), Vector2(192, 192), Vector4(1, 1, 0, 1));
	renderer->DrawCircle(Vector2(100, 100), 10.0f, Vector4(1, 0, 1, 1));

	renderer->DrawCircle(player->GetPosition(), 10.0f, Vector4(1, 0, 1, 1));
	renderer->DrawBox(player->GetPosition(), Vector2(8, 8), Vector4(1, 0, 0, 1));*/

	renderer->Render();

	// Timer to stop game reset triggering multiple times
	if (gameReset) {
		gameResetTimer += dt;
		if (gameResetTimer >= gameResetTimerLength) {
			gameReset = false;
			gameResetTimer = 0;
		}
	}

	// Pixie dust timer
	pixieDustTimer += dt;
	if (pixieDustTimer >= pixieDustTimerLength) {
		pixieDustTimer = 0;
		// spawn pixie dust
		this->placeDust();
	}
}

void FruitWizardGame::pixieMode() {
	this->dustCount -= 4;
	// Randomly place 8 pixies
	std::vector<Vector2> spawnSpaces = currentMap->FindSpawnSpace();
	std::vector<int> usedSpawnPositions;
	srand(time(NULL));
	for (int i = 0; i < 8; i++) {
		bool spawnChosen = false;

		float spawnPoint;
		while (!spawnChosen) {
			spawnPoint = rand() % spawnSpaces.size();
			spawnChosen = true;

			for (int i = 0; i < usedSpawnPositions.size(); i++) {
				if (spawnPoint == usedSpawnPositions[i]) {
					spawnChosen = false;
				}
			}
		}

		Pixie* pixie = new Pixie();
		pixie->SetPosition(spawnSpaces[spawnPoint]);
		pixie->SetSpring(player);
		AddNewObject(pixie);
		pixies.emplace_back(pixie);
		usedSpawnPositions.emplace_back(spawnPoint);
	}
}

void FruitWizardGame::placeDust() {
	// Randomly place dust 
	std::vector<Vector2> spawnSpaces = currentMap->FindSpawnSpace();
	srand(time(NULL));
	bool spawnChosen = false;

	float spawnChance = rand() % 4;
	if (spawnChance >= 2) {
		PixieDust* testFruit = new PixieDust();
		testFruit->SetPosition(spawnSpaces[rand() % spawnSpaces.size()]);
		AddNewObject(testFruit);
	}
	spawnChosen = true;
}

void FruitWizardGame::placeFruit(int numberOfFruit) {
	// Randomly place 16 fruit
	std::vector<Vector2> spawnSpaces = currentMap->FindSpawnSpace();
	std::vector<int> usedSpawnPositions;
	srand(time(NULL));
	for (int i = 0; i < numberOfFruit; i++) {
		bool spawned = false;

		float spawnPoint;
		while (!spawned) {
			spawnPoint = rand() % spawnSpaces.size();
			spawned = true;

			for (int i = 0; i < usedSpawnPositions.size(); i++) {
				if (spawnPoint == usedSpawnPositions[i]) {
					spawned = false;
				}
			}
		}

		Fruit* testFruit = new Fruit();
		testFruit->SetPosition(spawnSpaces[spawnPoint]);
		AddNewObject(testFruit);
		usedSpawnPositions.emplace_back(spawnPoint);
		fruitLeft = numberOfFruit;
	}
}

void FruitWizardGame::redistributeFruit() {
	// Randomly place remaining fruit 
}

void FruitWizardGame::placeNewGuard() {
	// Randomly place a new guard 
}

void FruitWizardGame::placeGuards(int guardAmount) {
	// Randomly place all guards
	std::vector<Vector2> spawnSpaces = currentMap->FindSpawnSpace();
	std::vector<int> usedSpawnPositions;
	srand(time(NULL));
	for (int i = 0; i < guardAmount; i++) {
		bool spawned = false;

		float spawnPoint;
		while (!spawned) {
			spawnPoint = rand() % spawnSpaces.size();
			spawned = true;

			for (int i = 0; i < usedSpawnPositions.size(); i++) {
				if (spawnPoint == usedSpawnPositions[i]) {
					spawned = false;
				}
			}
		}

		Guard* testGuard = new Guard(player);
		testGuard->SetPosition(spawnSpaces[spawnPoint] + Vector2(0, 5));
		AddNewObject(testGuard);
		guards.emplace_back(testGuard);
		usedSpawnPositions.emplace_back(spawnPoint);
		guardNumberLeft++;
	}
}

void FruitWizardGame::InitialiseGame() {
	delete currentMap;
	for (auto o : gameObjects) {
		delete o;
	}
	gameObjects.clear();

	currentMap = new GameMap("FruitWizardMap.txt", gameObjects, *texManager);

	renderer->SetScreenProperties(16, currentMap->GetMapWidth(), currentMap->GetMapHeight());

	player = new PlayerCharacter();
	player->SetPosition(Vector2(100, 32));
	AddNewObject(player);

	/*Guard* testGuard = new Guard();
	testGuard->SetPosition(Vector2(150, 224));
	AddNewObject(testGuard);*/

	/*
	Spell* testSpell = new Spell(Vector2(1,0));
	testSpell->SetPosition(Vector2(160, 48));
	AddNewObject(testSpell);*/

	/*Fruit* testFruit = new Fruit();
	testFruit->SetPosition(Vector2(250, 150));
	AddNewObject(testFruit);*/

	/*PixieDust* testDust = new PixieDust();
	testDust->SetPosition(Vector2(285, 220));
	AddNewObject(testDust);*/

	/*Pixie* pixie = new Pixie();
	pixie->SetPosition(Vector2(350, 96));
	pixie->SetSpring(player);
	AddNewObject(pixie);*/

	/* Froggo* testFroggo = new Froggo();
	testFroggo->SetPosition(Vector2(370, 285));
	AddNewObject(testFroggo); */

	// LEFT WALL
	Wall* wallLeft = new Wall(16, 320);
	wallLeft->SetPosition(Vector2(8, 160));
	AddNewObject(wallLeft);

	// RIGHT WALL
	Wall* wallRight = new Wall(16, 320);
	wallRight->SetPosition(Vector2(472, 160));
	AddNewObject(wallRight);

	// FLOORS
	Wall* floor1 = new Wall(440, 16);
	floor1->SetPosition(Vector2(240, 8));
	AddNewObject(floor1);

	Wall* floor2 = new Wall(70, 16);
	floor2->SetPosition(Vector2(60, 72));
	AddNewObject(floor2);

	Wall* floor3 = new Wall(20, 16);
	floor3->SetPosition(Vector2(135, 72));
	AddNewObject(floor3);

	Wall* floor4 = new Wall(60, 16);
	floor4->SetPosition(Vector2(212, 72));
	AddNewObject(floor4);

	Wall* floor5 = new Wall(175, 16);
	floor5->SetPosition(Vector2(365, 72));
	AddNewObject(floor5);

	Wall* floor6 = new Wall(170, 16);
	floor6->SetPosition(Vector2(265, 136));
	AddNewObject(floor6);

	Wall* floor7 = new Wall(70, 16);
	floor7->SetPosition(Vector2(420, 136));
	AddNewObject(floor7);

	Wall* floor8 = new Wall(125, 16);
	floor8->SetPosition(Vector2(114, 200));
	AddNewObject(floor8);

	Wall* floor9 = new Wall(15, 16);
	floor9->SetPosition(Vector2(213, 200));
	AddNewObject(floor9);

	Wall* floor10 = new Wall(18, 16);
	floor10->SetPosition(Vector2(282, 200));
	AddNewObject(floor10);

	Wall* floor11 = new Wall(48, 16);
	floor11->SetPosition(Vector2(345, 200));
	AddNewObject(floor11);

	Wall* floor12 = new Wall(64, 16);
	floor12->SetPosition(Vector2(50, 264));
	AddNewObject(floor12);

	Wall* floor13 = new Wall(16, 16);
	floor13->SetPosition(Vector2(117, 264));
	AddNewObject(floor13);

	Wall* floor14 = new Wall(210, 16);
	floor14->SetPosition(Vector2(315, 264));
	AddNewObject(floor14);

	Wall* floor15 = new Wall(13, 16);
	floor15->SetPosition(Vector2(453, 264));
	AddNewObject(floor15);

	// Ladders 
	Ladder* ladder1 = new Ladder(20, 60);
	ladder1->SetPosition(Vector2(110, 50));
	AddNewObject(ladder1);

	Ladder* ladder2 = new Ladder(20, 60);
	ladder2->SetPosition(Vector2(260, 50));
	AddNewObject(ladder2);

	Ladder* ladder3 = new Ladder(20, 60);
	ladder3->SetPosition(Vector2(370, 114));
	AddNewObject(ladder3);

	Ladder* ladder4 = new Ladder(20, 120);
	ladder4->SetPosition(Vector2(30, 150));
	AddNewObject(ladder4);

	Ladder* ladder5 = new Ladder(20, 60);
	ladder5->SetPosition(Vector2(190, 180));
	AddNewObject(ladder5);

	Ladder* ladder6 = new Ladder(20, 60);
	ladder6->SetPosition(Vector2(305, 180));
	AddNewObject(ladder6);

	Ladder* ladder7 = new Ladder(20, 120);
	ladder7->SetPosition(Vector2(433, 210));
	AddNewObject(ladder7);

	Ladder* ladder8 = new Ladder(20, 60);
	ladder8->SetPosition(Vector2(97, 242));
	AddNewObject(ladder8);

	placeFruit(noOfFruit);
	placeGuards(4);

	gameTime		= 0;
	currentScore	= 0;
	magicCount		= 4;
	dustCount		= 4;
	lives			= 3;
	guardNumber		= 0;
	guardNumberLeft = 0;
}

void FruitWizardGame::ResetGame() {
	player->SetPosition(Vector2(100, 32));

	float guards = 0;
	float fruits = 0;
	for (int i = 0; i < gameObjects.size(); i++) {
		Guard* guard = dynamic_cast<Guard*>(gameObjects[i]);
		if (guard) {
			guard->ResetGame();
			guards++;
			continue;
		}
		Fruit* fruit = dynamic_cast<Fruit*>(gameObjects[i]);
		if (fruit) {
			fruit->ResetGame();
			fruits++;
			continue;
		}
	}

	placeFruit(fruits);
	placeGuards(guards);
}

void FruitWizardGame::AddNewObject(SimObject* object) {
	newObjects.emplace_back(object);
	physics->AddRigidBody(object);
	if (object->GetCollider()) {
		physics->AddCollider(object->GetCollider());
	}
}

bool FruitWizardGame::HandleCollisionPair(CollisionVolume::CollisionPair* pair) {
	CollisionVolume* object1 = pair->object1;
	CollisionVolume* object2 = pair->object2;


	// Decides if collisions are applied between game objects 
	// true = collisions applied 

	// Wall and spell
	if (object1->getType() == CollisionVolume::Types::SPELL || object2->getType() == CollisionVolume::Types::SPELL) {
		if (object1->getType() == CollisionVolume::Types::WALL || object2->getType() == CollisionVolume::Types::WALL) {
			Spell* spell1 = dynamic_cast<Spell*>(object1->simObject);
			Spell* spell2 = dynamic_cast<Spell*>(object2->simObject);

			if (spell1) {
				spell1->bounces++; 
			}
			else if (spell2) {
				spell2->bounces++;
			}

			return true;
		}
	}

	// Spell & Guard
	if (object1->getType() == CollisionVolume::Types::SPELL || object2->getType() == CollisionVolume::Types::SPELL) {
		if (object1->getType() == CollisionVolume::Types::GUARD || object2->getType() == CollisionVolume::Types::GUARD) {
			Spell* spell1 = dynamic_cast<Spell*>(object1->simObject);
			Spell* spell2 = dynamic_cast<Spell*>(object2->simObject);
			Guard* guard1 = dynamic_cast<Guard*>(object1->simObject);
			Guard* guard2 = dynamic_cast<Guard*>(object2->simObject);

			if (spell1) {
				spell1->hitEnemy = true;
			}
			else if (spell2) {
				spell2->hitEnemy = true;
			}
			if (guard1) {
				if (guard1->currentState != Guard::states::STUN) {
					this->currentScore += 200;
					guard1->currentState = Guard::states::STUN;
				}
			}
			else if (guard2) {
				if (guard2->currentState != Guard::states::STUN) {
					this->currentScore += 200;
					guard2->currentState = Guard::states::STUN;
				}
			}
			return true;
		}
	}

	// Spell & Froggo
	if (object1->getType() == CollisionVolume::Types::SPELL || object2->getType() == CollisionVolume::Types::SPELL) {
		if (object1->getType() == CollisionVolume::Types::FROGGO || object2->getType() == CollisionVolume::Types::FROGGO) {
			Spell* spell1 = dynamic_cast<Spell*>(object1->simObject);
			Spell* spell2 = dynamic_cast<Spell*>(object2->simObject);
			if (spell1) {
				spell1->hitEnemy = true;
			}
			else if (spell2) {
				spell2->hitEnemy = true;
			} 
			return true;
		}
	}

	// Player & wall
	if (object1->getType() == CollisionVolume::Types::PLAYER || object2->getType() == CollisionVolume::Types::PLAYER) {
		if (object1->getType() == CollisionVolume::Types::WALL || object2->getType() == CollisionVolume::Types::WALL) {
			return true;
		}
	}

	// Guard & wall
	if (object1->getType() == CollisionVolume::Types::GUARD || object2->getType() == CollisionVolume::Types::GUARD) {
		if (object1->getType() == CollisionVolume::Types::WALL || object2->getType() == CollisionVolume::Types::WALL) {
			return true;
		}
	}

	// Player & Guard
	if (object1->getType() == CollisionVolume::Types::PLAYER || object2->getType() == CollisionVolume::Types::PLAYER) {
		if (object1->getType() == CollisionVolume::Types::GUARD || object2->getType() == CollisionVolume::Types::GUARD) {
			PlayerCharacter* player1 = dynamic_cast<PlayerCharacter*>(object1->simObject);
			PlayerCharacter* player2 = dynamic_cast<PlayerCharacter*>(object2->simObject);
			Guard* guard1 = dynamic_cast<Guard*>(object1->simObject);
			Guard* guard2 = dynamic_cast<Guard*>(object2->simObject);

			if (guard1) {
				if (guard1->currentState != Guard::states::STUN) {
					if (player1) {
						if (!gameReset) {
							player1->playerHit = true;
							this->lives -= 1;
							gameReset = true;
							this->ResetGame();
							return true;
						}
					}
					else if (player2) {
						if (!gameReset) {
							player1->playerHit = true;
							this->lives -= 1;
							gameReset = true;
							this->ResetGame();
							return true;
						}
					}
				}
			}
			else if (guard2) {
				if (guard2->currentState != Guard::states::STUN) {
					if (player1) {
						if (!gameReset) {
							player1->playerHit = true;
							this->lives -= 1;
							gameReset = true;
							this->ResetGame();
							return true;
						}
					}
					else if (player2) {
						if (!gameReset) {
							player1->playerHit = true;
							this->lives -= 1;
							gameReset = true;
							this->ResetGame();
							return true;
						}
					}
				}
			}
			return false;
		}
	}

	// Player & Froggo
	if (object1->getType() == CollisionVolume::Types::PLAYER || object2->getType() == CollisionVolume::Types::PLAYER) {
		if (object1->getType() == CollisionVolume::Types::FROGGO || object2->getType() == CollisionVolume::Types::FROGGO) {
			PlayerCharacter* player1 = dynamic_cast<PlayerCharacter*>(object1->simObject);
			PlayerCharacter* player2 = dynamic_cast<PlayerCharacter*>(object2->simObject);

			if (player1) {
				player1->playerHit = true;
				this->lives -= 1;
				if (!gameReset) {
					gameReset = true;
					this->ResetGame();
				}
				//reset game 
			}
			else if (player2) {
				player2->playerHit = true;
				this->lives -= 1;
				if (!gameReset) {
					gameReset = true;
					this->ResetGame();
				}
				// reset game 
			}

			return true;
		}
	}

	// Guard & Froggo
	if (object1->getType() == CollisionVolume::Types::GUARD || object2->getType() == CollisionVolume::Types::GUARD) {
		if (object1->getType() == CollisionVolume::Types::FROGGO || object2->getType() == CollisionVolume::Types::FROGGO) {
			return false;
		}
	}

	// Guard & Guard 
	if (object1->getType() == CollisionVolume::Types::GUARD && object2->getType() == CollisionVolume::Types::GUARD) {
		return false;
	}

	// Anything with unknown
	if (object1->getType() == CollisionVolume::Types::UNKNOWN || object2->getType() == CollisionVolume::Types::UNKNOWN) {
		//return true;
	}

	// Player and ladder 
	if (object1->getType() == CollisionVolume::Types::PLAYER || object2->getType() == CollisionVolume::Types::PLAYER) {
		if (object1->getType() == CollisionVolume::Types::LADDER || object2->getType() == CollisionVolume::Types::LADDER) {
			PlayerCharacter* player1 = dynamic_cast<PlayerCharacter*>(object1->simObject);
			PlayerCharacter* player2 = dynamic_cast<PlayerCharacter*>(object2->simObject);

			if (player1) {
				player1->onLadder = true;
			}
			else if (player2) {
				player1->onLadder = true;
			}
			return false;
		}
	}

	// Player and fruit
	if (object1->getType() == CollisionVolume::Types::FRUIT || object2->getType() == CollisionVolume::Types::FRUIT) {
		if (object1->getType() == CollisionVolume::Types::PLAYER || object2->getType() == CollisionVolume::Types::PLAYER) {
			Fruit* fruit1 = dynamic_cast<Fruit*>(object1->simObject);
			Fruit* fruit2 = dynamic_cast<Fruit*>(object2->simObject);

			if (fruit1) {
				fruit1->collected = true;
			}
			else if (fruit2) {
				fruit2->collected = true;
			}

			if (fruitLeft == 0) {
				this->currentScore += 5000;
				Froggo* testFroggo = new Froggo();
				testFroggo->SetPosition(Vector2(370, 285));
				AddNewObject(testFroggo);

				placeGuards(1);
				placeFruit(16);

			}
		
		}
	}

	// Pixie Dust & Player 
	if (object1->getType() == CollisionVolume::Types::DUST || object2->getType() == CollisionVolume::Types::DUST) {
		if (object1->getType() == CollisionVolume::Types::PLAYER || object2->getType() == CollisionVolume::Types::PLAYER) {
			PixieDust* dust1 = dynamic_cast<PixieDust*>(object1->simObject);
			PixieDust* dust2 = dynamic_cast<PixieDust*>(object2->simObject);

			if (dust1) {
				dust1->collected = true;
			}
			else if (dust2) {
				dust2->collected = true;
			}


			return true;
		}
	}

	// Pixie & Player 
	if (object1->getType() == CollisionVolume::Types::PIXIE || object2->getType() == CollisionVolume::Types::PIXIE) {
		if (object1->getType() == CollisionVolume::Types::PLAYER || object2->getType() == CollisionVolume::Types::PLAYER) {
			Pixie* pixie1 = dynamic_cast<Pixie*>(object1->simObject);
			Pixie* pixie2 = dynamic_cast<Pixie*>(object2->simObject);

			if (pixie1) {
				pixie1->collected = true;
			}
			else if (pixie2) {
				pixie2->collected = true;
			}
		}
	}

	return false;
}