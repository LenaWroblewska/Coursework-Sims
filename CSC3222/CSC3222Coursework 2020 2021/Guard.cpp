#include "Guard.h"
#include "TextureManager.h"
#include "GameSimsRenderer.h"
#include "CollisionVolume.h"
#include "FruitWizardGame.h"

using namespace NCL;
using namespace CSC3222;

Vector4 guardIdleFrames[] = {
	Vector4(384,32, 64, 32),
	Vector4(448,32, 64, 32),	
	Vector4(512,32, 64, 32),	
	Vector4(576,32, 64, 32),	
	Vector4(448,64, 64, 32),	
	Vector4(512,64, 64, 32),
};

Vector4 guardWalkFrames[] = {
	Vector4(384,96, 64, 32),
	Vector4(448,96, 64, 32),
	Vector4(512,96, 64, 32),
	Vector4(576,96, 64, 32),
	Vector4(640,96, 64, 32),
	Vector4(704,96, 64, 32),
};

Vector4 guardAttackFrames[] = {
	Vector4(384,128, 64, 32),
	Vector4(448,128, 64, 32),
	Vector4(512,128, 64, 32),
	Vector4(576,128, 64, 32),
	Vector4(640,128, 64, 32),
	Vector4(704,128, 64, 32),
};

Vector4 guardStunFrames[] = {
	Vector4(448,160, 64, 32),
	Vector4(448,160, 64, 32),
	Vector4(448,160, 64, 32),
	Vector4(448,160, 64, 32),
	Vector4(448,160, 64, 32),
	Vector4(448,160, 64, 32),
};

Guard::Guard(SimObject* player) : SimObject() {
	texture = texManager->GetTexture("FruitWizard\\mini_fantasy_sprites_oga_ver.png");
	animFrameCount	= 6;
	collider = new CollisionVolume(this, CollisionVolume::Shapes::QUADRILATERAL, CollisionVolume::Types::GUARD, 32.0f, 25.0f);
	collider->setPosition(this->position);
	startingPoint = Vector2(30 * 16, 20 * 16);
	this->player = player;
}

Guard::~Guard() {
}

bool Guard::UpdateObject(float dt) {
	animFrameData = guardWalkFrames[currentanimFrame];
	collider->setPosition(this->GetPosition());
	

	if (this->position.y < this->startingPoint.y && this->position != Vector2(0,0)) {
		startingPoint = this->position;
	}


	if (this->position.x < player->GetPosition().x) {
		distanceToPlayer = player->GetPosition() - this->GetPosition();
	}
	else {
		distanceToPlayer = this->GetPosition() - player->GetPosition();
	}

	switch (currentState) {
		case(Guard::states::WANDER):

			if (this->position.x < startingPoint.x) {
				distanceTravelled = startingPoint.x - this->GetPosition().x;
			}
			else {
				distanceTravelled = this->GetPosition().x - startingPoint.x;
			}

			if (distanceTravelled > 130) {
				if (distanceTravelled > 160 && turned == false) {
					moveRight = !moveRight;
					this->distanceTravelled = 0.0f;
					turned = true;
				}
			}
			else {
				turned = false;
			}

			if (moveRight) {
				flipAnimFrame = true;
				AddForce(Vector2(50, 0));
			}
			else {
				flipAnimFrame = false;
				AddForce(Vector2(-50, 0));
			}

			if (distanceToPlayer.x <= 48 && (distanceToPlayer.y <= 8 && distanceToPlayer.y > -8)) {
				currentState = Guard::states::ATTACK;
			}

			break;

			
		case(Guard::states::ATTACK):
			animFrameData = guardAttackFrames[currentanimFrame];
			
			if (this->position.x < player->GetPosition().x) {
				moveRight = true;
			}
			else {
				moveRight = false;
			}

			if (moveRight) {
				flipAnimFrame = true;
				AddForce(Vector2(10, 0));
			}
			else {
				flipAnimFrame = false;
				AddForce(Vector2(-10, 0));
			}


			if (distanceToPlayer.x > 48 || (distanceToPlayer.y > 8 && distanceToPlayer.y > -8)) {
				currentState = Guard::states::WANDER;
			}
			
			break;

		case(Guard::states::STUN):
			animFrameData = guardStunFrames[currentanimFrame];
			timer += dt;
			// Gravity 
			AddForce(Vector2(0.0f, -20.0f));
			if (timer >= stunTimer) {
				timer = 0;
				currentState = Guard::states::WANDER;
			}
			break;

	}


	if (gameReset) {
		return false;
	}
	return true;
}

void Guard::ResetGame() {
	this->gameReset = true;
}