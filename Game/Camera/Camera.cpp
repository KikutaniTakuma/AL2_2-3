#include "Game/Camera/Camera.h"
#include "Game/Frame/Frame.h"
#include "Game/Texture/Texture.h"
#include "Game/Quad/Quad.h"
#include "Game/MyMath/MyMath.h"
#include "Game/KeyInput/KeyInput.h"

#include <Novice.h>

#include <time.h>
#include <stdlib.h>
#include <limits.h>

Camera::Camera() {
	worldPos = {1280.0f / 2.0f, 720.0f/ 2.0f };
	screenPos = { 0.0f,0.0f };

	drawLeftTop = { 1280.0f / -2.0f, 720.0f / 2.0f };
	drawRightBottom = { 1280.0f / 2.0f, 720.0f / -2.0f };

	size = { 1280.0f, 720.0f };
	scale = 1.0f;

	viewMatrix.Translate(worldPos);
	viewMatrix.Inverse();

	NorDevMatrix.Orthographic(drawLeftTop, drawRightBottom);

	viewPortMatrix.Viewport(screenPos, size);

	frame = new Frame;
	frame->Restart();

	shakeScale = { 10.0f, 10.0f };
}

Camera::~Camera(){
	delete frame;
}

void Camera::Update() {
	viewMatrix.Translate(this->worldPos);
	viewMatrix.Inverse();

	NorDevMatrix.Orthographic(drawLeftTop / scale, drawRightBottom / scale);
	viewPortMatrix.Viewport(screenPos, size);

	vpvpMatrix = viewMatrix * NorDevMatrix * viewPortMatrix;

	frame->Start();
	if (frame->frame > ULLONG_MAX) {
		frame->Stop();
		frame->Restart();
	}
}

void Camera::Update(const Vector2D& worldPos, const Vector2D& cameraPos, const float& scale, const bool& shake) {
	frame->Start();
	if (frame->frame > ULLONG_MAX) {
		frame->Stop();
		frame->Restart();
	}

	this->scale = scale;

	this->worldPos = worldPos;

	// もしシェイクフラグがオンなら
	if (shake) {
		Shake();
	}

	viewMatrix.Translate(this->worldPos);
	viewMatrix.Inverse();
	NorDevMatrix.Orthographic(size / this->scale);
	viewPortMatrix.Viewport(cameraPos, size);

	vpvpMatrix = viewMatrix * NorDevMatrix * viewPortMatrix;
}

void Camera::Update(const Vector2D& worldPos, const Vector2D& cameraPos, const Vector2D& drawLeftTop, const Vector2D& drawRightBottom, const bool& shake) {
	frame->Start();
	if (frame->frame > ULLONG_MAX) {
		frame->Stop();
		frame->Restart();
	}

	this->worldPos = worldPos;

	// もしシェイクフラグがオンなら
	if (shake) {
		Shake();
	}
	this->drawLeftTop = drawLeftTop;
	this->drawRightBottom = drawRightBottom;

	viewMatrix.Translate(this->worldPos);
	viewMatrix.Inverse();
	NorDevMatrix.Orthographic(this->drawLeftTop, this->drawRightBottom);
	viewPortMatrix.Viewport(cameraPos, size);

	vpvpMatrix = viewMatrix * NorDevMatrix * viewPortMatrix;
}


void Camera::Shake() {
	worldPos.x += static_cast<float>((rand() % static_cast<int>(shakeScale.x))) - shakeScale.x / 2.0f;
	worldPos.y += static_cast<float>((rand() % static_cast<int>(shakeScale.y))) - shakeScale.y / 2.0f;
}

