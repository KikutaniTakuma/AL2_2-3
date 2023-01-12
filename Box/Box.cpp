#include "Box.h"
#include "Game/Mouse/Mouse.h"
#include "Game/Texture/Texture.h"
#include "Game/MyMath/MyMath.h"

Box::Box(Camera* camera, Vector2D pos):
	Object(camera),
	blend(BlendMode::kBlendModeNormal),
	flg(false),
	alpha(128)
{
	this->pos.Set(pos, { 600.0f, 600.0f });
}

Box::Box(Camera* camera, Vector2D pos, bool flg) :
	Object(camera),
	blend(BlendMode::kBlendModeNormal),
	flg(flg),
	alpha(128)
{
	this->pos.Set(pos, { 600.0f, 600.0f });
}

void Box::Update() {
	if (flg) {
		pos.worldPos = { static_cast<float>(Mouse::x),-static_cast<float>(Mouse::y) + 720.0f };
	}

	pos.worldMatrix.Translate(pos.worldPos);
}

void Box::Draw(class Texture& tex) {
	Novice::SetBlendMode(blend);
	camera->DrawQuad(pos, tex, 0, false, MyMath::GetRGB(255,255,255,alpha));
	Novice::SetBlendMode(BlendMode::kBlendModeNormal);
}

void Box::Reset() {

}