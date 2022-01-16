#include<Windows.h>
#include<d3d12.h>
#include<d3dx12.h>
#include<dxgi1_6.h>
#include<vector>
#include<string>
#include<DirectXMath.h>
#include<d3dcompiler.h>
#define DIRECTiNPUT_VERSION 

#include<DirectXTex.h>
#include<wrl.h>
#include "Input.h"
#include "WinApp.h"
#include "DirectXCommon.h"
#include "Sprite.h"
#include "Audio.h"
#include "Model.h"
#include "Object3d.h"
#include "Collision.h"

#pragma comment(lib,"d3dcompiler.lib")
#pragma comment(lib,"d3d12.lib")

#pragma comment(lib,"dxgi.lib")
#pragma comment(lib,"dinput8.lib")
#pragma comment(lib,"dxguid.lib")
using namespace DirectX;
using namespace Microsoft::WRL;
LRESULT WindowProc(HWND hwnd, UINT msg, WPARAM wparam, LPARAM lparam)
{
	switch (msg) {
	case WM_DESTROY:
		PostQuitMessage(0);
		return 0;
	}
	return DefWindowProc(hwnd, msg, wparam, lparam);
}

//テクスチャの最大枚数
const int SpriteSRVCount = 512;
#pragma region//構造体

#pragma endregion
#pragma region//関数

float easeInSine(float x) {
	return x * x * x;
}
float easeOutBack(float x) {
	return x == 1 ? 1 : 1 - powf(2, -10 * x);
}

