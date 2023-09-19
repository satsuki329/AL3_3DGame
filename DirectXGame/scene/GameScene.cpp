#include "GameScene.h"
#include "TextureManager.h"
#include <cassert>
#include "MathUtilityForText.h"

GameScene::GameScene() { 

}

GameScene::~GameScene() { 
	delete spriteBG;
	delete modelstage;
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
}

void GameScene::Update() {}

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
