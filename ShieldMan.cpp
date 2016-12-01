#include "ShieldMan.h"

ShieldMan::ShieldMan(CharaType charaType) :JobManager(charaType)
{
	charaType_ = charaType;
	spMoveFlg_ = false;
}

ShieldMan::~ShieldMan()
{

}

//
//	@brief	ƒŠƒZƒbƒg
void ShieldMan::Reset()
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
	spMoveFlg_ = false;

	m_Pos = D3DXVECTOR3(-2.25 + charaType_*1.5, 0, -10);
}

//
//	@brief	UŒ‚
void ShieldMan::Attack()
{
	if (GamePad::checkInput(charaType_, GamePad::InputName::A)
		/*|| GetKeyState('1') & 0x80*/)
	{
		++attackCount_;
	}
	else if (atkNo_ == normalAtk)
	{
		attackCount_ = 0;
		//atkNo_ = noAtk;
		Normal_Attack();
		//hit = false;
		//atkNo_ = noAtk;
		//Special_Attack();
	}
	else
	{
		spMoveFlg_ = false;
		attackCount_ = 0;
		atkNo_ = noAtk;
		motionChange_ = true;
		hit = false;
		spMove_ = D3DXVECTOR3(1, 0, 1);
	}
	//unsigned int inputTime = playerParam_.chargeTime_;

	unsigned int inputTime = FPS*param_->chargeTime_;
	unsigned int tempFrame = motion_->GetMotion("special")->frame_;

	if (0 < attackCount_ && attackCount_ <= inputTime)
	{
		atkNo_ = normalAtk;
	}
	else if (inputTime < attackCount_ && attackCount_ - inputTime < tempFrame)
	{
		atkNo_ = specialAtk;
	}
	else
	{
		spMoveFlg_ = true;
	}

	if (atkNo_ == specialAtk)
	{
		Special_Attack();
	}
}

//
//	@brief	’ÊíUŒ‚
void ShieldMan::Normal_Attack()
{
	float hitAngle = param_->attackRange_;
	Attack_Collision(hitAngle);

	//Normal_Attack_Collision();
	if (/*motionChange_ == true && */motionNo_ != motion_->GetMotion("attack1")->id_)
	{
		Sound::getInstance().SE_play("Sh_NORMALATK");

		motionChange_ = false;
		//motionNo_ = motion_->GetMotion("attack1")->id_;
		//m_pD3dxMesh->ChangeAnimSet(motionNo_);
		//timeEnd_ = motion_->GetMotion("attack1")->frame_;
		//motionSpeed_ = 1 / (float)timeEnd_;
		//motionCount_ = 0;
		ChangeMotion(motion_, "attack1");
	}

	if (++motionCount_ > motionFrame_)
	{
		atkNo_ = noAtk;
		//attackCount_ = 0;
		motionChange_ = true;
		motionCount_ = 0;
		hit = false;
	}
}

////
////	@brief	’ÊíUŒ‚“–‚½‚è”»’è
//void ShieldMan::Normal_Attack_Collision()
//{
//	float degree = D3DXToDegree(m_Yaw);
//	float hitAngle = param_->attackRange_;
//	float atkDist = param_->attackReach_;
//	float backDist = param_->knockbackDist_;
//	float backSpeed = param_->knockbackSpeed_;
//
//	if (!aroundCharaList_.empty())
//	{
//		for (auto chara : aroundCharaList_)
//		{
//			if (collision_->CharaNear(m_Pos, chara->m_Pos, atkDist))
//			{
//				D3DXVECTOR3 vec = chara->m_Pos - m_Pos;
//				float angle = (atan2(vec.z, vec.x)*-1) - (D3DX_PI / 2.0f);
//				angle = D3DXToDegree(angle);
//
//				if (fabsf(degree - angle) <= hitAngle)
//				{
//					hit = true;
//					chara->SetKnockBack(m_Pos, backDist, backSpeed);
//					if (chara->GetCharaType() == Enemy)
//					{
//						chara->DamageCalc(param_->normalAtk_);
//					}
//				}
//				else
//				{
//					hit = false;
//				}
//			}
//		}
//	}
//}

