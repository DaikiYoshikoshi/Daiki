//
//	@file	Main_Scene.h
//	@brief	メインゲームシーン管理クラス
//	@date	2016/11/08
//	@outher	仁科香苗
#pragma once
#include <vector>
#include <time.h>
#include "./Origin.h"
#include "./Read/XFileRead.h"
#include "./Read/ParameterRead.h"
#include "./Read/MotionRead.h"
#include "./Stage/Stage.h"
#include "./Charactor/Player/PlayerCommon.h"
#include "./Charactor/CharactorCreator.h"
#include "./Charactor/Enemy/EnemyJobManager.h"
#include "./Stage/SpawnManager.h"
#include "./Charactor/Princess.h"
#include "./Collision.h"
#include "./Camera.h"
#include "./SoundEffect/Sound.h"

class Main_Scene
{
	enum scene
	{
		StartS=0,
		MainS,
		EndS,
	};
	scene scene_;

	XFileRead* xfileRead;
	XFile* xfile;
	ParameterRead* parameter;
	MotionRead* motionRead_;

	HWND wnd_;
	ID3D11Device* device_;
	ID3D11DeviceContext* deviceContext_;

	Stage* stage_;						//ステージ情報
	Collision* ray_;
	std::vector<JobManager*> virChar_;	//確認用仮キャラクター
	Princess* princess_;				//姫
	Camera* camera_;

	int spawnAmount_;					//スポーンゲートの数
	//Spawn* spawn_;
	SpawnManager* spawnManager_;
	EnemyJobManager* virEnemy_;			//確用仮キャラクター
	CD3DXSKINMESH* slime_;
	/*EnemyJobManager* virEnemy_;*/

	void CollisionControl();			//衝突判定管理

	std::vector<CharactorManager*> charList_;	//ステージ上に存在するキャラクター
	std::vector<EnemyJobManager*> enemyList_;	//ステージ上に存在するエネミー
	std::vector<EnemyJobManager*> killList_;	//死亡エネミーリスト
	unsigned int time_;

	void GameStart();	//ゲーム開始導入部分
	void GameMain();	//ゲームメイン
	void GameEnd();		//ゲーム終了

	void EnemyDestroy();	//エネミー死亡処理

							//エフェクト	
	D3D11_SPRITE* uisword_;
	D3D11_SPRITE* uiseeld_;
	D3D11_SPRITE* uimagic_;
	D3D11_SPRITE* uibom_;
	D3D11_SPRITE* uititle_;


	//デバック
	D3D11_TEXT* debugText_;
	double insTime_,pushTime_;
	bool spawnFlg_;

	void PlayerDebug();
	void EnemyDebug();
public:
	Main_Scene();
	~Main_Scene();

	void Init(HWND m_hWnd,ID3D11Device* m_pDevice,ID3D11DeviceContext* m_pDeviceContext);	//初期化
	
	HRESULT DebugInit(ID3D11DeviceContext* m_pDeviceContext);								//デバッグ描画初期化
	HRESULT EffectInit(ID3D11DeviceContext* m_pDeviceContext);
	void Update();																			//更新
	/*void Render(D3DXMATRIX mView, D3DXMATRIX mProj);*/										//描画
	void Render();
	void EffectRender();	//描画
};

