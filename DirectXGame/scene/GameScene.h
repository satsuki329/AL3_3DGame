#pragma once

#include "Audio.h"
#include "DirectXCommon.h"
#include "Input.h"
#include "Model.h"
#include "SafeDelete.h"
#include "Sprite.h"
#include "ViewProjection.h"
#include "WorldTransform.h"

/// <summary>
/// ゲームシーン
/// </summary>
class GameScene {

public: // メンバ関数
	/// <summary>
	/// コンストクラタ
	/// </summary>
	GameScene();

	/// <summary>
	/// デストラクタ
	/// </summary>
	~GameScene();

	/// <summary>
	/// 初期化
	/// </summary>
	void Initialize();

	/// <summary>
	/// 毎フレーム処理
	/// </summary>
	void Update();

	/// <summary>
	/// 描画
	/// </summary>
	void Draw();

	void PlayerUpdate();

	void BeamUpdate();

	void BeamMoob();

	void BeamBorn();

	void EnemyUpdate();

	void EnemyMove();

	void EnemyBorn();


private: // メンバ変数
	DirectXCommon* dxCommon_ = nullptr;
	Input* input_ = nullptr;
	Audio* audio_ = nullptr;

	uint32_t texturehandleBG = 0;
	Sprite* spriteBG = nullptr;

	ViewProjection viewprojection;

	uint32_t texturehandlestage = 0;
	Model* modelstage = nullptr;
	WorldTransform worldtransformstage;

	uint32_t texturehandleplayer = 0;
	Model* modelplayer = nullptr;
	WorldTransform worldtransformplayer;

	uint32_t texturehandlebeam = 0;
	Model* modelbeam = nullptr;
	WorldTransform worldtransformbeam;
	int beamflag = 0;

	uint32_t texturehandleenemy = 0;
	Model* modelenemy = nullptr;
	WorldTransform worldtransformenemy;
	int enemyflag = 0;

	/// <summary>
	/// ゲームシーン用
	/// </summary>
};
