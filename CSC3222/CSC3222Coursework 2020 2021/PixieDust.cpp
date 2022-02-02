#include "PixieDust.h"
#include "TextureManager.h"
#include "GameSimsRenderer.h"
#include "CollisionVolume.h"
#include "FruitWizardGame.h"

using namespace NCL;
using namespace CSC3222;

Vector4 animFrames[] = {
	Vector4(64 , 192, 32, 32),
	Vector4(96, 192, 32, 32),
	Vector4(128, 192, 32, 32),
	Vector4(160, 192, 32, 32)
};

PixieDust::PixieDust() : SimObject() {
	animFrameCount = 4;
	texture = texManager->GetTexture("FruitWizard\\mini_fantasy_sprites_oga_ver.png");
	collider = new CollisionVolume(this, CollisionVolume::Shapes::CIRCLE, CollisionVolume::Types::DUST, 10.0f);
	collider->setPosition(this->position);
}

PixieDust::~PixieDust() {
}

bool PixieDust::UpdateObject(float dt) {
	animFrameData = animFrames[currentanimFrame];
	collider->setPosition(this->GetPosition());

	if (collected) {
		game->currentScore += 500;
		game->dustCount += 1;
		return false;
	}

	return true;
}