#include "./SwordMan.h"


SwordMan::SwordMan(CharaType charaType) :JobManager(charaType)
{
	charaType_ = charaType;
}

SwordMan::~SwordMan()
{

}

//
//	@brief	リセット

//
//	@brief	リセット
void SwordMan::Reset()
{
	hp_ = param_->hp_;
	motionCount_ = 0;
	motionChange_ = true;
	aliveFlg_ = true;
	moveAbleFlg_ = true;
	aroundCharaList_.clear();
	allCharaList_.clear();
	atkNo_ = noAtk;
	revivalFlg_ = false;
	callTiming_ = 0;
	attackCount_ = 0;

	m_Pos = D3DXVECTOR3(-2.25 + charaType_*1.5, 0, -10);
}


//
//	@brief	攻撃
void SwordMan::Attack()
{
	if (atkNo_ == noAtk)
	{
		moveAbleFlg_ = true;
	}
	else
	{
		moveAbleFlg_ = false;
	}

	if (GamePad::checkInput(charaType_,GamePad::InputName::A)
		/*|| GetKeyState('1') & 0x80*/)
	{
		++attackCount_;
		atkNo_ = waitAtk;
	}
	else if (atkNo_ == normalAtk)
	{
		attackCount_ = 0;
		//motionCount_ = 0;
		//atkNo_ = noAtk;
		//Normal_Attack();
		//hit = false;
		//atkNo_ = noAtk;
		//Special_Attack();
	}
	else if (atkNo_ == charge)
	{
		attackCount_ = 0;
		motionCount_ = 0;
		motionChange_ = true;
		atkNo_ = specialAtk;
		hit = false;
	}
	//unsigned int inputTime = playerParam_.chargeTime_;

	/* 修正の必要あり */
	unsigned int inputTime = (FPS*param_->chargeTime_)/2;

	if (0 < attackCount_ && attackCount_ <= inputTime)
	{
		atkNo_ = normalAtk;
	}
	else if (inputTime < attackCount_)
	{
		atkNo_ = charge;
		if (/*motionChange_ == true && */motionNo_ != motion_->GetMotion("charge")->id_)
		{
			motionChange_ = false;
			//motionNo_ = motion_->GetMotion("charge")->id_;
			//m_pD3dxMesh->ChangeAnimSet(motionNo_);
			//timeEnd_ = motion_->GetMotion("charge")->frame_;
			//motionSpeed_ = 1 / (float)timeEnd_;
			ChangeMotion(motion_, "charge");
			motionSpeed_ *= 5;
			motionFrame_ /= 2;
		}
	}

	if (atkNo_ == normalAtk)
	{
		Normal_Attack();
	}
	else if (atkNo_ == specialAtk)
	{
		Special_Attack();
	}
}

