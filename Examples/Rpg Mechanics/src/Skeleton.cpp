#include "Skeleton.h"

void Skeleton::OnStart(float x, float y) {
	skeleton.OnStart(
		16, 0, 16, 16,
		x, y, 64.0f, 64.0f,
		"assets/skeleton.png",
		SDL_FLIP_NONE, 0.0);

	ChangeAnimID();
}

void Skeleton::OnInput(const Uint8* keystates) {
	//Input to Move Skeleton Up-Down
	if (keystates[SDL_SCANCODE_W]) {
		direction.y = -1.0f;
		animID = AnimationID::IDLE_UP;
	} else if (keystates[SDL_SCANCODE_S]) {
		direction.y = 1.0f;
		animID = AnimationID::IDLE_DOWN;
	} else {
		direction.y = 0.0f;
	}

	//Input Move Skeleton Right-Left
	if (keystates[SDL_SCANCODE_D]) {
		direction.x = 1.0f;
		animID = AnimationID::IDLE_RIGHT;
	} else if (keystates[SDL_SCANCODE_A]) {
		direction.x = -1.0f;
		animID = AnimationID::IDLE_LEFT;
	} else {
		direction.x = 0.0f;
	}



}

void Skeleton::OnUpdate(float deltaTime) {
	//Normalize Direction
	float magnitude = sqrtf(direction.x * direction.x + direction.y * direction.y);
	if (magnitude > 1.0f) {
		direction.x /= magnitude;
		direction.y /= magnitude;
	}

	//Move 
	skeleton.SetDstX(skeleton.GetDstX() + direction.x * speed * deltaTime);
	skeleton.SetDstY(skeleton.GetDstY() + direction.y * speed * deltaTime);

	//Change Animation
	ChangeAnimID();
}

void Skeleton::Draw() {
	skeleton.Draw();
}

void Skeleton::OnFinish() {
	skeleton.OnFinish();
}

void Skeleton::ChangeAnimID() {
	int n = 0;

	switch (animID) {
	case AnimationID::IDLE_DOWN:
		n = 0;
		break;
	case AnimationID::IDLE_UP:
		n = 3;
		break;
	case AnimationID::IDLE_RIGHT:
		n = 2;
		break;
	case AnimationID::IDLE_LEFT:
		n = 1;
		break;
	}

	skeleton.SetSrcY(skeleton.GetSrcH() * n);
}