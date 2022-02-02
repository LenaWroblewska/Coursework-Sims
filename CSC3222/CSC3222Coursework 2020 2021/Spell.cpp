#include "Spell.h"
#include "TextureManager.h"
#include "GameSimsRenderer.h"
#include "CollisionVolume.h"

#include "../../Common/Maths.h"

#include "../../Common/Vector3.h"
#include "../../Common/Vector4.h"

using namespace NCL;
using namespace CSC3222;

Vector4 activeFrames[] = {
	Vector4(512,384, 32, 32),
	Vector4(512,384, 32, 32),
	Vector4(512,384, 32, 32),
	Vector4(384,416, 32, 32),
	Vector4(384,416, 32, 32),
	Vector4(384,416, 32, 32),
};

Vector4 explodeFrames[] = {
	Vector4(512,384, 32, 32),
	Vector4(384,416, 32, 32),
	Vector4(416,416, 32, 32),
	Vector4(448,416, 32, 32),
	Vector4(480,416, 32, 32),
	Vector4(512,416, 32, 32),	
};

Spell::Spell(Vector2 direction) : SimObject()	{
	texture		= texManager->GetTexture("FruitWizard\\mini_fantasy_sprites_oga_ver.png");
	velocity	= direction;
	animFrameCount = 6;
	inverseMass = 25.0f;
	elasticity = 1.0f;

	collider = new CollisionVolume(this, CollisionVolume::Shapes::CIRCLE, CollisionVolume::Types::SPELL, 8.0f);
	collider->setPosition(this->position);
	
	damping = 1.0f;
	AddImpulse(direction * 10);
}

Spell::~Spell()	{
}

void Spell::DrawObject(GameSimsRenderer &r) {
	Vector4	texData = explodeFrames[currentanimFrame];
	Vector2 texPos	= Vector2(texData.x, texData.y);
	Vector2 texSize = Vector2(texData.z, texData.w);
	r.DrawTextureArea((OGLTexture*)texture, texPos, texSize, position);
}

bool Spell::UpdateObject(float dt) {
	collider->setPosition(this->GetPosition());
	animFrameData = explodeFrames[currentanimFrame];

	if (hitEnemy) {
		return false;
	}

	if (velocity == Vector2(0,0)) {
		return false;
	}

	if (bounces == 3) {
		return false; 
	}
	return true;
}