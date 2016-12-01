//
//	@file	JobManager.h
//	@brief	敵ごとの処理
//	@date	2016/11/13
//	@outher	吉越大騎

#pragma once
#include "./../Enemy/EnemyManager.h"


//	職業管理クラス
class EnemyJobManager :public EnemyManager
{
protected:

public:
	EnemyJobManager();
	virtual ~EnemyJobManager();
	virtual void Reset();
	virtual void CharaUpdate();
};

//	スライムクラス
class Slim :public EnemyJobManager
{
private:
	virtual void NormalAttack();
public:
	//Slim() {};
	virtual ~Slim() {};
};

//	ゴブリンクラス
class Goblin :public EnemyJobManager
{
private:
	virtual void NormalAttack();
public:
	virtual ~Goblin() {};
};


#include "./../Player/Job/WeaponBall.h"
//	スケルトンクラス
class Skeleton :public EnemyJobManager
{
private:
	virtual void NormalAttack();
	std::vector<WeaponBall*> atkList_;
public:
	virtual ~Skeleton() {};
	virtual void Reset();
	virtual void CharaRender();		//描画

};