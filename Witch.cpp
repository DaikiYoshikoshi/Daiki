#include "./Witch.h"


Witch::Witch(CharaType charaType) :JobManager(charaType)
{
	charaType_ = charaType;
	magicFlg_ = false;
	atkNo_ = noAtk;
	attackCount_ = 0;
	magic_ = new WeaponBall;
}

Witch::~Witch()
{
	delete magic_;
	magic_ = nullptr;
}

//
//	@brief						Xファイル読み込み
//	@param (m_hWnd)				ウィンドウハンドル
//	@param (m_pDevice)			デバイス
//	@param (m_pDeviceContext)	デバイスコンテキスト
//	@param (fileName)			読み込むキャラ名
const char* Witch::CharaInit(const char* fileName)
{
	magic_->Init("magicball.x");

	char FileName[80] = { 0 };
	memset(FileName, 0, sizeof(FileName));
	strcpy_s(FileName, sizeof(FileName), "./Model/XFiles/Player/");
	strcat_s(FileName, sizeof(FileName), fileName);
	return FileName;
	//CreateFromX(FileName);
	//m_Scale = D3DXVECTOR3(0.2, 0.2, 0.2);
	//ownWright_ = 0.001f;
}

//
//	@brief	リセット
void Witch::Reset()
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
	magicFlg_ = false;
	magicBall_.clear();

	m_Pos = D3DXVECTOR3(-2.25 + charaType_*1.5, 0, -10);
}


//
//	@brief	攻撃
void Witch::Attack()
{
	if (atkNo_ == noAtk)
	{
		moveAbleFlg_ = true;
	}
	else
	{
		moveAbleFlg_ = false;
	}

	//atkNo_ = noAtk;
	if (magicFlg_ == false && GamePad::checkInput(charaType_, GamePad::InputName::A)
		/*|| GetKeyState('1') & 0x80*/)
	{
		++attackCount_;
		atkNo_ = waitAtk;
	}
	else if (atkNo_ == normalAtk)
	{
		attackCount_ = 0;
		Normal_Attack();
	}
	else if (atkNo_ == charge)
	{
		attackCount_ = 0;
		atkNo_ = specialAtk;

		//hit = false;
	}
	//unsigned int inputTime = playerParam_.chargeTime_;

	unsigned int inputTime = FPS*param_->chargeTime_;

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
		}
	}

	if (atkNo_ == specialAtk)
	{
		Special_Attack();
	}

	if (magicFlg_ == true && !magicBall_.empty())
	{
		int count = 0;
		float kDist = param_->weaponDelDist_;
		float speed = 1;
		for (size_t i = 0; i < magicBall_.size(); i++)
		{
			magicBall_[i]->Move_Weapon(kDist,speed);
			if (magicBall_[i]->GetDelFlg())
			{
				magicBall_.erase(magicBall_.begin() + count);
				/*atkNo_ = noAtk;*/
				--count;
			}
			++count;
		}
		if (magicBall_.empty())
		{
			magicBall_.clear();
			magicFlg_ = false;
		}
	}
}

//
//	@brief	通常攻撃
void Witch::Normal_Attack()
{
	//float dist = param_->attackReach_;
	float kRange = param_->weaponHitReach_;
	float kDist = param_->knockbackDist_;
	float kSpeed = param_->knockbackSpeed_;
	magicBallCount_ = param_->chainWeapon_;
	if (/*motionChange_ == true && */motionNo_ != motion_->GetMotion("attack")->id_)
	{
		motionChange_ = false;
		//motionNo_ = motion_->GetMotion("attack")->id_;
		//m_pD3dxMesh->ChangeAnimSet(motionNo_);
		//timeEnd_ = motion_->GetMotion("attack")->frame_;
		//motionSpeed_ = 1 / (float)timeEnd_;
		//motionCount_ = 0;
		ChangeMotion(motion_, "attack");
	}

	if (++motionCount_%motionFrame_ == 0)
	{
		Sound::getInstance().SE_play("M_NORMALATK");
		atkNo_ = noAtk;
		motionChange_ = true;
		if (!magicFlg_)
		{
			WeaponBall* magic = new WeaponBall();

			D3DXVECTOR3 vec(sinf(m_Yaw)*-0.1, 0, cosf(m_Yaw)*-0.1);
			magic->SetDir(vec);
			magic->SetScale(0.001);
			magic->SetStartPos(D3DXVECTOR3(m_Pos.x, m_Pos.y + 1, m_Pos.z));
			magic->SetDamageList(allCharaList_, charaType_);
			magic->SetKnockBack(kRange, kDist, kSpeed);
			magic->SetAttack(param_->normalAtk_);
			magic->SetHitSound("M_DAMAGE_HIT");
			magicBall_.push_back(magic);
			magicFlg_ = true;
		}
	}

	
}

//
//	@brief	特殊攻撃
void Witch::Special_Attack()
{
	float kRange = param_->weaponHitReach_;
	float kDist = param_->knockbackDist_;
	float kSpeed = param_->knockbackSpeed_;
	magicBallCount_ = param_->spChainWeapon_;
	if (/*motionChange_ == true && */motionNo_ != motion_->GetMotion("attack")->id_)
	{
		motionChange_ = false;
		//motionNo_ = motion_->GetMotion("attack")->id_;
		//m_pD3dxMesh->ChangeAnimSet(motionNo_);
		//timeEnd_ = motion_->GetMotion("attack")->frame_;
		//motionSpeed_ = 1 / (float)timeEnd_;
		//motionCount_ = 0;
		ChangeMotion(motion_, "attack");
	}

	if (++motionCount_%motionFrame_ == 0)
	{
		Sound::getInstance().SE_play("M_SPECIAL");
		atkNo_ = noAtk;
		motionChange_ = true;
		if (!magicFlg_)
		{
			float angle = D3DXToDegree(m_Yaw);
			for (int i = 0; i < magicBallCount_; i++)
			{
				WeaponBall* magic = new WeaponBall;
				int degree = 90 / (magicBallCount_ / 2 + 1);
				float temp = angle - 90 + degree + degree*i;
				temp = D3DXToRadian(temp);
				D3DXVECTOR3 vec(sinf(temp)*-0.1, 0, cosf(temp)*-0.1);
				magic->SetDir(vec);
				magic->SetScale(0.5);
				magic->SetStartPos(D3DXVECTOR3(m_Pos.x, m_Pos.y + 1, m_Pos.z));
				magic->SetDamageList(allCharaList_, charaType_);
				magic->SetKnockBack(kRange, kDist, kSpeed);
				magic->SetAttack(param_->specialAtk_);
				magic->SetHitSound("M_DAMAGE_HIT");
				magicBall_.push_back(magic);
			}
			magicFlg_ = true;
		}
	}

	
}

void Witch::DamageSound()
{
	Sound::getInstance().SE_play("M_DAMAGE");
}

//
//	@brief	死亡音再生
void Witch::DeadSound()
{
	Sound::getInstance().SE_play("M_DEAD");
}

//
//	@brief	描画
void Witch::CharaRender()
{
	Render(m_Pos);
	if (!magicBall_.empty())
	{
		for (auto m:magicBall_)
		{
			magic_->Render(m->GetPosition());
		}
	}
}