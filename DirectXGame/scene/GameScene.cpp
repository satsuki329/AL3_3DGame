#include "GameScene.h"
#include "MathUtilityForText.h"
#include "TextureManager.h"
#include "time.h"
#include <cassert>

GameScene::GameScene() {}

GameScene::~GameScene() {
	delete spriteBG;
	delete modelstage;
	delete modelplayer;
	delete modelbeam;
	delete modelenemy;
	delete spriteTitle;
	delete spriterule;
	delete spriteenter;
	delete spritegameover;
	for (int i = 0; i < 5; i++) {
		delete spritenumber[i];
	}
	delete spritescore;
	delete modelturanuki;
	delete modelult;
	delete modelspturanuki;
	delete modelv;
	delete modelb;
}

void GameScene::Initialize() {

	dxCommon_ = DirectXCommon::GetInstance();
	input_ = Input::GetInstance();
	audio_ = Audio::GetInstance();

	texturehandleBG = TextureManager::Load("bg.png");
	spriteBG = Sprite::Create(texturehandleBG, {0, 0});

	viewprojection.Initialize();

	texturehandlestage = TextureManager::Load("stage2.jpg");
	modelstage = Model::Create();

	for (int i = 0; i < 20; i++) {
		worldtransformstage[i].Initialize();
	}

	viewprojection.translation_.y = 1;
	viewprojection.translation_.z = -6;
	viewprojection.Initialize();

	for (int i = 0; i < 20; i++) {
		worldtransformstage[i].translation_ = {0, -1.5f, 2.0f * i - 5};
		worldtransformstage[i].scale_ = {4.5f, 1, 1};

		worldtransformstage[i].matWorld_ = MakeAffineMatrix(
		    worldtransformstage[i].scale_, worldtransformstage[i].rotation_,
		    worldtransformstage[i].translation_);

		worldtransformstage[i].TransferMatrix();
	}

	texturehandleplayer = TextureManager::Load("player.png");
	modelplayer = Model::Create();
	worldtransformplayer.scale_ = {0.5f, 0.5f, 0.5f};
	worldtransformplayer.Initialize();

	texturehandlebeam = TextureManager::Load("plbullet.png");
	modelbeam = Model::Create();
	for (int i = 0; i < 10; i++) {
		worldtransformbeam[i].scale_ = {0.1f, 0.1f, 0.1f};
		worldtransformbeam[i].Initialize();
	}

	texturehandleturanuki = TextureManager::Load("turanuki.png");
	modelturanuki = Model::Create();
	v = TextureManager::Load("v.png");
	modelv = Sprite::Create(v, {1000.0f, 0.0f});
	worldtransformturanuki.scale_ = {0.2f, 0.2f, 0.2f};
	worldtransformturanuki.Initialize();

	texturehandlespturanuki = TextureManager::Load("spturanuki.png");
	modelspturanuki = Model::Create();
	b = TextureManager::Load("b.png");
	modelb = Sprite::Create(b, {1100.0f, 0.0f});
	for (int i = 0; i < 5; i++) {
		worldtransformspturanuki[i].scale_ = {0.3f, 0.3f, 0.3f};
		worldtransformspturanuki[i].Initialize();
	}
	worldtransformspturanuki->Initialize();

	texturehandleult = TextureManager::Load("chest2.png");
	modelult = Model::Create();
	worldtransformult.Initialize();

	texturehandleenemy = TextureManager::Load("enemy.png");
	modelenemy = Model::Create();
	for (int i = 0; i < 10; i++) {
		worldtransformenemy[i].scale_ = {0.3f, 0.3f, 0.3f};
		worldtransformenemy[i].Initialize();
	}

	srand((unsigned int)time(NULL));

	debugtext = DebugText::GetInstance();
	debugtext->Initialize();

	texturehandleTitle = TextureManager::Load("title2.png");
	spriteTitle = Sprite::Create(texturehandleTitle, {0, 0});

	texturehandlerule = TextureManager::Load("rule.png");
	spriterule = Sprite::Create(texturehandlerule, {0, 0});

	enter = TextureManager::Load("enter.png");
	spriteenter = Sprite::Create(enter, {420, 450});

	texturehandlegameover = TextureManager::Load("gameover.png");
	spritegameover = Sprite::Create(texturehandlegameover, {0, 0});

	titlebgm = audio_->LoadWave("Ring05.wav");
	gamebgm = audio_->LoadWave("Ring08.wav");
	gameoverbgm = audio_->LoadWave("Ring09.wav");
	enemyhitse = audio_->LoadWave("chord.wav");
	playerhitse = audio_->LoadWave("tada.wav");

	voicehandlebgm = audio_->PlayWave(titlebgm, true);

	texturehandlenumber = TextureManager::Load("number.png");
	for (int i = 0; i < 5; i++) {
		spritenumber[i] = Sprite::Create(texturehandlenumber, {300.0f + i * 26, 0});
	}

	texturehandlescore = TextureManager::Load("score.png");
	spritescore = Sprite::Create(texturehandlescore, {150, 0});

	for (int i = 0; i < 3; i++) {
		spritelife[i] = Sprite::Create(texturehandleplayer, {800.0f + i * 60, 10});
		spritelife[i]->SetSize({
		    40,
		    40,
		});
	}
}

