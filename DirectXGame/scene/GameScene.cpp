#include "GameScene.h"
#include "TextureManager.h"
#include <cassert>
#include "MathUtilityForText.h"

GameScene::GameScene() { 

}

GameScene::~GameScene() { 
	delete spriteBG;
	delete modelstage;
	delete modelplayer;
	delete modelbeam;
}

void GameScene::Initialize() {

	dxCommon_ = DirectXCommon::GetInstance();
	input_ = Input::GetInstance();
	audio_ = Audio::GetInstance();

	texturehandleBG = TextureManager::Load("bg.jpg");
	spriteBG = Sprite::Create(texturehandleBG, {0, 0});

	viewprojection.Initialize();

	texturehandlestage = TextureManager::Load("stage.jpg");
	modelstage = Model::Create();
	worldtransformstage.Initialize();

	viewprojection.translation_.y = 1;
	viewprojection.translation_.z = -6;
	viewprojection.Initialize();

	worldtransformstage.translation_ = {0, -1.5f, 0};
	worldtransformstage.scale_ = {4.5f, 1, 40};

	worldtransformstage.matWorld_ = MakeAffineMatrix(
	    worldtransformstage.scale_,
		worldtransformstage.rotation_,
	    worldtransformstage.translation_);

	worldtransformstage.TransferMatrix();

	texturehandleplayer = TextureManager::Load("player.png");
	modelplayer = Model::Create();
	worldtransformplayer.scale_ = {0.5f, 0.5f, 0.5f};
	worldtransformplayer.Initialize();

	texturehandlebeam = TextureManager::Load("beam.png");
	modelbeam = Model::Create();
	worldtransformbeam.scale_ = {0.3f, 0.3f, 0.3f};
	worldtransformbeam.Initialize();
}

void GameScene::Update() { 
	PlayerUpdate();
	BeamUpdate();
}

void GameScene::Draw() {

	// コマンドリストの取得
	ID3D12GraphicsCommandList* commandList = dxCommon_->GetCommandList();

#pragma region 背景スプライト描画
	// 背景スプライト描画前処理
	Sprite::PreDraw(commandList);

	/// <summary>
	/// ここに背景スプライトの描画処理を追加できる
	/// </summary>
	spriteBG->Draw();


	// スプライト描画後処理
	Sprite::PostDraw();
	// 深度バッファクリア
	dxCommon_->ClearDepthBuffer();
#pragma endregion

#pragma region 3Dオブジェクト描画
	// 3Dオブジェクト描画前処理
	Model::PreDraw(commandList);

	/// <summary>
	/// ここに3Dオブジェクトの描画処理を追加できる
	/// </summary>
	modelstage->Draw(worldtransformstage, viewprojection, texturehandlestage);

	modelplayer->Draw(worldtransformplayer, viewprojection, texturehandleplayer);

	if (beamflag == 1)
	{
		modelbeam->Draw(worldtransformbeam, viewprojection, texturehandlebeam);
	}

	// 3Dオブジェクト描画後処理
	Model::PostDraw();
#pragma endregion

#pragma region 前景スプライト描画
	// 前景スプライト描画前処理
	Sprite::PreDraw(commandList);

	/// <summary>
	/// ここに前景スプライトの描画処理を追加できる
	/// </summary>

	// スプライト描画後処理
	Sprite::PostDraw();

#pragma endregion
}

void GameScene::PlayerUpdate() { 
	if (input_->PushKey(DIK_RIGHT))
	{
		worldtransformplayer.translation_.x += 0.1f;
		if (worldtransformplayer.translation_.x > 4.0f)
		{
			worldtransformplayer.translation_.x = 4.0f;
		}
	}

	if (input_->PushKey(DIK_LEFT)) {
		worldtransformplayer.translation_.x -= 0.1f;
		if (worldtransformplayer.translation_.x < -4.0f) {
			worldtransformplayer.translation_.x = -4.0f;
		}
	}

	worldtransformplayer.matWorld_ = MakeAffineMatrix(
	    worldtransformplayer.scale_, 
		worldtransformplayer.rotation_,
	    worldtransformplayer.translation_);

	worldtransformplayer.TransferMatrix();
}

void GameScene::BeamUpdate() {
	BeamMoob();
	BeamBorn();

	worldtransformbeam.matWorld_ = MakeAffineMatrix(
	    worldtransformbeam.scale_, 
		worldtransformbeam.rotation_, 
		worldtransformbeam.translation_);

	worldtransformbeam.TransferMatrix();
}

void GameScene::BeamMoob() { 
	if (beamflag == 1)
	{
		worldtransformbeam.translation_.z += 0.1f; 
		worldtransformbeam.rotation_.x += 0.1f;
		if (worldtransformbeam.translation_.z > 40)
		{
			beamflag = 0;
		}
	}
}

void GameScene::BeamBorn() { 
	if (input_->PushKey(DIK_SPACE) && beamflag == 0)
	{
		worldtransformbeam.translation_.x = worldtransformplayer.translation_.x;
		worldtransformbeam.translation_.y = worldtransformplayer.translation_.y;
		worldtransformbeam.translation_.z = worldtransformplayer.translation_.z;

		beamflag = 1;
	}
}