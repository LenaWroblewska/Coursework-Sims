#include "Froggo.h"
#include "TextureManager.h"
#include "GameSimsRenderer.h"
#include "CollisionVolume.h"

using namespace NCL;
using namespace CSC3222;

Vector4 froggoIdleFrames[] = {
	Vector4(0  ,(96 * 5) + 32, 80, 64),
	Vector4(80 ,(96 * 5) + 32, 80, 64),
	Vector4(160,(96 * 5) + 32, 80, 64),
	Vector4(240,(96 * 5) + 32, 80, 64),
	Vector4(320,(96 * 5) + 32, 80, 64),
	Vector4(400,(96 * 5) + 32, 80, 64),
};

Vector4 froggoWalkFrames[] = {
	Vector4(0  ,(96 * 5) + 32, 80, 64),
	Vector4(80 ,(96 * 5) + 32, 80, 64),
	Vector4(160,(96 * 5) + 32, 80, 64),
	Vector4(240,(96 * 5) + 32, 80, 64),
	Vector4(320,(96 * 5) + 32, 80, 64),
	Vector4(400,(96 * 5) + 32, 80, 64),
};

Vector4 froggoLadderFrames[] = {
	Vector4(0  ,(96 * 4) + 32, 80, 64),
	Vector4(80 ,(96 * 4) + 32, 80, 64),
	Vector4(160,(96 * 4) + 32, 80, 64),
	Vector4(240,(96 * 4) + 32, 80, 64),
	Vector4(320,(96 * 4) + 32, 80, 64),
	Vector4(400,(96 * 4) + 32, 80, 64),
};

Vector4 froggoStunFrames[] = {
	Vector4(0  ,(96 * 8) + 32, 80, 64),
	Vector4(80, (96 * 8) + 32, 80, 64),
	Vector4(160,(96 * 8) + 32, 80, 64),
	Vector4(240,(96 * 8) + 32, 80, 64),
	Vector4(320,(96 * 8) + 32, 80, 64),
	Vector4(400,(96 * 8) + 32, 80, 64),
};

Vector4 froggoAttackFrames[] = {
	Vector4(0  ,(96 * 1) + 32, 80, 64),
	Vector4(80 ,(96 * 1) + 32, 80, 64),
	Vector4(80 ,(96 * 1) + 32, 80, 64),
	Vector4(160,(96 * 1) + 32, 80, 64),
	Vector4(160,(96 * 1) + 32, 80, 64),
	Vector4(240,(96 * 1) + 32, 80, 64),
};

Vector4 froggoJumpFrames[] = {
	Vector4(0  ,(96 * 7) + 32, 80, 64),
	Vector4(80 ,(96 * 7) + 32, 80, 64),
	Vector4(80 ,(96 * 7) + 32, 80, 64),
	Vector4(160,(96 * 7) + 32, 80, 64),
	Vector4(160,(96 * 7) + 32, 80, 64),
	Vector4(240,(96 * 7) + 32, 80, 64),
};

Froggo::Froggo() : SimObject() {
	texture			 = texManager->GetTexture("FruitWizard\\frogman_crown.png");
	animFrameCount	 = 6;
	collider = new CollisionVolume(this, CollisionVolume::Shapes::QUADRILATERAL, CollisionVolume::Types::FROGGO, 26.0f, 30.0f);
	collider->setPosition(this->position);
}

Froggo::~Froggo() {
}

bool Froggo::UpdateObject(float dt) {
	animFrameData = froggoWalkFrames[currentanimFrame];
	collider->setPosition(this->GetPosition());
	// Gravity 
	AddForce(Vector2(0.0f, -20.0f));

	return true;
}