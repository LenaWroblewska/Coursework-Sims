#include "PlayerCharacter.h"
#include "TextureManager.h"
#include "../../Common/Window.h"
#include "GameSimsRenderer.h"
#include "CollisionVolume.h"
#include "Spell.h"
#include "FruitWizardGame.h"

using namespace NCL;
using namespace CSC3222;

Vector4 runFrames[] = {
	Vector4(64,160, 32, 32),
	Vector4(96,160, 32, 32),
	Vector4(128,160, 32, 32),
	Vector4(160,160, 32, 32),
	Vector4(192,160, 32, 32),
	Vector4(224,160, 32, 32),
};

Vector4 attackFrames[] = {
	Vector4(128,288, 32, 30),
	Vector4(64,224, 32, 32),
	Vector4(160,288, 32, 30),
	Vector4(96,224, 32, 32),
	Vector4(192,288, 32, 29),
	Vector4(64,256, 32, 32)
};

Vector4 idleFrames[] = {
	Vector4(64,128, 32, 32),
	Vector4(96,128, 32, 32),
	Vector4(128,128, 32, 32),
	Vector4(160,128, 32, 32),
	Vector4(128,128, 32, 32),
	Vector4(224,128, 32, 32)
};

Vector4 fallFrames[] = {
	Vector4(64,320, 32, 32),
	Vector4(64,320, 32, 32),
	Vector4(64,320, 32, 32),
	Vector4(96,320, 32, 32),
	Vector4(96,320, 32, 32),
	Vector4(96,320, 32, 32)
};

Vector4 deathFrames[] = {
	Vector4(96,352, 32, 32),
	Vector4(128,352, 32, 32),
	Vector4(96,352, 32, 32),	
	Vector4(128,352, 32, 32),
	Vector4(96,352, 32, 32),
	Vector4(128,352, 32, 32),
};

Vector4 ladderFrames[] = {
	Vector4(64,224, 32, 32),
	Vector4(64,224, 32, 32),
	Vector4(64,224, 32, 32),
	Vector4(64,224, 32, 32),
	Vector4(64,224, 32, 32),
	Vector4(64,224, 32, 32),
};


PlayerCharacter::PlayerCharacter() : SimObject() {
	currentAnimState	= PlayerState::Left;
	texture				= texManager->GetTexture("FruitWizard\\mini_fantasy_sprites_oga_ver.png");
	animFrameCount		= 6;
	inverseMass			= 5.0f; 
	collider = new CollisionVolume(this, CollisionVolume::Shapes::CIRCLE, CollisionVolume::Types::PLAYER, 13.0f); 
	collider->setPosition(this->position);
}

PlayerCharacter::~PlayerCharacter() {

}

bool PlayerCharacter::UpdateObject(float dt) {
	collider->setPosition(this->GetPosition());
	float testSpeed = 64;
	if (!onLadder) {
		// Gravity 
		AddForce(Vector2(0.0f, -20.0f));
	}

	Vector4* animSource = idleFrames;

	if (currentAnimState == PlayerState::Attack) {
		animSource = attackFrames;
		if (currentanimFrame >= 5) {
			currentAnimState = PlayerState::Idle;
		}
	}
	else {
		currentAnimState = PlayerState::Idle;
		if (Window::GetKeyboard()->KeyDown(KeyboardKeys::LEFT)) {
			animSource = runFrames;
			currentAnimState = PlayerState::Left;
			this->AddForce(Vector2(-70, 0));
			flipAnimFrame = true;
		}
		if (Window::GetKeyboard()->KeyDown(KeyboardKeys::RIGHT)) {
			animSource = runFrames;
			currentAnimState = PlayerState::Right;
			this->AddForce(Vector2(70, 0));
			flipAnimFrame = false;
		}

		if (onLadder) {
			if (Window::GetKeyboard()->KeyDown(KeyboardKeys::UP)) {
				animSource = runFrames;
				currentAnimState = PlayerState::Right;
				this->AddForce(Vector2(0, 35));
				flipAnimFrame = false;
			}
			if (Window::GetKeyboard()->KeyDown(KeyboardKeys::DOWN)) {
				animSource = runFrames;
				currentAnimState = PlayerState::Right;
				this->AddForce(Vector2(0, -35));
				flipAnimFrame = false;
			}
		}
		
		if (Window::GetKeyboard()->KeyDown(KeyboardKeys::CONTROL)) {
			if (game->dustCount >= 4) {
				game->pixieMode();
			}
		}
		if (Window::GetKeyboard()->KeyPressed(KeyboardKeys::SPACE)) {
			if (game->magicCount > 0) {
				currentAnimState = PlayerState::Attack;
				currentanimFrame = 0;
				Spell* testSpell;
				float dirY = (rand() % 3) - 1;

				if (flipAnimFrame) {
					testSpell = new Spell(Vector2(-1, dirY));
				}
				else {
					testSpell = new Spell(Vector2(1, dirY));
				}
				testSpell->SetPosition(this->position);
				this->game->AddNewObject(testSpell);
				game->magicCount--;
			}
		}

		if (playerHit) {
			
		}
		
	}

	animFrameData = animSource[currentanimFrame];

	onLadder = false;
	return true;
}