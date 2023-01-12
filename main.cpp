#include <Novice.h>
#include "Game/Texture/Texture.h"
#include "Box/Box.h"
#include "Game/KeyInput/KeyInput.h"
#include "Game/Mouse/Mouse.h"
#include <vector>

const char kWindowTitle[] = "LC1A_08_キクタニタクマ_タイトル";

// Windowsアプリでのエントリーポイント(main関数)
int WINAPI WinMain(HINSTANCE, HINSTANCE, LPSTR, int) {

	// ライブラリの初期化
	Novice::Initialize(kWindowTitle, 1280, 720);

	int number = 1;

	Camera* camera = new Camera;

	Texture tex("./Resources/particle.png", 600, 600, 600);

	std::vector<Box*> obj(0);

	for (int i = 0; i < 3; i++) {
		obj.emplace_back(new Box(camera, { 390.0f + 250.0f*i, 360.0f }));
	}
	obj.emplace_back(new Box(camera, { 0.0f,0.0f }, true));

	BlendMode blend = BlendMode::kBlendModeNone;

	// ウィンドウの×ボタンが押されるまでループ
	while (Novice::ProcessMessage() == 0) {
		// フレームの開始
		Novice::BeginFrame();

		KeyInput::Input();
		Mouse::Input();

		///
		/// ↓更新処理ここから
		///
		if (KeyInput::Pushed(DIK_1)) {
			for (auto& i : obj) {
				i->blend = BlendMode::kBlendModeNormal;
			}
			number = 1;
		}
		if (KeyInput::Pushed(DIK_2)) {
			for (auto& i : obj) {
				i->blend = BlendMode::kBlendModeAdd;
			}
			number = 2;
		}
		if (KeyInput::Pushed(DIK_3)) {
			for (auto& i : obj) {
				i->blend = BlendMode::kBlendModeSubtract;
			}
			number = 3;
		}
		if (KeyInput::Pushed(DIK_4)) {
			for (auto& i : obj) {
				i->blend = BlendMode::kBlendModeNone;
			}
			number = 4;
		}

		if (KeyInput::getKeys(DIK_UP)) {
			for (auto& i : obj) {
				i->alpha++;
				if (i->alpha > 255) {
					i->alpha = 255;
				}
			}
		}
		if (KeyInput::getKeys(DIK_DOWN)) {
			for (auto& i : obj) {
				i->alpha--;
				if (i->alpha < 0) {
					i->alpha = 0;
				}
			}
		}

		
		for (auto& i : obj) {
			i->Update();
		}
		///
		/// ↑更新処理ここまで
		///

		///
		/// ↓描画処理ここから
		///
		camera->Update();

		Novice::DrawBox(0, 0, 1280, 720, 0.0f, 0xff, kFillModeSolid);

		Novice::DrawLine(0, 360, 1280, 360,WHITE);
		Novice::DrawLine(640, 0, 640, 720, WHITE);

		for (auto& i : obj) {
			i->Draw(tex);
		}

		Novice::ScreenPrintf(0, 0, "Change Blend Mode : 1 alpha Blend(Normal), 2 Add Blend, 3 Subtract Blend, 4 None    Current Blend Mode is %d", number);
		Novice::ScreenPrintf(0, 20, "Change Alpha Value : Up/Down : %d", obj[0]->alpha);
		
		///
		/// ↑描画処理ここまで
		///

		// フレームの終了
		Novice::EndFrame();

		// ESCキーが押されたらループを抜ける
		if (KeyInput::Released(DIK_SPACE)) {
			break;
		}
	}

	// ライブラリの終了
	Novice::Finalize();
	return 0;
}
