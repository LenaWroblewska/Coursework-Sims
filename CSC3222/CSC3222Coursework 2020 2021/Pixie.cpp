#include "Pixie.h"
#include "TextureManager.h"
#include "GameSimsRenderer.h"
#include "CollisionVolume.h"
#include "FruitWizardGame.h"

using namespace NCL;
using namespace CSC3222;

Vector4 pixieFrames[] = {
	Vector4(32 * 1,32 * 4, 32, 32),
	Vector4(32 * 2,32 * 4, 32, 32),
	Vector4(32 * 3,32 * 4, 32, 32),
	Vector4(32 * 4,32 * 4, 32, 32),
};

Pixie::Pixie() : SimObject() {
	texture			= texManager->GetTexture("FruitWizard\\super_random_sprites.png");
	animFrameCount	= 4;
	collider = new CollisionVolume(this, CollisionVolume::Shapes::CIRCLE, CollisionVolume::Types::PIXIE, 9.0f);
	collider->setPosition(this->position);
}

Pixie::~Pixie() {
}

bool Pixie::UpdateObject(float dt) {
	collider->setPosition(this->GetPosition());
	animFrameData = pixieFrames[currentanimFrame];
	this->Flocking(dt, game->pixies, game->guards);
	updateSprings();

	if (collected) {
		game->pixies.erase(std::find(game->pixies.begin(), game->pixies.end(),this));
		game->currentScore += 1000;
		game->magicCount += 1;
		return false; 
	}

	return true;
}

Vector2 Pixie::Alignment(std::vector<SimObject*> allBoids) {
	Vector2  dir = this->velocity.Normalised(); 
	for (SimObject* s : allBoids) {
	if (s == this || s == nullptr) {
		continue;
	}
	float  distance = (this->position - s->GetPosition()).Length();
	if (distance > alignmentThreshold) { 
		continue;
	}
	dir += s->GetVelocity().Normalised();
	}
	return  dir.Normalised(); 
} 

Vector2 Pixie::Separation(std::vector<SimObject*> allBoids) {
	Vector2  dir;
	for (SimObject* s : allBoids) {
		if (s == this || s == nullptr) {
			continue;
		}
		float  distance = (this->position - s->GetPosition()).Length();
		if (distance > separationThreshold) { 
			continue;
		}
		float  strength = 1.0f - (distance / separationThreshold);
		dir += (this->position - s->GetPosition()).Normalised() * strength;
	}
	return  dir.Normalised();
}

Vector2 Pixie::Avoidance(std::vector<SimObject*> allEnemies) {
	Vector2  dir;
	for (SimObject* e : allEnemies) {
		float  distance = (this->position - e->GetPosition()).Length();
		if (distance > avoidanceThreshold) { 
			continue;
		}
		dir += (this->position - e->GetPosition()).Normalised();
	}
	return dir.Normalised();
}

void Pixie::Flocking(float dt, std::vector<SimObject*> allBoids, std::vector<SimObject*> allEnemies) {
	for (SimObject* s : allBoids) {
		Vector2  dir;
		dir += Alignment(allBoids);
		dir += Separation(allBoids);
		//dir += Cohesion(allBoids);
		dir += Avoidance(allEnemies);
		this->position += dir * dt; // directly  applied  as ’velocity ’
	 }
}