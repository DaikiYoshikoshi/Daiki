//
//	@file	PlayerManager.h
//	@brief	プレイヤー管理クラス
//	@date	2016/11/09
//	@outher	仁科香苗
#pragma once
#include <time.h>
#include "./../CharactorManager.h"
#include "./GamePad.h"

//	パラメータ
/* 訂正の必要あり？ */
struct PlayerParameter
{
	char name_[30];							//職業名
	unsigned int hp_;					//HP
	unsigned int normalAtk_;			//通常攻撃力
	unsigned int specialAtk_;			//特殊攻撃力
	unsigned int def_;					//守備力
	unsigned int chainWeapon_;			//連続飛び道具設置個数
	unsigned int spChainWeapon_;		//特殊時連続飛び道具設置個数
	unsigned int specialAttackTime_;	//特殊攻撃時間
	unsigned int invisibleTime_;		//無敵時間
	unsigned int weaponDelTime_;		//飛び道具消滅時間
	unsigned int chargeTime_;			//チャージタイム
	float moveSpeed_;					//移動速度
	float specialMoveSpeed_;			//特殊移動速度
	float weight_;						//重さ
	float attackReach_;					//攻撃リーチ
	float attackRange_;					//攻撃範囲
	float specialAtkRange_;				//特殊攻撃時範囲
	float scale_;						//スケール
	float knockbackSpeed_;				//ノックバックさせるスピード
	float knockbackDist_;				//ノックバックする距離
	float weaponScale_;					//飛び道具サイズ
	float weaponHitReach_;				//飛び道具衝突範囲
	float weaponDelDist_;				//飛び道具消滅距離
};

class PlayerManager : public CharactorManager
{
private:
protected:
	double callTiming_;		//姫を呼んだクロック時間


	//攻撃状態番号
	enum AttackState
	{
		noAtk = 0,	//攻撃していない
		waitAtk,	//攻撃待機
		normalAtk,	//通常攻撃
		charge,		//チャージ
		specialAtk,	//特殊攻撃
	};

	AttackState atkNo_;		//攻撃状態番号
	PlayerParameter* param_;

	bool revivalFlg_;		//復活フラグ
	D3DXVECTOR3 callPos_;	//読んだ瞬間の座標

	virtual void Normal_Attack() = 0;	//通常攻撃
	virtual void Special_Attack() = 0;	//特殊攻撃
	//virtual void Attack();			//攻撃
	virtual void Move(float speed);		//移動処理
	virtual void Dead();				//死亡
	void Revival();						//復活
	virtual void DeadSound() {};			//死亡時サウンド

public:
	PlayerManager();
	virtual ~PlayerManager() {};

	virtual const char* CharaInit(const char* fileName);	//初期化と読み込み
	virtual void DamageCalc(unsigned int atk);		//ダメージ計算
	void Princess_Call();							//姫呼び
	void SetRevivalFlg();
	bool GetAliveFlg()const;						//生存フラグ取得
	double GetCallTiming()const;
	D3DXVECTOR3 GetCallPos()const;					//呼んだ瞬間の座標取得

	//デバッグ用
	//int GetAtkCnt() { return attackCount_; };
	AttackState GetAtkState() { return atkNo_; };
	float GetYaw() { return m_Yaw; };
};