void GameScene::Update() {
	switch (sceneMode) {
	case 0:
		GamePlayUpdate();

		break;

	case 1:
		TitleUpdate();
		GamePlayStart();

		gametimer += 1;
		break;

	case 2:
		GameOverUpdate();
		gametimer += 1;
		break;

	case 3:
		RuleUpdate();

		break;
	}
}

void GameScene::GamePlayUpdate() {
	PlayerUpdate();
	BeamUpdate();
	EnemyUpdate();
	Collision();
	StageUpdate();
	TuranukiUpdate();
	SpTuranukiUpdate();
	// UltUpdate();
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

	switch (sceneMode) {
	case 0:
		GamePlayDraw2DBack();

	case 2:
		GamePlayDraw2DBack();

		break;
	}
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

	switch (sceneMode) {
	case 0:
		GamePlayDraw3D();

	case 2:
		GamePlayDraw3D();

		break;
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

	switch (sceneMode) {
	case 0:
		GamePlayDraw2DNear();

		break;

	case 1:
		TitleDraw2DNear();

		break;

	case 2:
		GamePlayDraw2DNear();

		GameOverDraw2DNear();

		break;

	case 3:
		RuleDraw2DNear();

		break;
	}

	// スプライト描画後処理
	Sprite::PostDraw();

#pragma endregion
}

void GameScene::GamePlayDraw3D() {
	// for (int i = 0; i < 20; i++) {
	// modelstage->Draw(worldtransformstage[i], viewprojection, texturehandlestage);
	//}

	if (turanukiflag == 1) {
		modelturanuki->Draw(worldtransformturanuki, viewprojection, texturehandleturanuki);
	}

	if (playertimer % 4 < 2) {
		modelplayer->Draw(worldtransformplayer, viewprojection, texturehandleplayer);
	}

	for (int i = 0; i < 5; i++) {
		if (spturanukiflag[i] == 1) {
			modelspturanuki->Draw(
			    worldtransformspturanuki[i], viewprojection, texturehandlespturanuki);
		}
	}

	if (ultflag == 1) {
		modelult->Draw(worldtransformult, viewprojection, texturehandleult);
	}

	for (int i = 0; i < 10; i++) {
		if (beamflag[i] == 1) {
			modelbeam->Draw(worldtransformbeam[i], viewprojection, texturehandlebeam);
		}
	}

	for (int i = 0; i < 10; i++) {
		if (enemyflag[i] != 0) {
			modelenemy->Draw(worldtransformenemy[i], viewprojection, texturehandleenemy);
		}
	}
}

void GameScene::GamePlayDraw2DBack() { spriteBG->Draw(); }

void GameScene::GamePlayDraw2DNear() {
	DrawScore();

	DrawSkil();
}

void GameScene::PlayerUpdate() {

	worldtransformplayer.rotation_.x += 10.0f;

	if (input_->PushKey(DIK_RIGHT)) {
		worldtransformplayer.translation_.x += 0.1f;
		if (worldtransformplayer.translation_.x > 4.0f) {
			worldtransformplayer.translation_.x = 4.0f;
		}
	}

	if (input_->PushKey(DIK_LEFT)) {
		worldtransformplayer.translation_.x -= 0.1f;
		if (worldtransformplayer.translation_.x < -4.0f) {
			worldtransformplayer.translation_.x = -4.0f;
		}
	}

	if (input_->PushKey(DIK_UP)) {
		worldtransformplayer.translation_.y += 0.1f;
		if (worldtransformplayer.translation_.y > 2.84f) {
			worldtransformplayer.translation_.y = 2.8f;
		}
	}

	if (input_->PushKey(DIK_DOWN)) {
		worldtransformplayer.translation_.y -= 0.1f;
		if (worldtransformplayer.translation_.y < 0) {
			worldtransformplayer.translation_.y = 0;
		}
	}

	worldtransformplayer.matWorld_ = MakeAffineMatrix(
	    worldtransformplayer.scale_, worldtransformplayer.rotation_,
	    worldtransformplayer.translation_);

	worldtransformplayer.TransferMatrix();

	if (playertimer > 0) {
		playertimer--;
	}
}

void GameScene::BeamUpdate() {
	BeamMoob();
	BeamBorn();

	for (int i = 0; i < 10; i++) {
		worldtransformbeam[i].matWorld_ = MakeAffineMatrix(
		    worldtransformbeam[i].scale_, worldtransformbeam[i].rotation_,
		    worldtransformbeam[i].translation_);

		worldtransformbeam[i].TransferMatrix();
	}
}

void GameScene::BeamMoob() {
	for (int i = 0; i < 10; i++) {
		if (beamflag[i] == 1) {
			worldtransformbeam[i].translation_.z += 0.2f;
			worldtransformbeam[i].rotation_.x += 3.0f;
			if (worldtransformbeam[i].translation_.z > 40) {
				beamflag[i] = 0;
			}
		}
	}
}

void GameScene::BeamBorn() {

	for (int i = 0; i < 10; i++) {

		if (input_->PushKey(DIK_SPACE) && beamflag[i] == 0 && beamtimer == 0) {
			worldtransformbeam[i].translation_.x = worldtransformplayer.translation_.x;
			worldtransformbeam[i].translation_.y = worldtransformplayer.translation_.y;
			worldtransformbeam[i].translation_.z = worldtransformplayer.translation_.z;

			beamflag[i] = 1;
			beamtimer = 100;

			break;
		} else {
			beamtimer--;
			if (beamtimer < 0) {
				beamtimer = 0;
			}
		}
	}
}

void GameScene::TuranukiUpdate() {
	TuranukiBorn();
	TuranukiMove();

	worldtransformturanuki.matWorld_ = MakeAffineMatrix(
	    worldtransformturanuki.scale_, worldtransformturanuki.rotation_,
	    worldtransformturanuki.translation_);

	worldtransformturanuki.TransferMatrix();
}

void GameScene::TuranukiBorn() {

	if (input_->PushKey(DIK_V) && turanukitimer == 0) {
		turanukitimer = 700;
		turanukiflag = 1;
		worldtransformturanuki.translation_.x = worldtransformplayer.translation_.x;
		worldtransformturanuki.translation_.y = worldtransformplayer.translation_.y;
		worldtransformturanuki.translation_.z = worldtransformplayer.translation_.z;
	}

	if (turanukitimer > 0) {
		turanukitimer--;
	}
}

void GameScene::TuranukiMove() {

	if (turanukiflag == 1) {
		worldtransformturanuki.translation_.z += 0.3f;
		worldtransformturanuki.rotation_.x += 3.0f;
		if (worldtransformturanuki.translation_.z > 40) {
			turanukiflag = 0;
		}
	}
}

void GameScene::SpTuranukiUpdate() {

	SpTuranukiBorn();
	SpTuranukiMove();

	for (int i = 0; i < 5; i++) {
		worldtransformspturanuki[i].matWorld_ = MakeAffineMatrix(
		    worldtransformspturanuki[i].scale_, worldtransformspturanuki[i].rotation_,
		    worldtransformspturanuki[i].translation_);

		worldtransformspturanuki[i].TransferMatrix();
	}
}

void GameScene::SpTuranukiBorn() {

	if (input_->PushKey(DIK_B) && spturanukitimer == 0) {
		spturanukitimer = 2500;
		for (int i = 0; i < 5; i++) {
			if (i < 3) {
				spturanukiflag[i] = 1;
				worldtransformspturanuki[i].translation_.x =
				    worldtransformplayer.translation_.x + i * 2;
				worldtransformspturanuki[i].translation_.y = worldtransformplayer.translation_.y;
				worldtransformspturanuki[i].translation_.z = worldtransformplayer.translation_.z;
			} else {
				spturanukiflag[i] = 1;
				worldtransformspturanuki[i].translation_.x =
				    worldtransformplayer.translation_.x - (i - 2) * 2;
				worldtransformspturanuki[i].translation_.y = worldtransformplayer.translation_.y;
				worldtransformspturanuki[i].translation_.z = worldtransformplayer.translation_.z;
			}
		}
	}

	if (spturanukitimer > 0) {
		spturanukitimer--;
	}
}

void GameScene::SpTuranukiMove() {

	for (int i = 0; i < 5; i++) {
		if (spturanukiflag[i] == 1) {
			worldtransformspturanuki[i].translation_.z += 0.3f;
			worldtransformspturanuki[i].rotation_.x += 3.0f;
			if (worldtransformspturanuki[i].translation_.z > 40) {
				spturanukiflag[i] = 0;
			}
		}
	}
}

void GameScene::UltUpdate() {
	UltBorn();
	UltMove();

	worldtransformult.matWorld_ = MakeAffineMatrix(
	    worldtransformult.scale_, worldtransformult.rotation_, worldtransformult.translation_);

	worldtransformult.TransferMatrix();
}

void GameScene::UltBorn() {

	if (input_->PushKey(DIK_N) && ulttimer == 0) {
		ulttimer = 1000;
		ultflag = 1;
		worldtransformult.translation_.x = worldtransformplayer.translation_.x;
		worldtransformult.translation_.y = worldtransformplayer.translation_.y;
		worldtransformult.translation_.z = worldtransformplayer.translation_.z;
	}

	if (ulttimer > 0) {
		ulttimer--;
	}
}

void GameScene::UltMove() {

	if (ultflag == 1) {
		worldtransformult.translation_.z += 0.2f;
		if (worldtransformult.translation_.z > 40) {
			ultflag = 0;
		}
	}
}

void GameScene::EnemyUpdate() {
	EnemyMove();
	EnemyBorn();
	EnemyJunp();

	for (int i = 0; i < 10; i++) {
		worldtransformenemy[i].matWorld_ = MakeAffineMatrix(
		    worldtransformenemy[i].scale_, worldtransformenemy[i].rotation_,
		    worldtransformenemy[i].translation_);

		worldtransformenemy[i].TransferMatrix();
	}
}

void GameScene::EnemyMove() {
	for (int i = 0; i < 10; i++) {
		if (enemyflag[i] == 1) {
			worldtransformenemy[i].translation_.z -= 0.1f;
			worldtransformenemy[i].translation_.z -= ingametimer / 300.0f;
			worldtransformenemy[i].rotation_.x -= 0.1f;
			worldtransformenemy[i].translation_.x += enemyspeed[i];
			worldtransformenemy[i].translation_.y += enemyspeedy[i];
			if (worldtransformenemy[i].translation_.x > 4) {
				enemyspeed[i] = -enemyspeed[i];
			} else if (worldtransformenemy[i].translation_.x < -4) {
				enemyspeed[i] = -enemyspeed[i];
			}
			if (worldtransformenemy[i].translation_.y < 0) {
				enemyspeedy[i] = -enemyspeedy[i];
			} else if (worldtransformenemy[i].translation_.y > 2.8f) {
				enemyspeedy[i] = -enemyspeedy[i];
			}
			if (worldtransformenemy[i].translation_.z < -5) {
				enemyflag[i] = 0;
			}
		}
	}
}

void GameScene::EnemyBorn() {

	for (int i = 0; i < 10; i++) {
		if (enemyflag[i] == 0) {
			int x = rand() % 80;
			float x2 = (float)x / 10 - 4;
			int y = rand() % 20;
			float y2 = (float)y - 10;
			worldtransformenemy[i].translation_.y = 0;
			enemyspeedy[i] = 0.1f;

			if (rand() % 10 == 0) {
				if (rand() % 2 == 0) {
					enemyspeed[i] = 0.1f;
				} else {
					enemyspeed[i] = -0.1f;
				}
				enemyflag[i] = 1;
				worldtransformenemy[i].translation_.z = 40 + y2;
				worldtransformenemy[i].translation_.x = x2;
			}
			break;
		}
	}
}

void GameScene::Collision() {
	CollisionPlayerEnemy();
	CollisionBeamEnemy();
	CollisionTuranukiEnemy();
	CollisionSpTuranukiEnemy();
	// CollisionUltEnemy();
}

void GameScene::CollisionPlayerEnemy() {
	if (playertimer == 0) {
		for (int i = 0; i < 10; i++) {
			if (enemyflag[i] == 1) {
				float dx = abs(
				    worldtransformplayer.translation_.x - worldtransformenemy[i].translation_.x);
				float dy = abs(
				    worldtransformplayer.translation_.y - worldtransformenemy[i].translation_.y);
				float dz = abs(
				    worldtransformplayer.translation_.z - worldtransformenemy[i].translation_.z);

				if (dx < 1 && dy < 1 && dz < 1) {
					enemyflag[i] = 2;
					playerlife -= 1;
					enemyjunpspeed[i] = 1;
					playertimer = 60;

					audio_->PlayWave(playerhitse);

					if (playerlife == 0) {
						sceneMode = 2;
						audio_->StopWave(voicehandlebgm);
						voicehandlebgm = audio_->PlayWave(gameoverbgm, true);
					}
				}
			}
		}
	}
}

void GameScene::CollisionBeamEnemy() {
	for (int i = 0; i < 10; i++) {

		for (int j = 0; j < 10; j++) {
			if (enemyflag[i] == 1 && beamflag[j] == 1) {
				float dx = abs(
				    worldtransformbeam[j].translation_.x - worldtransformenemy[i].translation_.x);
				float dy = abs(
				    worldtransformbeam[j].translation_.y - worldtransformenemy[i].translation_.y);
				float dz = abs(
				    worldtransformbeam[j].translation_.z - worldtransformenemy[i].translation_.z);

				if (dx < 1 && dy < 1 && dz < 1) {
					audio_->PlayWave(enemyhitse);

					enemyjunpspeed[i] = 1;
					enemyflag[i] = 2;
					beamflag[j] = 0;
					gamescore += 1;
				}
			}
		}
	}
}

void GameScene::CollisionTuranukiEnemy() {
	for (int i = 0; i < 10; i++) {
		if (enemyflag[i] == 1 && turanukiflag == 1) {
			float dx =
			    abs(worldtransformenemy[i].translation_.x - worldtransformturanuki.translation_.x);
			float dy =
			    abs(worldtransformenemy[i].translation_.y - worldtransformturanuki.translation_.y);
			float dz =
			    abs(worldtransformenemy[i].translation_.z - worldtransformturanuki.translation_.z);

			if (dx < 1 && dy < 1 && dz < 1) {
				audio_->PlayWave(enemyhitse);

				enemyjunpspeed[i] = 1;
				enemyflag[i] = 2;
				gamescore += 1;
			}
		}
	}
}

void GameScene::CollisionSpTuranukiEnemy() {

	for (int i = 0; i < 10; i++) {
		for (int j = 0; j < 5; j++) {
			if (enemyflag[i] == 1 && spturanukiflag[j] == 1) {
				float dx =
				    abs(worldtransformenemy[i].translation_.x -
				        worldtransformspturanuki[j].translation_.x);
				float dy =
				    abs(worldtransformenemy[i].translation_.y -
				        worldtransformspturanuki[j].translation_.y);
				float dz =
				    abs(worldtransformenemy[i].translation_.z -
				        worldtransformspturanuki[j].translation_.z);

				if (dx < 1 && dy < 1 && dz < 1) {
					audio_->PlayWave(enemyhitse);

					enemyjunpspeed[i] = 1;
					enemyflag[i] = 2;
					gamescore += 1;
				}
			}
		}
	}
}

void GameScene::CollisionUltEnemy() {

	for (int i = 0; i < 10; i++) {
		if (enemyflag[i] == 1 && ultflag == 1) {
			float dx =
			    abs(worldtransformenemy[i].translation_.x - worldtransformult.translation_.x);
			float dy =
			    abs(worldtransformenemy[i].translation_.y - worldtransformult.translation_.y);
			float dz =
			    abs(worldtransformenemy[i].translation_.z - worldtransformult.translation_.z);

			if (dx < 1 && dy < 1 && dz < 1) {
				audio_->PlayWave(enemyhitse);

				enemyjunpspeed[i] = 1;
				enemyflag[i] = 3;
				gamescore += 1;
			}
		}

		if (enemyflag[i] == 3) {
			for (int j = 0; j < 10; j++) {
				float dx = abs(
				    worldtransformenemy[i].translation_.x - worldtransformenemy[j].translation_.x);
				float dy = abs(
				    worldtransformenemy[i].translation_.y - worldtransformenemy[j].translation_.y);
				float dz = abs(
				    worldtransformenemy[i].translation_.z - worldtransformenemy[i].translation_.z);

				if (dx < 1.5f && dy < 1.5f && dz < 1.5f) {
					enemyflag[j] = 3;
				}
			}
		}
	}
}

void GameScene::TitleUpdate() {

	if (input_->TriggerKey(DIK_RETURN)) {
		sceneMode = 3;
	}
}

void GameScene::TitleDraw2DNear() {

	spriteTitle->Draw();

	if (gametimer % 40 >= 20) {
		spriteenter->Draw();
	}
}

void GameScene::RuleUpdate() {

	if (input_->TriggerKey(DIK_RETURN)) {
		sceneMode = 0;

		audio_->StopWave(voicehandlebgm);
		voicehandlebgm = audio_->PlayWave(gamebgm, true);
	}
}

void GameScene::RuleDraw2DNear() { spriterule->Draw(); }

void GameScene::GameOverUpdate() {

	if (input_->TriggerKey(DIK_RETURN)) {
		sceneMode = 1;
		audio_->StopWave(voicehandlebgm);
		voicehandlebgm = audio_->PlayWave(titlebgm, true);
	}
}

void GameScene::GameOverDraw2DNear() {

	spritegameover->Draw();

	if (gametimer % 40 >= 20) {
		spriteenter->Draw();
	}
}

void GameScene::GamePlayStart() {

	for (int i = 0; i < 10; i++) {
		beamflag[i] = 0;
		enemyflag[i] = 0;
	}
	if (gametimer > 1000) {
		gametimer = 0;
	}

	for (int i = 0; i < 5; i++) {
		spturanukiflag[i] = 0;
	}
	ingametimer = 0;
	gamescore = 0;
	playerlife = 3;
	playertimer = 0;
	turanukitimer = 380;
	turanukiflag = 0;
	spturanukitimer = 2500;
	ulttimer = 1500;
	ultflag = 0;
	worldtransformplayer.translation_.x = 0;
	PlayerUpdate();
}

void GameScene::StageUpdate() {

	for (int i = 0; i < 20; i++) {
		worldtransformstage[i].translation_.z -= 0.1f;

		if (worldtransformstage[i].translation_.z < -5) {
			worldtransformstage[i].translation_.z += 40;
		}

		worldtransformstage[i].matWorld_ = MakeAffineMatrix(
		    worldtransformstage[i].scale_, worldtransformstage[i].rotation_,
		    worldtransformstage[i].translation_);

		worldtransformstage[i].TransferMatrix();
	}
}

void GameScene::EnemyJunp() {
	for (int i = 0; i < 10; i++) {
		if (enemyflag[i] == 2 || enemyflag[i] == 3) {
			worldtransformenemy[i].translation_.y += enemyjunpspeed[i];

			enemyjunpspeed[i] -= 0.1f;

			worldtransformenemy[i].translation_.x += enemyspeed[i] * 4;

			if (worldtransformenemy[i].translation_.y < -3) {
				enemyflag[i] = 0;
			}
		}
	}
}

void GameScene::DrawScore() {

	int eachnumber[5] = {};
	int number = gamescore;

	spritescore->Draw();

	int keta = 10000;
	for (int i = 0; i < 5; i++) {
		eachnumber[i] = number / keta;
		number = number % keta;
		keta = keta / 10;
	}

	for (int i = 0; i < 5; i++) {
		spritenumber[i]->SetSize({32, 64});
		spritenumber[i]->SetTextureRect({32.0f * eachnumber[i], 0}, {32, 64});
		spritenumber[i]->Draw();
	}

	for (int i = 0; i < playerlife; i++) {
		spritelife[i]->Draw();
	}
}

void GameScene::DrawSkil() {

	if (turanukitimer % 40 == 0) {
		modelv->Draw();
	}

	if (spturanukitimer % 40 == 0) {
		modelb->Draw();
	}
}