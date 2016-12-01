//
//	@file	JobManager.h
//	@brief	敵ごとの処理
//	@date	2016/11/13
//	@outher	吉越大騎


#include "EnemyJobManager.h"

EnemyJobManager::EnemyJobManager()
{
}

EnemyJobManager::~EnemyJobManager()
{
}

//
//	@brief	リセット
void EnemyJobManager::Reset()
{
	hp_ = param_->hp_;
	motionChange_ = true;
	aliveFlg_ = true;
	moveAbleFlg_ = true;
	aroundCharaList_.clear();
	allCharaList_.clear();

}

void EnemyJobManager::CharaUpdate()
{
	//m_pD3dxMesh->m_pAnimController->AdvanceTime(motionSpeed_, NULL);

	//空間番号の更新
	spaceNo_ = collision_->SetSpaceNo(m_Pos);

	float speed = param_->moveSpeed_;
	MoveCharaHit();
	Move(speed);

	Attack();
}

//=======スライム=======//
//Slim::Slim()
//{
//}

void Slim::NormalAttack()
{
	float dist = 3;
	float speed = 1;
	targetChar_->DamageCalc(param_->atk_);
	targetChar_->KnockBack(m_Pos,dist,speed);
}

//=======ゴブリン========//
void Goblin::NormalAttack()
{
	float dist = 3;
	float speed = 1;
	targetChar_->DamageCalc(param_->atk_);
	targetChar_->KnockBack(m_Pos, dist, speed);
}

//=======スケルトン======//


//
//	@brief	リセット
void Skeleton::Reset()
{
	hp_ = param_->hp_;
	motionChange_ = true;
	aliveFlg_ = true;
	moveAbleFlg_ = true;
	aroundCharaList_.clear();
	allCharaList_.clear();
	atkList_.clear();
}

void Skeleton::NormalAttack()
{
	float dist = 1;
	float speed = 1;
	WeaponBall* atkBall=new WeaponBall();
	D3DXVECTOR3 vec(sinf(m_Yaw)*-0.1, 0, cosf(m_Yaw)*-0.1);
	atkBall->SetDir(vec);
	atkBall->SetDamageList(allCharaList_, charaType_);
	atkBall->SetKnockBack(dist, param_->attackReach_,speed);
	atkBall->SetAttack(param_->atk_);
	atkList_.push_back(atkBall);
}


void Skeleton::CharaRender()
{
	Render(m_Pos);

	if (!atkList_.empty())
	{
		for (auto a : atkList_)
		{
			//a->Render();
		}
	}
}