//
//	@brief	“ÁŽêUŒ‚
void ShieldMan::Special_Attack()
{
	//spMove_ = D3DXVECTOR3(param_->specialMoveSpeed_, param_->specialMoveSpeed_, param_->specialMoveSpeed_);
	//Special_Attack_Collision();
	float hitAngle = param_->specialAtkRange_;
	Attack_Collision(hitAngle);

	if (spMoveFlg_ == false)
	{
		if (motionNo_ != motion_->GetMotion("special")->id_)
		{
			motionChange_ = false;
			//motionNo_ = motion_->GetMotion("special")->id_;
			//m_pD3dxMesh->ChangeAnimSet(motionNo_);
			//motionSpeed_ = 1 / (float)motion_->GetMotion("special")->frame_;
			ChangeMotion(motion_, "special");
		}
	}
	//if (/*motionChange_ == true && */motionNo_ != motion_->GetMotion("special")->id_)
	//{
	//	motionChange_ = false;
	//	motionNo_ = motion_->GetMotion("special")->id_;
	//	m_pD3dxMesh->ChangeAnimSet(motionNo_);
	//	timeEnd_ = motion_->GetMotion("special")->frame_;
	//	motionSpeed_ = 1 / (float)timeEnd_;
	//	motionCount_ = 0;
	//}

	//if (++motionCount_ > timeEnd_)
	//{
	//	atkNo_ = noAtk;
	//	//attackCount_ = 0;
	//	motionChange_ = true;
	//	motionCount_ = 0;
	//	hit = false;
	//}

}

