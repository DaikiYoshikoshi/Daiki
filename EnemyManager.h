//
//	@file	EnemyManager.h
//	@brief	敵管理クラス
//	@date	2016/11/13
//	@outher	吉越大騎

#pragma once
#include "./../CharactorManager.h"
#include "./../../DebugDraw/D3D11_TEXT.h"
#include "./../../Read/ParameterRead.h"

//	パラメータ
struct EnemyParam
{
	char name_[30];				//敵の名
	unsigned int hp_;			//HP
	unsigned int atk_;			//通常攻撃力
	unsigned int def_;			//守備力
	unsigned int waitTime_;		//待機時間
	float moveSpeed_;			//移動速度
	float attackReach_;			//攻撃距離
	float weight_;				//重さ
	float scale_;				//サイズ
};

class EnemyManager : public CharactorManager
{
private:
	D3DXVECTOR3 targetPos_;
	enum TargetObject
	{
		princess = 0,
		player,
	};
	TargetObject targetObj_;
	
protected:
	unsigned int atkWaitTime_;	//攻撃する間隔

	EnemyParam* param_;
	CharactorManager* targetChar_;
	D3DXVECTOR3 getpos_;
	virtual void Move(float speed);		//移動処理
	virtual void Attack();
	virtual void NormalAttack() = 0;
	virtual void Dead();				//死亡
	virtual void DamageCalc(unsigned int atk);
	virtual void DamageSound() {};		//ダメージサウンド

public:
	EnemyManager() ;
	virtual ~EnemyManager();
	
	virtual const char* CharaInit(const char* fileName);		//初期化・読み込み
	void SetParameter(EnemyParameter* param);
	void SetTarget(CharactorManager* chara);
	void Target_Update(CharactorManager* chara, CharactorManager* princess);

	void SetTargetChar(CharactorManager* checkChar, CharactorManager* princess);	//近くにいるか判別
	
	D3DXVECTOR3 GetTargetPos() { return targetPos_; };	//ターゲットポジション取得デバック用
	EnemyParam* GetParam() { return param_; };
	CharactorManager* GetTarget() { return targetChar_; };
};

