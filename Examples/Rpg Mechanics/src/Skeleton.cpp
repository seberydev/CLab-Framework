#include "Skeleton.h"

void Skeleton::OnStart(float x, float y) {
	skeleton.OnStart(
		0, 0, 16, 16,
		x, y, 64.0f, 64.0f,
		"assets/skeleton.png",
		SDL_FLIP_NONE, 0.0);

	ChangeAnimID();
	maxX = clf::Engine::ScreenWidthF() - skeleton.GetDstW() / 2.0f;
	minX = skeleton.GetDstW() / 2.0f;
	maxY = clf::Engine::ScreenHeightF() - 50.0f - skeleton.GetDstH();
	minY = skeleton.GetDstH() / 2.0f;
}

const SDL_FRect& Skeleton::GetPos() const { return skeleton.GetDst(); }

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

	SetOnBound();

	//Change Animation
	if (direction.x != 0.0f || direction.y != 0.0f) {
		currentTime += deltaTime;

		if (currentTime >= animDelay) {
			currentTime = 0.0f;
			MoveAnim(deltaTime);
		}
	} else {
		currentTime = 0.0f;
		skeleton.SetSrcX(16);
	}


	ChangeAnimID();
}

void Skeleton::Draw() {
	skeleton.Draw();
}

void Skeleton::OnFinish() {
	skeleton.OnFinish();
}

void Skeleton::ChangeAnimID() {
	int y = 0;
	int x = 1;

	switch (animID) {
	case AnimationID::IDLE_DOWN:
		y = 0;
		break;
	case AnimationID::IDLE_UP:
		y = 3;
		break;
	case AnimationID::IDLE_RIGHT:
		y = 2;
		break;
	case AnimationID::IDLE_LEFT:
		y = 1;
		break;
	}

	skeleton.SetSrcY(skeleton.GetSrcH() * y);
}

void Skeleton::MoveAnim(float deltaTime) {
	int nextX = animIndex * skeleton.GetSrcW();
	
	if (nextX > skeleton.GetSrcW() * (animTotalFrames - 1)) {
		nextX = 0;
		animIndex = 1;
	} else {
		++animIndex;
	}

	skeleton.SetSrcX(nextX);
}

void Skeleton::SetOnBound() {
	float x = skeleton.GetDstX();
	float y = skeleton.GetDstY();

	if (x >= maxX)
		skeleton.SetDstX(maxX);

	if (x <= minX)
		skeleton.SetDstX(minX);

	if (y >= maxY)
		skeleton.SetDstY(maxY);

	if (y <= minY)
		skeleton.SetDstY(minY);
}