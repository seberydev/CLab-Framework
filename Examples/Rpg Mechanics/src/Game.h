#pragma once

#include "clf.h"
#include "Skeleton.h"

class Game : public clf::Engine {
protected:
	void OnStart() override;
	void OnInput(const Uint8* keystates) override;
	void OnUpdate(float deltaTime) override;
	void OnRender() override;
	void OnFinish() override;
private:
	clf::Utilities::Sprite bg;
	Skeleton skeleton;
	float bottomSize = 50.0f;
};