//
//	@brief	UŒ‚‚Ì“–‚½‚è”»’è
void ShieldMan::Attack_Collision(float hitAngle)
{
	float degree = D3DXToDegree(m_Yaw);
	//float hitAngle = param_->attackRange_;
	float atkDist = param_->attackReach_;
	float backSpeed = param_->knockbackSpeed_;


	float backDist = 0;
	if (atkNo_ == normalAtk)
	{
		backDist = param_->knockbackDist_;
	}



	if (!aroundCharaList_.empty())
	{
		for (auto chara : aroundCharaList_)
		{
			if (collision_->CharaNear(m_Pos, chara->m_Pos, atkDist))
			{
				D3DXVECTOR3 vec = chara->m_Pos - m_Pos;
				float angle = (atan2(vec.z, vec.x)*-1) - (D3DX_PI / 2.0f);
				angle = D3DXToDegree(angle);

				//float tempAngle = 360;
				if (fabsf(degree - angle) <= hitAngle)
				{
					hit = true;
					chara->SetKnockBack(m_Pos, backDist, backSpeed);
					if (chara->GetCharaType() == Enemy && atkNo_ == normalAtk)
					{
						chara->DamageCalc(param_->normalAtk_);

						//“G‚Éƒ_ƒ[ƒW‚ª“ü‚Á‚½Žž‚ÌSE
						Sound::getInstance().SE_play("Sh_DAMAGE_HIT");
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

//
////
////	@brief	“ÁŽêUŒ‚“–‚½‚è”»’è
//void ShieldMan::Special_Attack_Collision()
//{
//	float degree = D3DXToDegree(m_Yaw);
//	float hitAngle = param_->attackRange_;
//	float atkDist = param_->attackReach_;
//	float backDist = param_->knockbackDist_;
//
//	if (!aroundCharaList_.empty())
//	{
//		for (auto chara : aroundCharaList_)
//		{
//			if (collision_->CharaNear(m_Pos, chara->m_Pos, atkDist))
//			{
//				D3DXVECTOR3 vec = chara->m_Pos - m_Pos;
//				D3DXVec3Normalize(&vec, &vec);
//				float angle = (atan2(vec.z, vec.x)*-1) - (D3DX_PI / 2.0f);
//				angle = D3DXToDegree(angle);
//
//				if (fabsf(degree - angle) <= hitAngle)
//				{
//					hit = true;
//					chara->SetKnockBack(m_Pos, backDist, backSpeed);
//					if (chara->GetCharaType() == Enemy)
//					{
//						chara->DamageCalc(param_->specialAtk_);
//					}
//				}
//				else
//				{
//					hit = false;
//				}
//			}
//		}
//	}
//}

//
//	@brief	ƒ_ƒ[ƒWŒvŽZ
void ShieldMan::DamageCalc(unsigned int atk)
{
	Sound::getInstance().SE_play("Sh_DAMAGE");

	float damage = 0;
	float constant = 1;
	if (atkNo_ != specialAtk)
	{
		damage = atk / (1 + ((float)param_->def_ / 100));
	}
	else
	{
		damage = constant;
	}

	hp_ -= damage;
	if (hp_ <= 0 || param_->hp_ < hp_)
	{
		hp_ = 0;
		aliveFlg_ = false;
	}

}


//
//	@brief			ˆÚ“®ˆ—
//	@param (speed)	ˆÚ“®‘¬“x
void ShieldMan::Move(float speed)
{
	//ƒXƒeƒBƒbƒN‚ÌŒX‚«Žæ“¾
	D3DXVECTOR3 inputStick;
	inputStick.x = GamePad::getAnalogValue(charaType_, GamePad::AnalogName::AnalogName_LeftStick_X);
	inputStick.z = GamePad::getAnalogValue(charaType_, GamePad::AnalogName::AnalogName_LeftStick_Y);

	//‰ñ“]ˆ—
	const float rotEpsilon = 0.3;
	if (fabsf(inputStick.x) > rotEpsilon || fabsf(inputStick.z) > rotEpsilon)
	{
		Rotation(inputStick);
	}


	//ˆÚ“®
	const float moveEpsilon = 0.2;	//Œëì–hŽ~—p
	float sp = 0;
	if (fabsf(inputStick.x) > moveEpsilon || fabsf(inputStick.z) > moveEpsilon)
	{
		sp = speed;

		if (motionChange_ == true)
		{
			if (atkNo_ != specialAtk && motionNo_ != motion_->GetMotion("walk")->id_)
			{
				ChangeMotion(motion_, "walk");
			}
			else if (atkNo_ == specialAtk && motionNo_ != motion_->GetMotion("specialWalk")->id_)
			{
				Sound::getInstance().SE_play("Sh_SPECIAL");
				ChangeMotion(motion_, "specialWalk");
			}
		}
	}
	else
	{

		if (motionChange_ == true)
		{
			if (atkNo_ != specialAtk && motionNo_ != motion_->GetMotion("wait")->id_)
			{
				ChangeMotion(motion_, "wait");
			}
		}
	}

	//opponentWeight_ = 1;

	MoveCharaHit();

	m_Dir = D3DXVECTOR3(inputStick.x*sp * opponentWeight_, 0, inputStick.z*sp * opponentWeight_);
	//m_vPos += D3DXVECTOR3(inputStick.x*sp - opponentWeight_, 0, inputStick.z*sp - opponentWeight_);

	GamePad::update();

	//m_Dir = D3DXVECTOR3(m_AxisX.x, m_AxisY.y, m_AxisZ.z);
	//m_Dir = D3DXVECTOR3(m_Move.x, 0, m_Move.z);

}


//
//	@brief	Ž€–S‰¹Ä¶
void ShieldMan::DeadSound()
{
	Sound::getInstance().SE_play("Sh_DEAD");
}


//
//	@breif	‚Žm—pˆÚ“®ˆ—
void ShieldMan::Move_Update()
{
	float backSpeed = param_->knockbackSpeed_;

	if (aliveFlg_ == true)
	{
		if (knockBackFlg_ == false && atkNo_ == (!normalAtk || !specialAtk))
		{
			m_Pos += m_Dir;
		}
		else if (knockBackFlg_ == true && atkNo_ != specialAtk)
		{
			KnockBack(knockBackPos_, knockBackDis_, backSpeed);
		}
		else if (spMoveFlg_ == true && atkNo_ == specialAtk)
		{
			float sp = param_->specialMoveSpeed_;
			m_Pos += D3DXVECTOR3(m_Dir.x*sp, 0, m_Dir.z*sp);
		}
	}
}