#pragma endregion
#pragma region//WinMain
int WINAPI WinMain(HINSTANCE, HINSTANCE, LPSTR, int)
{
#pragma region//ポインタ置き場
	//ポインタ置き場
	Input* input = nullptr;
	WinApp* winApp = nullptr;
	DirectXCommon* dxCommon = nullptr;
	winApp = new WinApp();
	winApp->Initialize();
	//DirectXの初期化
	dxCommon = new DirectXCommon();
	dxCommon->Initialize(winApp);
	MSG msg{};
	//でバッグレイヤーのやつ
#ifdef _DEBUG
#endif
	//directx初期化
	HRESULT result;
#pragma endregion
#pragma region//定数バッファ
#pragma endregion
#pragma region//ビュー行列
	//ビュー変換行列
	XMMATRIX matview;
	XMFLOAT3 eye(0, 0, -100);
	XMFLOAT3 target(0, 0, 0);
	XMFLOAT3 up(0, 1, 0);
	matview = XMMatrixLookAtLH(XMLoadFloat3(&eye), XMLoadFloat3(&target), XMLoadFloat3(&up));
#pragma endregion
#pragma region//スプライト
	//スプライト共通データ
	if (!Sprite::StaticInitialize(dxCommon->GetDev(), WinApp::window_width, WinApp::window_height)) {
		assert(0);
		return 0;
	}

	const int SpriteMax = 20;
	const int SpriteNumberMax = 10;
	const int SpriteRankMax = 4;
	Sprite* sprite[SpriteMax] = { nullptr };
	// スプライト共通テクスチャ読み込み
	Sprite::LoadTexture(0, L"Resources/GAMETITLE.png");
	Sprite::LoadTexture(1, L"Resources/GAMECLEAR.png");
	Sprite::LoadTexture(2, L"Resources/PlayerHP.png");
	Sprite::LoadTexture(3, L"Resources/Background.png");
	Sprite::LoadTexture(4, L"Resources/RedBack.png");
	Sprite::LoadTexture(5, L"Resources/NextWave.png");
	Sprite::LoadTexture(6, L"Resources/LastWave.png");
	Sprite::LoadTexture(7, L"Resources/RESULT.png");
	Sprite::LoadTexture(8, L"Resources/EXPLATION.png");
	Sprite::LoadTexture(9, L"Resources/RESULTBACK.png");
	Sprite::LoadTexture(10, L"Resources/0.png");
	Sprite::LoadTexture(11, L"Resources/1.png");
	Sprite::LoadTexture(12, L"Resources/2.png");
	Sprite::LoadTexture(13, L"Resources/3.png");
	Sprite::LoadTexture(14, L"Resources/4.png");
	Sprite::LoadTexture(15, L"Resources/5.png");
	Sprite::LoadTexture(16, L"Resources/6.png");
	Sprite::LoadTexture(17, L"Resources/7.png");
	Sprite::LoadTexture(18, L"Resources/8.png");
	Sprite::LoadTexture(19, L"Resources/9.png");
	Sprite::LoadTexture(20, L"Resources/S.png");
	Sprite::LoadTexture(21, L"Resources/A.png");
	Sprite::LoadTexture(22, L"Resources/B.png");
	Sprite::LoadTexture(23, L"Resources/C.png");
	Sprite::LoadTexture(24, L"Resources/UI.png");
	Sprite::LoadTexture(25, L"Resources/Right.png");
	Sprite::LoadTexture(26, L"Resources/Left.png");
	Sprite::LoadTexture(27, L"Resources/UP.png");
	Sprite::LoadTexture(28, L"Resources/Down.png");
	sprite[0] = Sprite::Create(0, { 0.0f,0.0f });
	sprite[1] = Sprite::Create(1, { 0.0f,0.0f });
	sprite[2] = Sprite::Create(2, { 0.0f,0.0f });
	sprite[3] = Sprite::Create(3, { 0.0f,0.0f });
	sprite[4] = Sprite::Create(4, { 0.0f,0.0f });
	sprite[5] = Sprite::Create(5, { 0.0f,0.0f });
	sprite[6] = Sprite::Create(6, { 0.0f,0.0f });
	sprite[7] = Sprite::Create(7, { 0.0f,0.0f });
	sprite[8] = Sprite::Create(8, { 0.0f,0.0f });
	sprite[9] = Sprite::Create(9, { 0.0f,0.0f });
	sprite[10] = Sprite::Create(24, { 0.0f,0.0f });
	sprite[11] = Sprite::Create(25, { 0.0f,0.0f });
	sprite[12] = Sprite::Create(26, { 0.0f,0.0f });
	sprite[13] = Sprite::Create(27, { 0.0f,0.0f });
	sprite[14] = Sprite::Create(28, { 0.0f,0.0f });
	Sprite* spriteFirstNumber[SpriteNumberMax] = { nullptr };
	spriteFirstNumber[0] = Sprite::Create(10, { 0.0f,0.0f });
	spriteFirstNumber[1] = Sprite::Create(11, { 0.0f,0.0f });
	spriteFirstNumber[2] = Sprite::Create(12, { 0.0f,0.0f });
	spriteFirstNumber[3] = Sprite::Create(13, { 0.0f,0.0f });
	spriteFirstNumber[4] = Sprite::Create(14, { 0.0f,0.0f });
	spriteFirstNumber[5] = Sprite::Create(15, { 0.0f,0.0f });
	spriteFirstNumber[6] = Sprite::Create(16, { 0.0f,0.0f });
	spriteFirstNumber[7] = Sprite::Create(17, { 0.0f,0.0f });
	spriteFirstNumber[8] = Sprite::Create(18, { 0.0f,0.0f });
	spriteFirstNumber[9] = Sprite::Create(19, { 0.0f,0.0f });
	Sprite* spriteSecondNumber[SpriteNumberMax] = { nullptr };
	spriteSecondNumber[0] = Sprite::Create(10, { 0.0f,0.0f });
	spriteSecondNumber[1] = Sprite::Create(11, { 0.0f,0.0f });
	spriteSecondNumber[2] = Sprite::Create(12, { 0.0f,0.0f });
	spriteSecondNumber[3] = Sprite::Create(13, { 0.0f,0.0f });
	spriteSecondNumber[4] = Sprite::Create(14, { 0.0f,0.0f });
	spriteSecondNumber[5] = Sprite::Create(15, { 0.0f,0.0f });
	spriteSecondNumber[6] = Sprite::Create(16, { 0.0f,0.0f });
	spriteSecondNumber[7] = Sprite::Create(17, { 0.0f,0.0f });
	spriteSecondNumber[8] = Sprite::Create(18, { 0.0f,0.0f });
	spriteSecondNumber[9] = Sprite::Create(19, { 0.0f,0.0f });
	Sprite* spriteThirdNumber[SpriteNumberMax] = { nullptr };
	spriteThirdNumber[0] = Sprite::Create(10, { 0.0f,0.0f });
	spriteThirdNumber[1] = Sprite::Create(11, { 0.0f,0.0f });
	spriteThirdNumber[2] = Sprite::Create(12, { 0.0f,0.0f });
	spriteThirdNumber[3] = Sprite::Create(13, { 0.0f,0.0f });
	spriteThirdNumber[4] = Sprite::Create(14, { 0.0f,0.0f });
	spriteThirdNumber[5] = Sprite::Create(15, { 0.0f,0.0f });
	spriteThirdNumber[6] = Sprite::Create(16, { 0.0f,0.0f });
	spriteThirdNumber[7] = Sprite::Create(17, { 0.0f,0.0f });
	spriteThirdNumber[8] = Sprite::Create(18, { 0.0f,0.0f });
	spriteThirdNumber[9] = Sprite::Create(19, { 0.0f,0.0f });
	Sprite* spriteFourNumber[SpriteNumberMax] = { nullptr };
	spriteFourNumber[0] = Sprite::Create(10, { 0.0f,0.0f });
	spriteFourNumber[1] = Sprite::Create(11, { 0.0f,0.0f });
	spriteFourNumber[2] = Sprite::Create(12, { 0.0f,0.0f });
	spriteFourNumber[3] = Sprite::Create(13, { 0.0f,0.0f });
	spriteFourNumber[4] = Sprite::Create(14, { 0.0f,0.0f });
	spriteFourNumber[5] = Sprite::Create(15, { 0.0f,0.0f });
	spriteFourNumber[6] = Sprite::Create(16, { 0.0f,0.0f });
	spriteFourNumber[7] = Sprite::Create(17, { 0.0f,0.0f });
	spriteFourNumber[8] = Sprite::Create(18, { 0.0f,0.0f });
	spriteFourNumber[9] = Sprite::Create(19, { 0.0f,0.0f });
	Sprite* spriteRank[SpriteRankMax] = { nullptr };
	spriteRank[0] = Sprite::Create(20, { 0.0f,0.0f });
	spriteRank[1] = Sprite::Create(21, { 0.0f,0.0f });
	spriteRank[2] = Sprite::Create(22, { 0.0f,0.0f });
	spriteRank[3] = Sprite::Create(23, { 0.0f,0.0f });
	XMFLOAT4 SpriteColor = { 1.0f,1.0f,1.0f,0.5f };
	sprite[5]->SetColor(SpriteColor);
	sprite[6]->SetColor(SpriteColor);
	sprite[10]->SetPosition({ 0.0f,420.0f });
	sprite[11]->SetPosition({ 150.0f,520.0f });
	sprite[12]->SetPosition({ 30.0f,520.0f });
	sprite[13]->SetPosition({ 90.0f,450.0f });
	sprite[14]->SetPosition({ 90.0f,590.0f });
	int SpriteColorNumber = 0;
	int SpriteColorCount = 0;
	int ScoreFirstCount = 0;
	int ScoreSecondCount = 0;
	int ScoreThirdCount = 0;
	int ScoreFourCount = 0;
	for (int i = 0; i < SpriteNumberMax; i++) {
		spriteFirstNumber[i]->SetPosition({ 900,60 });
		spriteSecondNumber[i]->SetPosition({ 800,60 });
		spriteThirdNumber[i]->SetPosition({ 700,60 });
		spriteFourNumber[i]->SetPosition({ 600,60 });
	}

	for (int i = 0; i < SpriteRankMax; i++) {
		spriteRank[i]->SetPosition({ 720,330 });
	}
#pragma endregion
#pragma region//モデル読み込み
	if (!Object3d::StaticInitialize(dxCommon->GetDev(), WinApp::window_width, WinApp::window_height)) {
		assert(0);
		return 0;
	}
	const int Block_NUM = 100;
	int WaveBlock = 20;
	const int Particle_NUM = 20;
	Model* Player_model = Model::LoadFromOBJ("Player");
	Object3d* Player_object = Object3d::Create();
	Player_object->SetModel(Player_model);
	//障害物
	Model* Block_model[Block_NUM];
	for (int i = 0; i < Block_NUM; i++) {
		Block_model[i] = Model::LoadFromOBJ("Block");
	}
	Object3d* Block_object[Block_NUM];
	XMFLOAT3 BlockScale[Block_NUM];
	for (int i = 0; i < Block_NUM; i++) {
		Block_object[i] = Object3d::Create();
		Block_object[i]->SetModel(Block_model[i]);
		BlockScale[i] = { 3.0f,3.0f,3.0f };
		Block_object[i]->SetScale({ BlockScale[i] });
	}
	//パーティクル
	Model* Particle_model[Particle_NUM];
	for (int i = 0; i < Particle_NUM; i++) {
		Particle_model[i] = Model::LoadFromOBJ("Particle");
	}
	Object3d* Particle_object[Particle_NUM];
	XMFLOAT3 ParticleScale[Particle_NUM];
	for (int i = 0; i < Particle_NUM; i++) {
		Particle_object[i] = Object3d::Create();
		Particle_object[i]->SetModel(Particle_model[i]);
		ParticleScale[i] = { 3.0f,3.0f,3.0f };
		Particle_object[i]->SetScale({ ParticleScale[i] });
	}

#pragma endregion
#pragma region//オーディオ
	//オーディオ
	const int AudioMax = 3;
	Audio* audio = new Audio;
	if (!audio->Initialize()) {
		assert(0);
		return 1;
	}
	audio->LoadSound(0, "Resources/Sound/vigilante.wav");
	audio->LoadSound(1, "Resources/Sound/se_21_siren02.wav");
	audio->LoopWave(0, 0.4f);
#pragma endregion
#pragma region//プレイヤー
	XMFLOAT3 PlayerPosition;
	PlayerPosition = { -65.0f,5.0f,0.0f };
	XMFLOAT3 PlayerRotation;
	PlayerRotation = { 0,180,0 };
	Player_object->SetPosition(PlayerPosition);
	Player_object->SetRotation(PlayerRotation);
	Player_object->SetScale({ 3.0f,3.0f,3.0f });
	int HitFlag = 0;
	int LaneNumber = 0;
	int HighNumber = 0;
	float frame = 0.0f;
	float frameMax = 20.0f;
	float initPositionX = 0.0f;
	float initPositionY = 0.0f;
	float initRotation = 0.0f;
	int MoveNumber = 0;
	int hp = 20;
#pragma endregion
#pragma region//障害物
	int ResPornTimer[Block_NUM];
	XMFLOAT3 BlockPosition[Block_NUM];
	XMFLOAT3 BlockRotation[Block_NUM];
	int BlockisAlive[Block_NUM];
	int BlockRandLane[Block_NUM];
	int BlockRandHigh[Block_NUM];
	int BlockRandZ[Block_NUM];
	int BlockbreakCount[Block_NUM];
	for (int i = 0; i < Block_NUM; i++) {
		BlockPosition[i] = { 0.0f,0.0f,0.0f };
		BlockRotation[i] = { 0.0f,0.0f,0.0f };
		Block_object[i]->SetRotation(BlockRotation[i]);
		Block_object[i]->SetPosition(BlockPosition[i]);
		ResPornTimer[i] = 10;
		BlockisAlive[i] = 0;
		BlockRandLane[i] = 0;
		BlockRandHigh[i] = 0;
		BlockRandZ[i] = 0;
		BlockbreakCount[i] = 0;
	}
#pragma endregion
#pragma region//パーティクル
	int particleAlive[Particle_NUM];
	XMFLOAT3 particlePosition[Particle_NUM];

	float particleXG[Particle_NUM];
	float particleYG[Particle_NUM];
	for (int i = 0; i < Particle_NUM; i++) {
		particleAlive[i] = 0;
		particleXG[i] = 0.0f;
		particleYG[i] = 0.0f;
	}

#pragma endregion
#pragma region//キー処理
	//キー処理
	//入力の初期化
	input = new Input();
	input->Initialize(winApp);
#pragma endregion
#pragma region//カメラ
	XMVECTOR v0 = {50,0,40,0 };
	XMMATRIX rotM;// = XMMatrixIdentity();
	XMVECTOR eye2;
	XMVECTOR target2 = { PlayerPosition.x, PlayerPosition.y,PlayerPosition.z, 0 };
	XMVECTOR up2 = { 0,0.0f,0,0 };
	//カメラの回転角
	float PI = 3.14f;
	float angle = 0.0f;
	float Cameraradius = 0.0f;
	float CameraSpeed = 0.0f;
	float Camerascale = 0.0f;
	float CameraCircleX = 0.0f;
	float CameraCircleZ = 0.0f;
#pragma endregion
#pragma region//シーン変数
	int Scene = 0;
	enum Scene {
		title,
		Explation,
		gamePlay,
		Wave,
		Result
	};
	int WaveTimer = 0;
	int WaveCount = 1;
	int SpaceCount = 0;
	int StartFlag = 0;
#pragma endregion
	Collision* collision = nullptr;
#pragma region//ループ処理
	while (true) {
#pragma region//更新処理
		//ウィンドウメッセージ処理
		if (winApp->ProcessMessage()) {
			break;
		}
		//キーの更新
		input->Update();
#pragma region//タイトル
		if (Scene == title) {
			if (input->TriggerKey(DIK_SPACE)) {
				Scene = Explation;
				SpaceCount++;
			}
		}
#pragma endregion
#pragma region//説明画面
		if (Scene == Explation) {
			if (input->TriggerKey(DIK_SPACE)) {
				SpaceCount++;
			}

			if (SpaceCount == 3) {
				Scene = gamePlay;
				SpaceCount = 0;
			}
		}
#pragma endregion
		//毎フレーム処理
		//プレイヤー処理
#pragma region//ゲームプレイ中
		if (Scene == gamePlay) {
			if (StartFlag == 0) {
				if (v0.m128_f32[2] != -40.0f) {
					v0.m128_f32[2] -= 0.5f;
				}
				if (v0.m128_f32[0] != 0.0f) {
					v0.m128_f32[0] -= 0.25f;
				}

				if (v0.m128_f32[0] == 0.0f && v0.m128_f32[2] == -40.0f) {
					StartFlag = 1;
				}
			}
			//経過時間を決める
			else {
				WaveTimer++;
				if (WaveTimer % 220 == 0 && WaveTimer != 0) {
					ScoreSecondCount++;
				}

				if (ScoreSecondCount >= 10) {
					ScoreSecondCount = 0;
					ScoreThirdCount++;
				}

				if (ScoreThirdCount >= 10) {
					ScoreThirdCount = 0;
					ScoreFourCount++;
				}

				if (WaveTimer == 3000 && WaveCount != 6) {
					Scene = Wave;
					WaveCount++;
					WaveTimer = 0;
				}

				//一定のWaveでゲームクリア
				if (WaveCount == 6) {
					Scene = Result;
				}

				if (MoveNumber == 0) {
					if (input->TriggerKey(DIK_DOWN) && HighNumber == 0 && hp >= 1) {
						audio->PlayWave("Resources/Sound/se_sad04.wav", 0.1f);
						initPositionY = PlayerPosition.y;
						initRotation = PlayerRotation.z;
						frame = 0;
						HighNumber = 1;
						MoveNumber = 1;
					}

					if (input->TriggerKey(DIK_UP) && HighNumber == 1 && hp >= 1) {
						audio->PlayWave("Resources/Sound/se_sad04.wav", 0.1f);
						initPositionY = PlayerPosition.y;
						initRotation = PlayerRotation.z;
						frame = 0;
						HighNumber = 0;
						MoveNumber = 2;
					}

					if (input->TriggerKey(DIK_LEFT) && LaneNumber >= 1 && hp >= 1) {
						audio->PlayWave("Resources/Sound/se_sad04.wav", 0.1f);
						initPositionX = PlayerPosition.x;
						initRotation = PlayerRotation.z;
						LaneNumber--;
						frame = 0;
						MoveNumber = 3;
					}
					if (input->TriggerKey(DIK_RIGHT) && LaneNumber <= 2 && hp >= 1) {
						audio->PlayWave("Resources/Sound/se_sad04.wav", 0.1f);
						initPositionX = PlayerPosition.x;
						initRotation = PlayerRotation.z;
						LaneNumber++;
						frame = 0;
						MoveNumber = 4;
					}
				}

				//イージングで移動
				if (MoveNumber == 1) {
					PlayerPosition.y = initPositionY - 20.0f * easeInSine(frame / frameMax);
					PlayerRotation.z = initRotation - 360.0f * easeInSine(frame / frameMax);
					if (frame != frameMax) {
						frame = frame + 1;
					} else {
						MoveNumber = 0;
					}
				}

				if (MoveNumber == 2) {
					PlayerPosition.y = initPositionY + 20.0f * easeInSine(frame / frameMax);
					PlayerRotation.z = initRotation + 360.0f * easeInSine(frame / frameMax);
					if (frame != frameMax) {
						frame = frame + 1;
					} else {
						MoveNumber = 0;
					}
				}

				if (MoveNumber == 3) {
					PlayerPosition.x = initPositionX - 15.0f * easeInSine(frame / frameMax);
					PlayerRotation.z = initRotation - 360.0f * easeInSine(frame / frameMax);
					if (frame != frameMax) {
						frame = frame + 1;
					} else {
						MoveNumber = 0;
					}
				}

				if (MoveNumber == 4) {
					PlayerPosition.x = initPositionX + 15.0f * easeInSine(frame / frameMax);
					PlayerRotation.z = initRotation + 360.0f * easeInSine(frame / frameMax);
					if (frame != frameMax) {
						frame = frame + 1;
					} else {
						MoveNumber = 0;
					}
				}
				//ブロック出現
				for (int i = 0; i < Block_NUM; i++) {
					if (BlockisAlive[i] == 0) {
						ResPornTimer[i]--;
						if (ResPornTimer[i] <= 0) {
							ResPornTimer[i] = 10;
							BlockisAlive[i] = 1;
							BlockScale[i] = { 3.0f,3.0f,3.0f };
							BlockRandLane[i] = rand() % 4;
							BlockRandHigh[i] = rand() % 2;
							if (WaveCount == 1) {
								BlockRandZ[i] = rand() % 3500;
							} else if (WaveCount == 2) {
								BlockRandZ[i] = rand() % 2500;
							} else if (WaveCount == 3) {
								BlockRandZ[i] = rand() % 1500;
							} else if (WaveCount == 4) {
								BlockRandZ[i] = rand() % 1250;
							} else if (WaveCount == 5) {
								BlockRandZ[i] = rand() % 1000;
							}
							BlockPosition[i].z = PlayerPosition.z + BlockRandZ[i] + 200;
							if (BlockRandHigh[i] == 0) {
								BlockPosition[i].y = 5.0f;
							} else if (BlockRandHigh[i] == 1) {
								BlockPosition[i].y = -15.0f;
							}
							if (BlockRandLane[i] == 0) {
								BlockPosition[i].x = -65.0f;
							} else if (BlockRandLane[i] == 1) {
								BlockPosition[i].x = -50.0f;
							} else if (BlockRandLane[i] == 2) {
								BlockPosition[i].x = -35.0f;
							} else if (BlockRandLane[i] == 3) {
								BlockPosition[i].x = -20.0f;
							}
						}
					}
					//一定の距離で消滅
					if (BlockPosition[i].z <= PlayerPosition.z - 50) {
						BlockisAlive[i] = 0;
					}
				}
				for (int i = 0; i < Block_NUM; i++) {
					if (BlockisAlive[i] == 1) {
						BlockRotation[i].y += 5.0f;
				
						if (collision->SphereCollision(BlockPosition[i].x, BlockPosition[i].y, BlockPosition[i].z,4,
							PlayerPosition.x, PlayerPosition.y, PlayerPosition.z, 4) == 1 && hp >= 1) {
							BlockbreakCount[i]++;
							BlockPosition[i].z = PlayerPosition.z + 4.25;
						}
						
						if (BlockbreakCount[i] == 7) {
							ScoreSecondCount += 1;
							BlockScale[i].x -= 1.0f;
							BlockScale[i].y -= 1.0f;
							BlockScale[i].z -= 1.0f;
							BlockbreakCount[i] = 0;
							hp--;
						}
					}

					if ((BlockScale[i].x <= 0.0f) && (BlockScale[i].y <= 0.0f) && (BlockScale[i].z <= 0.0f)) {
						BlockisAlive[i] = 0;
						if (ResPornTimer[i] == 10) {
							audio->PlayWave("Resources/Sound/se_30103.wav", 0.3f);
							for (int j = 0; j < Particle_NUM; j++) {
								particlePosition[j] = BlockPosition[i];
								particleAlive[j] = 1;
								particleXG[j] = rand() % 6 - 3;
								particleYG[j] = rand() % 6;
							}
						}
					}
				}
				//パーティクル発生
				for (int i = 0; i < Particle_NUM; i++) {
					if (particleAlive[i] == 1) {
						particleYG[i] -= 0.2;
						particlePosition[i].x += particleXG[i];
						particlePosition[i].y += particleYG[i];
						if (particlePosition[i].y <= -200) {
							particleAlive[i] = 0;
						}
					}
				}
				PlayerPosition.z += 0.75;

				if (hp <= 0) {
					PlayerPosition.y -= 0.1f;
					PlayerRotation.x += 1.0f;
					PlayerRotation.y += 1.0f;
					if (PlayerPosition.y <= -30.0f) {
						Scene = Result;
					}
				}
			}
		}
#pragma endregion
#pragma region//クリア
		if (Scene == Result) {
			//もう一回ゲーム画面
			if (input->TriggerKey(DIK_SPACE)) {
				//様々なものの初期化
				Scene = gamePlay;
				PlayerPosition = { -65.0f,5.0f,0.0f };
				PlayerRotation = { 0,180,0 };
				hp = 20;
				LaneNumber = 0;
				HighNumber = 0;
				WaveTimer = 0;
				WaveCount = 1;
				for (int i = 0; i < Block_NUM; i++) {
					BlockPosition[i] = { 0.0f,0.0f,0.0f };
					BlockRotation[i] = { 0.0f,0.0f,0.0f };
					ResPornTimer[i] = 0;
					BlockisAlive[i] = 0;
				}
				SpriteColorNumber = 0;
				SpriteColorCount = 0;
				ScoreFirstCount = 0;
				ScoreSecondCount = 0;
				ScoreThirdCount = 0;
				ScoreFourCount = 0;
				v0 = { 50,0,40,0 };
				StartFlag = 0;
			}
		}
#pragma endregion
#pragma region//ウェーブ
		if (Scene == Wave) {
			if (SpriteColorCount == 0) {
			}
			if (SpriteColor.w <= 0.0f) {
				SpriteColorNumber = 1;
				SpriteColorCount++;
			} if (SpriteColor.w >= 1.0f) {
				SpriteColorNumber = 0;
				SpriteColorCount++;
			}

			if (SpriteColorNumber == 0) {
				SpriteColor.w -= 0.025;
			}

			else {
				SpriteColor.w += 0.025f;
			}

			if (SpriteColorCount == 5) {
				hp += 9;
				if (hp >= 20) {
					hp = 20;
				}
				Scene = gamePlay;
				SpriteColorCount = 0;
				for (int i = 0; i < Block_NUM; i++) {
					BlockisAlive[i] = 0;
					ResPornTimer[i] = 0;
				}
			}
		}
#pragma endregion
		//移動のやつ
		//カメラの注視点をプレイヤーの位置に固定
		target2.m128_f32[2] = PlayerPosition.z;
		//カメラ追従用の処理
		target2.m128_f32[0] = PlayerPosition.x;
		target2.m128_f32[1] = 0;
		//行列を作り直す
		rotM = XMMatrixRotationX(XMConvertToRadians(angle));
		XMVECTOR v;
		v = XMVector3TransformNormal(v0, rotM);
		eye2 = target2 + v;
		matview = XMMatrixLookAtLH((eye2), (target2), XMLoadFloat3(&up));
		Player_object->SetPosition(PlayerPosition);
		Player_object->SetRotation(PlayerRotation);
		Player_object->Update(matview);
		for (int i = 0; i < Block_NUM; i++) {
			Block_object[i]->SetPosition(BlockPosition[i]);
			Block_object[i]->SetRotation(BlockRotation[i]);
			Block_object[i]->SetScale(BlockScale[i]);
			Block_object[i]->Update(matview);
		}

		for (int i = 0; i < Particle_NUM; i++) {
			Particle_object[i]->SetPosition(particlePosition[i]);
			Particle_object[i]->Update(matview);
		}


#pragma endregion
#pragma region//描画
		//びょうがこまんど
		dxCommon->PreDraw();
		Sprite::PreDraw(dxCommon->GetCmdList());
		sprite[3]->Draw();
		// 背景スプライト描画
		if ((Scene == Explation) || (Scene == Result)) {
			sprite[9]->Draw();
		}
		if (Scene == Explation) {
			sprite[8]->Draw();
		}
		if (Scene == Result) {
			sprite[7]->Draw();
			spriteFirstNumber[ScoreFirstCount]->Draw();
			spriteSecondNumber[ScoreSecondCount]->Draw();
			spriteThirdNumber[ScoreThirdCount]->Draw();
			spriteFourNumber[ScoreFourCount]->Draw();
			if (ScoreFourCount >= 1 && ScoreThirdCount >= 2) {
				spriteRank[0]->Draw();
			} else if(ScoreFourCount == 0 && ScoreThirdCount >= 8){
				spriteRank[1]->Draw();
			} else if (ScoreFourCount == 0 && ScoreThirdCount >= 5) {
				spriteRank[2]->Draw();
			} else if (ScoreFourCount == 0 && ScoreThirdCount <= 4) {
				spriteRank[3]->Draw();
			}
		}
		if (Scene == Wave) {
			sprite[4]->Draw();
			sprite[5]->SetColor(SpriteColor);
			sprite[6]->SetColor(SpriteColor);
			if (WaveCount != 5) {
				sprite[5]->Draw();
			} else {
				sprite[6]->Draw();
			}
		}
		dxCommon->ClearDepthBuffer();
		// スプライト描画後処理
		Sprite::PostDraw();
		////4.描画コマンドここから
		dxCommon->GetCmdList()->IASetPrimitiveTopology(D3D_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
		Object3d::PreDraw(dxCommon->GetCmdList());
		//描画コマンド
		//背景
		if (Scene == gamePlay) {
			Player_object->Draw();
			for (int i = 0; i < Block_NUM; i++) {
				if (BlockisAlive[i] == 1) {
					Block_object[i]->Draw();
				}
			}

			for (int i = 0; i < Particle_NUM; i++) {
				if (particleAlive[i] == 1) {
					Particle_object[i]->Draw();
				}
			}
		}

		Sprite::PreDraw(dxCommon->GetCmdList());
		if (Scene == title) {
			sprite[0]->Draw();
		}
		Sprite::PostDraw();
		Sprite::PreDraw(dxCommon->GetCmdList());
		if (Scene == gamePlay) {
			sprite[10]->Draw();
			if (LaneNumber != 3) {
				sprite[11]->Draw();
			}
			if (LaneNumber != 0) {
				sprite[12]->Draw();
			}
			if (HighNumber == 1) {
				sprite[13]->Draw();
			}
			else {
				sprite[14]->Draw();
			}
			sprite[2]->Draw();
			sprite[2]->SetSize({ (float)(hp * 25.8),24.0f });
		}

		Sprite::PostDraw();
	
		Sprite::PostDraw();
		////描画コマンド　ここまで
		Object3d::PostDraw();
		dxCommon->PostDraw();
#pragma endregion
	}
#pragma endregion
#pragma region//開放
	winApp->Finalize();
	for (int i = 0; i < SpriteMax; i++) {
		delete sprite[i];
	}
	//入力開放
	delete input;
	delete audio;
	delete Player_model;
	delete Player_object;
	for (int i = 0; i < Block_NUM; i++) {
		delete Block_model[i];
	}
	for (int i = 0; i < Block_NUM; i++) {
		delete Block_object[i];
	}
	for (int i = 0; i < Particle_NUM; i++) {
		delete Particle_model[i];
	}
	for (int i = 0; i < Particle_NUM; i++) {
		delete Particle_object[i];
	}
	delete winApp;
	delete dxCommon;

	winApp = nullptr;
	return 0;
#pragma endregion
}
#pragma endregion
