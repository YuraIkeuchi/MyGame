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
#include "Block.h"
#include "BackGround.h"
#include "Player.h"

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
struct PipelineSet {
	ComPtr<ID3D12PipelineState> pipelinestate;
	ComPtr<ID3D12RootSignature> rootsignature;
};
struct Object3d {
	ID3D12Resource* constBuff;

	D3D12_CPU_DESCRIPTOR_HANDLE cpuDescHandleCBV;

	D3D12_GPU_DESCRIPTOR_HANDLE gpuDescHandleCBV;

	XMFLOAT3 scale = { 1,1,1 };
	XMFLOAT3 rotation = { 0,0,0 };
	XMFLOAT3 position = { 0,0,0 };

	XMMATRIX matWorld;

	Object3d* parent = nullptr;
};

struct VertexPosUv {
	XMFLOAT3 pos;
	XMFLOAT2 uv;
};


//定数バッファ用データ構造体
struct ConstBufferData {
	XMFLOAT4 color;
	XMMATRIX mat;
};

//頂点データ構造体
struct Vertex
{
	XMFLOAT3 pos;
	XMFLOAT3 normal;
	XMFLOAT2 uv;
};

#pragma endregion
#pragma region//関数

int collision2(const float& X1, const float& Y1, const float& R1, const float& X2, const float& Y2, const float& R2) {
	int a = X1 - X2;
	int b = Y1 - Y2;
	int distance = sqrtf(a * a + b * b);
	int radius = R1 + R2;
	if (distance <= radius) {
		return TRUE;
	}
}

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

	const int SpriteMax = 10;
	Sprite* sprite[SpriteMax] = { nullptr };
	// スプライト共通テクスチャ読み込み
	/*SpriteCommonLoadTexture(spriteCommon, 0, L"Resources/texture.png", dxCommon->GetDev());
	SpriteCommonLoadTexture(spriteCommon, 1, L"Resources/house.png", dxCommon->GetDev());*/
	Sprite::LoadTexture(0, L"Resources/GAMETITLE.png");
	Sprite::LoadTexture(1, L"Resources/END.png");
	Sprite::LoadTexture(2, L"Resources/EnemyHP.png");
	sprite[0] = Sprite::Create(0, { 0.0f,0.0f });
	sprite[1] = Sprite::Create(1, { 0.0f,0.0f });
	sprite[2] = Sprite::Create(2, { 0.0f,0.0f });
	XMFLOAT2 SpritePosition = sprite[0]->GetPosition();
#pragma endregion
#pragma region//オーディオ
	//オーディオ
	const int AudioMax = 3;
	Audio* audio = new Audio;
	if (!audio->Initialize()) {
		assert(0);
		return 1;
	}
	audio->LoadSound(0, "Resources/Sound/kadai_BGM.wav");
	audio->LoopWave(0, 0.5f);
#pragma endregion
#pragma region//プレイヤー
	Player* player;

	if (!player->StaticInitialize(dxCommon->GetDev(), WinApp::window_width, WinApp::window_height)) {
		assert(0);
		return 1;
	}
	player = Player::Create();
	player->Update(matview);

	XMFLOAT3 PlayerPosition;
	PlayerPosition = player->GetPosition();

	PlayerPosition = { -65.0f,5.0f,0.0f };
	player->SetPosition(PlayerPosition);
	XMFLOAT3 PlayerRotation;
	PlayerRotation = player->GetRotaition();
	int HitFlag = 0;
	int LaneNumber = 0;
	int HighNumber = 0;
	float frame = 0.0f;
	float frameMax = 30.0f;
	float initPositionX = 0.0f;
	float initPositionY = 0.0f;
	float initRotation = 0.0f;
	int MoveNumber = 0;
#pragma endregion
#pragma region//障害物
	const int Block_NUM = 40;
	
	Block* block[Block_NUM];
	int ResPornTimer[Block_NUM];
	XMFLOAT3 BlockPosition[Block_NUM];
	for (int i = 0; i < _countof(block); i++) {
		if (!block[i]->StaticInitialize(dxCommon->GetDev(), WinApp::window_width, WinApp::window_height)) {
			assert(0);
			return 1;
		}

		block[i] = Block::Create();
		block[i]->Update(matview);
		block[i]->Shot(player);
		ResPornTimer[i] = 50;
		BlockPosition[i] = block[i]->GetPosition();
	}

	
	
