#pragma once

#include "Audio.h"
#include "DebugText.h"
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

	void Collision();

	void CollisionPlayerEnemy();

	void CollisionBeamEnemy();

	void GamePlayUpdate();

	void GamePlayDraw3D();

	void GamePlayDraw2DBack();

	void GamePlayDraw2DNear();

	void TitleUpdate();

	void TitleDraw2DNear();

	void GameOverUpdate();

	void GameOverDraw2DNear();

	void GamePlayStart();


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
	WorldTransform worldtransformbeam[10];
	int beamflag[10] = {};

	uint32_t texturehandleenemy = 0;
	Model* modelenemy = nullptr;
	WorldTransform worldtransformenemy[10];
	int enemyflag[10] = {};
	float enemyspeed[10] = {};

	DebugText* debugtext = nullptr;
	int gamescore = 0;
	int playerlife = 1;

	int sceneMode = 1;

	uint32_t texturehandleTitle = 0;
	Sprite* spriteTitle = nullptr;

	uint32_t enter = 0;
	Sprite* spriteenter = nullptr;

	int gametimer = 0;

	uint32_t texturehandlegameover = 0;
	Sprite* spritegameover = nullptr;

	int beamtimer = 1; 

	uint32_t titlebgm = 0;
	uint32_t gamebgm = 0;
	uint32_t gameoverbgm = 0;
	uint32_t enemyhitse = 0;
	uint32_t playerhitse = 0;
	uint32_t voicehandlebgm = 0;

	/// <summary>
	/// ゲームシーン用
	/// </summary>
};
