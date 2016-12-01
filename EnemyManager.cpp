//
//	@file	EnemyManager.cpp
//	@brief	エネミー管理クラス
//	@date	2016/11/13
//	@outher　吉越大騎

#include "EnemyManager.h"

EnemyManager::EnemyManager()
{
	charaType_ = Enemy;
	m_Pos = { 0,0,0 };
	aliveFlg_ = true;
	moveAbleFlg_ = true;
	opponentWeight_ = 1;
	//targetPos_ = D3DXVECTOR3(0, 0, 0);
	collision_ = new Collision;
	param_ = new EnemyParam;
}

EnemyManager::~EnemyManager()
{
	delete collision_;
	collision_ = nullptr;
	//delete motion_;
	//motion_ = nullptr;
}

//
//	@brief						Xファイル読み込み
//	@param (m_hWnd)				ウィンドウハンドル
//	@param (m_pDevice)			デバイス
//	@param (m_pDeviceContext)	デバイスコンテキスト
//	@param (fileName)			読み込むキャラ名
const char* EnemyManager::CharaInit(const char* fileName)
{
	char FileName[80] = { 0 };
	memset(FileName, 0, sizeof(FileName));
	strcpy_s(FileName, sizeof(FileName), "./Model/XFiles/Enemy/");
	strcat_s(FileName, sizeof(FileName), fileName);

	return FileName;
	//CreateFromX(FileName);
	//m_Scale = D3DXVECTOR3(0.2, 0.2, 0.2);
	//m_Pos = D3DXVECTOR3(0, 0, 0);
}

//
//	@brief	パラメータセット
void EnemyManager::SetParameter(EnemyParameter* param)
{
	memset(param_->name_, 0, sizeof(param_->name_));
	memcpy(param_->name_, param->GetName(), sizeof(param_->name_));

	param_->hp_ = param->GetHP();
	param_->atk_ = param->GetAtk();
	param_->def_ = param->GetDefence();
	param_->moveSpeed_ = param->GetMoveSpeed();
	param_->weight_ = param->GetWeight();
	param_->attackReach_ = param->GetAttackReach();
	param_->scale_ = param->GetScale();

	ownWright_ = param_->weight_;
	hp_ = param_->hp_;
	//hp_ = 10000;
}

//
//	@brief　			ターゲット設定
//	@param (chara)		ターゲットにするキャラ
void EnemyManager::SetTarget(CharactorManager* chara)
{
	targetChar_ = chara;
	targetPos_ = targetChar_->m_Pos;
}

//
//	@brief　				ターゲットの更新
//	@param (chara)			プレイヤー
//	@param (princess)		姫
void EnemyManager::Target_Update(CharactorManager * chara, CharactorManager * princess)
{
	CharactorManager* temp = chara;
	float dist = 5;

	if (targetChar_->GetCharaType() != temp->GetCharaType())
	{
		temp = targetChar_;
	}

	//現在のターゲットが姫
	if (targetChar_->GetCharaType() == princess->GetCharaType())
	{
		if (collision_->CharaNear(m_Pos, chara->m_Pos, dist))
		{
			temp = chara;
		}
	}

	//ターゲットが死んでいる
	if (!temp->GetAliveFlg())
	{
		temp = princess;
	}

	//ターゲットの更新
	SetTarget(temp);
}

//
//	@brief　			ターゲットポジションの更新
//	@param (position)	座標
void EnemyManager::SetTargetChar(CharactorManager* checkChar, CharactorManager* princess)
{
	////現在のターゲットとチェックするプレイヤーが一致するか
	//if (targetChar_->GetCharaType() == checkChar->GetCharaType())
	//{
	//	//チェックする(現在ターゲットのプレイヤー）が生存しているか
	//	if (checkChar->GetAliveFlg())
	//	{
	//		//ターゲット更新
	//		targetChar_ = checkChar;
	//		targetPos_ = targetChar_->m_Pos;
	//	}
	//	else
	//	{
	//		//ターゲットを姫に変更
	//		targetChar_ = princess;
	//		targetPos_ = targetChar_->m_Pos;
	//	}
	//}
	//else if (targetChar_->GetCharaType() == princess->GetCharaType()/* || targetChar_ == nullptr*/)       //現在のターゲットが姫
	//{
	//	//近くに生きているプレイヤーがいるかどうか(チェックするプレイヤーが生きている 且つ 距離が一定以内)
	//	if (checkChar->GetAliveFlg() && collision_->CharaNear(m_Pos, checkChar->m_Pos, 50.0))
	//	{
	//		//ターゲットをプレイヤーに変更
	//		targetChar_ = checkChar;
	//		targetPos_ = targetChar_->m_Pos;
	//	}
	//	else         //近くに生きているプレイヤーがいない
	//	{
	//		//ターゲット更新
	//		targetChar_ = princess;
	//		targetPos_ = targetChar_->m_Pos;
	//	}
	//}

	////if (!targetChar_->GetAliveFlg())
	////{
	////	targetChar_ = princess;
	////	targetPos_ = targetChar_->m_Pos;
	////}
}

//
//	@brief			移動処理
//	@param (speed)	移動速度
void EnemyManager::Move(float speed)
{
	D3DXVECTOR3 E_Lock;
	E_Lock.x = targetPos_.x - m_Pos.x;
	E_Lock.z = targetPos_.z - m_Pos.z;

	D3DXVec3Normalize(&E_Lock, &E_Lock);

	//回転処理
	Rotation(E_Lock);

	//向いている角度から単位ベクトルを取得
	D3DXVECTOR3 vec = D3DXVECTOR3(sinf(m_Yaw)*-1, 0, cosf(m_Yaw)*-1);
	//D3DXVECTOR3 vec = E_Lock;
	float sp = speed;
	//opponentWeight_ = 1;
	m_Dir = D3DXVECTOR3(vec.x*sp*opponentWeight_, 0, vec.z*sp*opponentWeight_);

	/*if (motionNo_ != motion_->GetMotion("walk")->id_)
	{
		motionNo_ = motion_->GetMotion("walk")->id_;
		m_pD3dxMesh->ChangeAnimSet(motionNo_);
		motionSpeed_ = 1 / (float)motion_->GetMotion("walk")->frame_;
	}*/

}

//
//	@brief	ダメージ計算
void EnemyManager::DamageCalc(unsigned int atk)
{

	float damage = atk / (1 + ((float)param_->def_ / 100));

	if (hp_ <= damage)
	{
		hp_ = 0;
		aliveFlg_ = false;
	}
	else
	{
		hp_ -= damage;
	}
}

//
//	@brief	死亡処理
void EnemyManager::Dead()
{
}

//
//	@brief	敵通常攻撃
void EnemyManager::Attack()
{
	float atkableDist = 2;//param_->attackReach_;
	int time = 3;

	if (collision_->CharaNear(m_Pos, targetPos_, atkableDist))
	{
		if (++atkWaitTime_ % (FPS*time) == 0)
		{
			moveAbleFlg_ = false;
			NormalAttack();
		}
		else
		{
			moveAbleFlg_ = true;
		}
	}
	else
	{
		moveAbleFlg_ = true;
		atkWaitTime_ = 0;
	}
}
