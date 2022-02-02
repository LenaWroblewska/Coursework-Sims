#include "Fruit.h"
#include "TextureManager.h"
#include "GameSimsRenderer.h"
#include "CollisionVolume.h"
#include "FruitWizardGame.h"

using namespace NCL;
using namespace CSC3222;

Fruit::Fruit() : SimObject() {
	texture = texManager->GetTexture("FruitWizard\\Fruit.png");

	int fruitID = rand() % 16;

	animFrameData = Vector4((fruitID / 4) * 16.0f, (fruitID % 4) * 16.0f, 16.0f, 16.0f);
	collider = new CollisionVolume(this, CollisionVolume::Shapes::CIRCLE, CollisionVolume::Types::FRUIT, 5.0f);
	collider->setPosition(this->position);
}

Fruit::~Fruit() {
}

bool Fruit::UpdateObject(float dt) {
	collider->setPosition(this->GetPosition());
	if (collected) {
		game->currentScore += 1000;
		game->fruitLeft -= 1;
		return false; 
	}
	if (gameReset) {
		return false;
	}
	return true;
}

void Fruit::ResetGame() {
	this->gameReset = true;
}