void Camera::DrawQuad(Quad quad, Texture& texture, const int& animationSpd, const bool& animationStop, const unsigned int& color) const {
	if (isDraw(quad.worldPos)) {
		quad.worldMatrix *= vpvpMatrix;

		if (!animationStop && animationSpd != 0) {
			if (frame->frame % animationSpd == 0) {
				texture.drawPos += texture.width;
				if (texture.drawPos > texture.spriteSize - texture.width) {
					texture.drawPos = 0;
				}
			}
		}

		Novice::DrawQuad(
			static_cast<int>(quad.getMatrixLeftTop().x), static_cast<int>(quad.getMatrixLeftTop().y),
			static_cast<int>(quad.getMatrixRightTop().x), static_cast<int>(quad.getMatrixRightTop().y),
			static_cast<int>(quad.getMatrixLeftUnder().x), static_cast<int>(quad.getMatrixLeftUnder().y),
			static_cast<int>(quad.getMatrixRightUnder().x), static_cast<int>(quad.getMatrixRightUnder().y),
			texture.drawPos, 0, texture.width, texture.height, texture.textureHandle, color
		);
	}
}

void Camera::DrawQuad(Quad quad, Texture& texture, float deg, const int& animationSpd, const bool& animationStop, const unsigned int& color) const {
	if (isDraw(quad.worldPos)) {
		quad.worldMatrix *= vpvpMatrix;

		if (!animationStop && animationSpd != 0) {
			if (frame->frame % animationSpd == 0) {
				texture.drawPos += texture.width;
				if (texture.drawPos > texture.spriteSize - texture.width) {
					texture.drawPos = 0;
				}
			}
		}

		Novice::DrawQuad(
			static_cast<int>(quad.getMatrixLeftTop().x), static_cast<int>(quad.getMatrixLeftTop().y),
			static_cast<int>(quad.getMatrixRightTop().x), static_cast<int>(quad.getMatrixRightTop().y),
			static_cast<int>(quad.getMatrixLeftUnder().x), static_cast<int>(quad.getMatrixLeftUnder().y),
			static_cast<int>(quad.getMatrixRightUnder().x), static_cast<int>(quad.getMatrixRightUnder().y),
			texture.drawPos, 0, texture.width, texture.height, texture.textureHandle, color
		);
	}
}


void Camera::DrawUI(Quad quad, Texture& texture, const int& animationSpd, const bool& animationStop, const unsigned int& color) const {
	Matrix3x3 mat;
	mat.Translate(worldPos);
	quad.worldMatrix *= mat * vpvpMatrix;

	if (!animationStop && animationSpd != 0) {
		if (frame->frame % animationSpd == 0) {
			texture.drawPos += texture.width;
			if (texture.drawPos > texture.spriteSize - texture.width) {
				texture.drawPos = 0;
			}
		}
	}

	Novice::DrawQuad(
		static_cast<int>(quad.getMatrixLeftTop().x), static_cast<int>(quad.getMatrixLeftTop().y),
		static_cast<int>(quad.getMatrixRightTop().x), static_cast<int>(quad.getMatrixRightTop().y),
		static_cast<int>(quad.getMatrixLeftUnder().x), static_cast<int>(quad.getMatrixLeftUnder().y),
		static_cast<int>(quad.getMatrixRightUnder().x), static_cast<int>(quad.getMatrixRightUnder().y),
		texture.drawPos, 0, texture.width, texture.height, texture.textureHandle, color
	);
}


bool Camera::isDraw(Vector2D pos, const float& drawLength) const {
	pos -= worldPos;
	if (pos.x < drawLeftTop.x - drawLength || pos.x > drawRightBottom.x + drawLength ||
		pos.y > drawLeftTop.y + drawLength || pos.y < drawRightBottom.y - drawLength) {
		return false;
	}
	else {
		return true;
	}
}

Vector2D Camera::getPos() const {
	return worldPos;
}

Vector2D Camera::getDrawLeftTop() const {
	return drawLeftTop;
}
Vector2D Camera::getDrawRightBottom() const {
	return drawRightBottom;
}
Vector2D Camera::getDrawSize() const {
	return size;
}