#pragma endregion
#pragma region//背景
	BackGround* background;
	if (!background->StaticInitialize(dxCommon->GetDev(), WinApp::window_width, WinApp::window_height)) {
		assert(0);
		return 1;
	}
	//XMFLOAT3 BackPosition;
	//BackPosition = background->GetPosition();

	background = BackGround::Create();
	background->Update(matview);
#pragma endregion
#pragma region//キー処理
	//キー処理
	//入力の初期化
	input = new Input();
	input->Initialize(winApp);
#pragma endregion
#pragma region//カメラ
	XMVECTOR v0 = { 0,0,-1,0 };
	XMMATRIX rotM;// = XMMatrixIdentity();
	XMVECTOR eye2;
	XMVECTOR target2 = { PlayerPosition.x, PlayerPosition.y,PlayerPosition.z, 0 };
	XMVECTOR up2 = { 0,0.0f,0,0 };
	//カメラの回転角
	float angle = 0.0f;
#pragma endregion
#pragma region//シーン変数
	int Scene = 0;
	enum Scene {
		title,
		gamePlay,
		gameOver,
		gameClear
	};
#pragma endregion
#pragma region//ループ処理
	while (true) {
#pragma region//更新処理
		//ウィンドウメッセージ処理
		if (winApp->ProcessMessage()) {
			break;
		}
		PlayerPosition = player->GetPosition();
		PlayerRotation = player->GetRotaition();
		SpritePosition = sprite[0]->GetPosition();
		for (int i = 0; i < _countof(block); i++) {
			BlockPosition[i] = block[i]->GetPosition();
		}
		//キーの更新
		input->Update();
#pragma region//タイトル
		if (Scene == title) {
			if (input->TriggerKey(DIK_SPACE)) {
				Scene = gamePlay;
			}
		}
#pragma endregion
		//毎フレーム処理
		//プレイヤー処理
#pragma region//ゲームプレイ中
		if (Scene == gamePlay) {
			if (MoveNumber == 0) {
				if (input->TriggerKey(DIK_DOWN) && HighNumber == 0) {
					audio->PlayWave("Resources/Sound/Decision.wav", 0.7f);
					initPositionY = PlayerPosition.y;
					initRotation = PlayerRotation.z;
					frame = 0;
					HighNumber = 1;
					MoveNumber = 1;
				}
				if (input->TriggerKey(DIK_UP) && HighNumber == 1) {
					audio->PlayWave("Resources/Sound/Decision.wav", 0.7f);
					initPositionY = PlayerPosition.y;
					initRotation = PlayerRotation.z;
					frame = 0;
					HighNumber = 0;
					MoveNumber = 2;
				}

				if (input->TriggerKey(DIK_LEFT) && LaneNumber >= 1) {
					audio->PlayWave("Resources/Sound/Decision.wav", 0.7f);
					initPositionX = PlayerPosition.x;
					initRotation = PlayerRotation.z;
					LaneNumber--;
					frame = 0;
					MoveNumber = 3;
				}
				if (input->TriggerKey(DIK_RIGHT) && LaneNumber <= 2) {
					audio->PlayWave("Resources/Sound/Decision.wav", 0.7f);
					initPositionX = PlayerPosition.x;
					initRotation = PlayerRotation.z;
					LaneNumber++;
					frame = 0;
					MoveNumber = 4;
				}
			}

			//	//imgui移動処理
			//	Input::MouseMove mouseMove = input->GetMouseMove();

			///*	if (input->PushMouseLeft()) {
			//		ImGui::SetNextWindowPos(ImVec2(mouseMove.lX, mouseMove.lY));
			//	}*/

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


			//ブロック生産
		/*	if (block->GetIsAlive() == 0) {
			}*/
			for (int i = 0; i < _countof(block); i++) {
				if (BlockPosition[i].z <= PlayerPosition.z - 50) {
					block[i]->SetIsAlive(0);
				}
				if (block[i]->Collide(player) == 1) {
					block[i]->SetIsAlive(0);
				}
				if (block[i]->GetIsAlive() == 0) {
					ResPornTimer[i]--;
					if (ResPornTimer[i] <= 0) {
						ResPornTimer[i] = 50;
						block[i]->Shot(player);
					}
				}
			}

		
			if (input->TriggerKey(DIK_R)) {
				Scene = gameClear;
			}

#pragma endregion
		}

		PlayerPosition.z += 1.25;

		//background->SetPosition(BackPosition);
		player->SetPosition(PlayerPosition);
		player->SetRotaition(PlayerRotation);

		//移動のやつ
		//カメラの注視点をプレイヤーの位置に固定
		target2.m128_f32[2] = PlayerPosition.z - 45;
		//カメラ追従用の処理
		target2.m128_f32[0] = background->GetPosition().x;
		target2.m128_f32[1] = 0;
		//行列を作り直す
		rotM = XMMatrixRotationX(XMConvertToRadians(angle));
		XMVECTOR v;
		v = XMVector3TransformNormal(v0, rotM);
		eye2 = target2 + v;
		matview = XMMatrixLookAtLH((eye2), (target2), XMLoadFloat3(&up));

		player->Update(matview);
		background->Update(matview);
		for (int i = 0; i < _countof(block); i++) {
			block[i]->Update(matview);
		}
		//ルートシグネチャの設定コマンド
#pragma region//クリア
		if (Scene == gameClear) {
			if (input->TriggerKey(DIK_S)) {
				Scene = title;
			}
		}
#pragma endregion
#pragma endregion
#pragma region//描画
		//びょうがこまんど
		//x,y座標のデバッグログ
		wchar_t str[256];
	
		swprintf_s(str, L"PlayerPosition.x:%f\n", PlayerPosition.x);
		OutputDebugString(str);
		swprintf_s(str, L"PlayerPosition.y:%f\n", PlayerPosition.y);
		OutputDebugString(str);
	
		dxCommon->PreDraw();
		////4.描画コマンドここから
		dxCommon->GetCmdList()->IASetPrimitiveTopology(D3D_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
		ImGui::Begin("test");
		if (ImGui::TreeNode("Debug"))
		{
			if (ImGui::TreeNode("Player"))
			{
				ImGui::Indent();
				ImGui::SliderFloat("Position.x", &PlayerPosition.x, 50, -50);
				ImGui::SliderFloat("Position.y", &PlayerPosition.y, 50, -50);
				ImGui::Unindent();
				ImGui::TreePop();
			}
			ImGui::TreePop();
		}
		ImGui::Indent();
		ImGui::Unindent();
		ImGui::End();
	
		//描画コマンド
		Player::PreDraw(dxCommon->GetCmdList());
		Block::PreDraw(dxCommon->GetCmdList());
		BackGround::PreDraw(dxCommon->GetCmdList());
		//背景
		if (Scene == gamePlay) {
			player->Draw();
			for (int i = 0; i < _countof(block); i++) {
				block[i]->Draw();
			}
		}
	
		background->Draw();

		Sprite::PreDraw(dxCommon->GetCmdList());
		if (Scene == title) {
			sprite[0]->Draw();
		}
		Sprite::PostDraw();
		Sprite::PreDraw(dxCommon->GetCmdList());
		if (Scene == gamePlay) {
			sprite[2]->Draw();
			sprite[2]->SetSize({ (float)(player->GetHp() * 25.8),24.0f });
		}

		Sprite::PostDraw();
		Sprite::PreDraw(dxCommon->GetCmdList());
		if (Scene == gameClear) {
			sprite[1]->Draw();
		
		}

		Sprite::PostDraw();
		////描画コマンド　ここまで
	
		Player::PostDraw();
		Block::PostDraw();
		BackGround::PostDraw();
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
	delete winApp;
	delete dxCommon;
	for (int i = 0; i < _countof(block); i++) {
		delete block[i];
	}
	delete player;
	delete background;
	winApp = nullptr;
	return 0;
#pragma endregion
}
#pragma endregion