//
//	@breif	通常攻撃
void SwordMan::Normal_Attack()
{
	Normal_Attack_Collision();
	if (motionChange_==true && motionNo_ != motion_->GetMotion("attack1")->id_)
	{
		Sound::getInstance().SE_play("S_NORMALATK");

		motionChange_ = false;
		//motionNo_ = motion_->GetMotion("attack1")->id_;
		//m_pD3dxMesh->ChangeAnimSet(motionNo_);
		//timeEnd_ = motion_->GetMotion("attack1")->frame_;
		//motionSpeed_ = 1 / (float)timeEnd_;
		ChangeMotion(motion_, "attack1");

		/*モーション自体が遅い為丁度良い感じに修正
		　修正の必要があり。　　　　　　　　　　　　　　　　　　　　　　　　　　　　　　　　　　　　　　　　　　　　　　　　　　　　　　　　　　　　　　　　　　　　　　　　　　　　　　　　　　　　　　　　　　　　　　　　　　　　　　　　　　　　　　　　　　　　　　　　　　　　　　　　　　　　　　　　　　　　　　　　　　　　　　　　　　　　　　　　　　　　　　　　　　　　　　　　　　　　　　　　　　　　　　　　　　　　　　　　　　　　　　　　　　　　　　　　　　　　　　　　　　　　　　　　　　　　　　　　　　　　　　　　　　　　　　　　　　　　　　　　　　　　　　　　　　　　　　　　　　　　　　　　　　　　　　　　　　　　　　　　　　　　　　　　　　　　　　　　　　　　　　　　　　　　　　　　　　　　　　　　　　　　　　　　　　　　　　　　　　　　　　　　　　　　　　　　　　　　　　　　　　　　　　　　　　　　　　　　　　　　　　　　　　　　　　　　　　　　　　　　　　　　　　　　　　　　　　　　　　　　　　　　　　　　　　　　　　　　　　　　　　　　　　　　　　　　　　　　　　　　　　　　　　　　　　　　　　　　　　　　　　　　　　　　　　　　　　　　　　　　　　　　　　　　　　　　　　　　　　　　　　　　　　　　　　　　　　　　　　　　　　　　　　　　　　　　　　　　　　　　　　　　　　　　　　　　　　　　　　　　　　　　　　　　　　　　　　　　　　　　　　　　　　　　　　　　　　　　　　　　　　　　　　　　　　　　　　　　　　　　　　　　　　　　　　　　　　　　　　　　　　　　　　　　　　　　　　　　　　　　　　　　　　　　　　　　　　　　　　　　　　　　　　　　　　　　　　　　　　　　　　　　　　　　　　　　　　　　　　　　　　　　　　　　　　　　　　　　　　　　　　　　　　　　　　　　　　　　　　　　　　　　　　　　　　　　　　　　　　　　　　　　　　　　　　　　　　　　　　　　　　　　　　　　　　　　　　　　　　　　　　　　　　　　　　　　　　　　　　　　　　　　　　　　　　　　　　　　　　　　　　　　　　　　　　　　　　　　　　　　　　　　　　　　　　　　　　　　　　　　　　　　　　　　　　　　　　　　　　　　　　　　　　　　　　　　　　　　　　　　　　　　　　　　　　　　　　　　　　　　　　　　　　　　　　　　　　　　　　　　　　　　　　　　　　　　　　　　　　　　　　　　　　　　　　　　　　　　　　　　　　　　　　　　　　　　　　　　　　　　　　　　　　　　　　　　　　　　　　　　　　　　　　　　　　　　　　　　　　　　　　　　　　　　　　　　　　　　　　　　　　　　　　　　　　　　　　　　　　　　　　　　　　　　　　　　　　　　　　　　　　　　　　　　　　　　　　　　　　　　　　　　　　　　　　　　　　　　　　　　　　　　　　　　　　　　　　　　　　　　　　　　　　　　　　　　　　　　　　　　　　　　　　　　　　　　　　　　　　　　　　　　　　　　　　　　　　　　　　　　　　　　　　　　　　　　　　　　　　　　　　　　　　　　　　　　　　　　　　　　　　　　　　　　　　　　　　　　　　　　　　　　　　　　　　　　　　　　　　　　　　　　　　　　　　　　　　　　　　　　　　　　　　　　　　　　　　　　　　　　　　　　　　　　　　　　　　　　　　　　　　　　　　　　　　　　　　　　　　　　　　　　　　　　　　　　　　　　　　　　　　　　　　　　　　　　　　　　　　　　　　　　　　　　　　　　　　　　　　　　　　　　　　　　　　　　　　　　　　　　　　　　　　　　　　　　　　　　　　　　　　　　　　　　　　　　　　　　　　　　　　　　　　　　　　　　　　　　　　　　　　　　　　　　　　　　　　　　　　　　　　　　　　　　　　　　　　　　　　　　　　　　　　　　　　　　　　　　　　　　　　　　　　　　　　　　　　　　　　　　　　　　　　　　　　　　　　　　　　　　　　　　　　　　　　　　　　　　　　　　　　　　　　　　　　　　　　　　　　　　　　　　　　　　　　　　　　　　　　　　　　　　　　　　　　　　　　　　　　　　　　　　　　　　　　　　　　　　　　　　　　　　　　　　　　　　　　　　　　　　　　　　　　　　　　　　　　　　　　　　　　　　　　　　　　　　　　　　　　　　　　　　　　　　　　　　　　　　　　　　　　　　　　　　　　　　　　　　　　　　　　　　　　　　　　　　　　　　　　　　　　　　　　　　　　　　　　　　　　　　　　　　　　　　　　　　　　　　　　　　　　　　　　　　　　　　　　　　　　　　　　　　　　　　　　　　　　　　　　　　　　　　　　　　　　　　　　　　　　　　　　　　　　　　　　　　　　　　　　　　　　　　　　　　　　　　　　　　　　　　　　　　　　　　　　　　　　　　　　　　　　　　　　　　　　　　　　　　　　　　　　　　　　　　　　　　　　　　　　　　　　　　　　　　　　　　　　　　　　　　　　　　　　　　　　　　　　　　　　　　　　　　　　　　　　　　　　　　　　　　　　　　　　　　　　　　*/
		motionSpeed_ *= 10;
		motionFrame_ /= 5;
	}

	if (++motionCount_ > motionFrame_)
	{
		atkNo_ = noAtk;
		//attackCount_ = 0;
		motionCount_ = 0;
		hit = false;
		motionChange_ = true;
	}
}

