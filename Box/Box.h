#pragma once

#include "Game/Object/Object.h"
#include <Novice.h>

class Box : public Object {
public:
	Box(Camera* camera, Vector2D pos);
	Box(Camera* camera, Vector2D pos, bool flg);

public:
	int alpha;
	BlendMode blend;

private:
	bool flg;

public:
	void Update() override;
	void Draw(class Texture& tex) override;
	void Reset() override;
};