//
//	@brief	通常攻撃当たり判定
void SwordMan::Normal_Attack_Collision()
{
	float degree = D3DXToDegree(m_Yaw);
	float atkDist = param_->attackReach_;
	float hitAngle = param_->attackRange_;
	float backDist = param_->knockbackDist_;
	float speed = param_->knockbackSpeed_;
	if (!aroundCharaList_.empty())
	{
		for (auto chara : aroundCharaList_)
		{
			if (collision_->CharaNear(m_Pos, chara->m_Pos, atkDist))
			{
				D3DXVECTOR3 vec = chara->m_Pos - m_Pos;
				D3DXVec3Normalize(&vec, &vec);
				float angle = (atan2(vec.z, vec.x)*-1) - (D3DX_PI / 2.0f);
				angle = D3DXToDegree(angle);


				if (fabsf(degree - angle) <= hitAngle)
				{
					hit = true;
					chara->SetKnockBack(m_Pos, backDist,speed);
					if (chara->GetCharaType() == Enemy)
					{
						//敵にダメージが入った時のSE
						Sound::getInstance().SE_play("S_DAMAGE_HIT");
						chara->DamageCalc(param_->normalAtk_);
					}
				}
				else
				{
					hit = false;
				}
			}
		}
	}
}

void SwordMan::DamageSound()
{
	Sound::getInstance().SE_play("S_DAMAGE");
}

//
//	@brief	特殊攻撃
void SwordMan::Special_Attack()
{
	Special_Attack_Collision();

	if (motionChange_ == true && motionNo_ != motion_->GetMotion("special")->id_)
	{
		Sound::getInstance().SE_play("S_SPECIAL");

		motionChange_ = false;
		//motionNo_ = motion_->GetMotion("special")->id_;
		//m_pD3dxMesh->ChangeAnimSet(motionNo_);
		//timeEnd_ = motion_->GetMotion("special")->frame_;
		//motionSpeed_ = 1 / (float)timeEnd_;
		ChangeMotion(motion_, "special");
		motionSpeed_ *= 5;
		motionFrame_ /= 2;
	}

	if (++motionCount_ > motionFrame_)
	{
		atkNo_ = noAtk;
		//attackCount_ = 0;
		motionCount_ = 0;
		hit = false;
		motionChange_ = true;
	}
}

//
//	@brief	死亡音再生
void SwordMan::DeadSound()
{
	Sound::getInstance().SE_play("S_DEAD");
}

//
//	@brief	特殊攻撃当たり判定
void SwordMan::Special_Attack_Collision()
{
	float atkRange = param_->attackRange_;
	float atkDist = param_->attackReach_;
	float backDist = param_->knockbackDist_;
	float backSpeed = param_->knockbackSpeed_;
	if (!aroundCharaList_.empty())
	{
		for (auto chara : aroundCharaList_)
		{
			if (collision_->CharaNear(m_Pos, chara->m_Pos, atkDist))
			{
				hit = true;
				chara->SetKnockBack(m_Pos, backDist,backSpeed);
				if (chara->GetCharaType() == Enemy)
				{
					//敵にダメージが入った時のSE
					Sound::getInstance().SE_play("S_DAMAGE_HIT");
					chara->DamageCalc(param_->specialAtk_);
				}
			}
		}
	}
}