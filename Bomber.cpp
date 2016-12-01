#include "./Bomber.h"

Bomber::Bomber(CharaType charaType) :JobManager(charaType)
{
	charaType_ = charaType;
	bombFlg_ = false;
	bombList_.clear();
	bombScale_ = 1;
	bombCount_ = 1;
	invisibleCount_ = 0;
	invinsibleFlg_ = false;
	bomb_ = new WeaponBall;
}

Bomber::~Bomber()
{
	delete bomb_;
	bomb_ = nullptr;
}


//
//	@brief						Xファイル読み込み
//	@param (m_hWnd)				ウィンドウハンドル
//	@param (m_pDevice)			デバイス
//	@param (m_pDeviceContext)	デバイスコンテキスト
//	@param (fileName)			読み込むキャラ名
const char* Bomber::CharaInit(const char* fileName)
{
	bomb_->Init("bomb.x");

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
void Bomber::Reset()
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
	bombFlg_ = false;
	invinsibleFlg_ = false;
	bombList_.clear();

	m_Pos = D3DXVECTOR3(-2.25 + charaType_*1.5, 0, -10);
}

//
//	@brief	移動方向にキャラクターがいるか
//	@note	移動方向にキャラクターがいたら、そのキャラクターの重さを取得
void Bomber::MoveCharaHit()
{
	float dist = 1;
	float degree = D3DXToDegree(m_Yaw);
	CharactorManager* opp = nullptr;
	for (auto c : aroundCharaList_)
	{
		if (collision_->CharaNear(m_Pos, c->m_Pos, dist))
		{
			D3DXVECTOR3 vec = c->m_Pos - m_Pos;
			float angle = (atan2(vec.z, vec.x)*-1) - (D3DX_PI / 2.0f);
			angle = D3DXToDegree(angle);

			float hitAngle = 90 / 2;
			if (fabsf(degree - angle) <= hitAngle)
			{
				/*opponentWeight_ = c->ownWright_;*/
				opponentWeight_ = 0;
				opp = c;
			}
		}
	}
	if (opp == nullptr || invinsibleFlg_ == true)
	{
		opponentWeight_ = 1;
	}
}

//
//	@brief	攻撃
void Bomber::Attack()
{
	if (GamePad::checkInput(charaType_, GamePad::InputName::A)
		/*|| GetKeyState('1') & 0x80*/)
	{
		++attackCount_;
	}
	else if (atkNo_ == normalAtk)
	{
		attackCount_ = 0;
		Normal_Attack();
	}
	else if (atkNo_ == charge)
	{
		moveAbleFlg_ = true;
		attackCount_ = 0;
		atkNo_ = specialAtk;
		invinsibleFlg_ = true;
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
		moveAbleFlg_ = false;
		if (/*motionChange_ == true && */motionNo_ != motion_->GetMotion("charge")->id_)
		{
			motionChange_ = false;
			//motionNo_ = motion_->GetMotion("charge")->id_;
			//m_pD3dxMesh->ChangeAnimSet(motionNo_);
			//timeEnd_ = motion_->GetMotion("attack")->frame_;
			//motionSpeed_ = 1 / (float)timeEnd_;
			//motionCount_ = 0;
			ChangeMotion(motion_, "charge");
		}
	}

	if (atkNo_ == specialAtk)
	{
		Special_Attack();
	}

	if (invinsibleFlg_)
	{
		int invincibleTime = param_->specialAttackTime_;
		if (++invisibleCount_ % (FPS * invincibleTime) == 0)
		{
			invinsibleFlg_ = false;
			invisibleCount_ = 0;
		}
	}

	if (!bombList_.empty())
	{
		bombFlg_ = true;
		//static int count = 0;
		//int count = 0;
		float delTime = FPS*param_->weaponDelTime_;
		for (auto b : bombList_)
		{
			b->Time_Del_Weapon(delTime);
		}
		if (/*b != nullptr &&*/ bombList_[0]->GetDelFlg())
		{
			bombList_.erase(bombList_.begin());
			Sound::getInstance().SE_play("B_NORMALATK");
			//delete b;
			//b = nullptr;
			//++count;
			//--count;
		}
		//++count;
		if (bombList_.empty())
		{
			bombList_.clear();
			bombFlg_ = false;
			//count = 0;
		}

		/*if (count == bombCount_)
		{
		bomb_.clear();
		bombFlg_ = false;
		count = 0;
		}*/
	}
}

//
//	@brief	通常攻撃
void Bomber::Normal_Attack()
{
	size_t size = param_->chainWeapon_;
	float range = param_->weaponHitReach_;
	float kDist = param_->knockbackDist_;
	float kSpeed = param_->knockbackSpeed_;
	moveAbleFlg_ = false;
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
		Sound::getInstance().SE_play("B_SPECIAL");
		motionChange_ = true;
		motionChange_ = true;
		if (bombList_.empty() || bombList_.size() < size)
		{
			WeaponBall* bomb = new WeaponBall;
			bomb->SetStartPos(m_Pos);
			bomb->SetScale(0.2);
			bomb->SetAttack(param_->normalAtk_);
			bomb->SetDamageList(allCharaList_, charaType_);
			bomb->SetKnockBack(range, kDist, kSpeed);
			bomb->SetHitSound("B_DAMAGE_HIT");
			bombList_.push_back(bomb);
		}
		atkNo_ = noAtk;
		moveAbleFlg_ = true;
	}



	//atkNo_ = noAtk;
}

//
//	@brief	特殊攻撃
void Bomber::Special_Attack()
{
	if (/*motionChange_ == true && */motionNo_ != motion_->GetMotion("special")->id_)
	{
		motionChange_ = false;
		/*motionNo_ = motion_->GetMotion("special")->id_;
		m_pD3dxMesh->ChangeAnimSet(motionNo_);
		timeEnd_ = motion_->GetMotion("special")->frame_;
		motionSpeed_ = 1 / (float)timeEnd_;
		motionCount_ = 0;*/
		ChangeMotion(motion_, "special");
	}

	if (++motionCount_%motionFrame_ == 0)
	{
		atkNo_ = noAtk;
		motionChange_ = true;
	}

	/*int invincibleTime = param_->specialAttackTime_;
	if (++motionCount_ % (FPS * invincibleTime) == 0)
	{
		invincibleFlg_ = false;
		motionCount_ = 0;
	}
	atkNo_ = noAtk;*/
}

//
//	@brief	爆弾士用移動処理
void Bomber::Move_Update()
{
	float kSpeed = param_->knockbackSpeed_;
	if (aliveFlg_ == true)
	{
		if (knockBackFlg_ == false && moveAbleFlg_==true)
		{
			m_Pos += m_Dir;
		}
		else if (knockBackFlg_ == true && invinsibleFlg_ == false)
		{
			KnockBack(knockBackPos_, knockBackDis_, kSpeed);
		}
	}
}


//
//	@brief			ダメージ計算
//	@param (atk)	攻撃者の攻撃力
void Bomber::DamageCalc(unsigned int atk)
{
	Sound::getInstance().SE_play("B_DAMAGE");
	float damage = 0;
	if (invinsibleFlg_ == false)
	{
		damage = atk / (1 + ((float)param_->def_ / 100));
	}

	hp_ -= damage;
	if (hp_ <= 0 || param_->hp_ < hp_)
	{
		hp_ = 0;
		aliveFlg_ = false;
	}

}


//
//	@brief	死亡音再生
void Bomber::DeadSound()
{
	Sound::getInstance().SE_play("B_DEAD");
}

//
//	@brief	描画
void Bomber::CharaRender()
{
	Render(m_Pos);


	if (!bombList_.empty())
	{
		for (auto b : bombList_)
		{
			if (b != nullptr)
			{
				//b->Render();
				bomb_->Render(b->GetPosition());
			}
		}
	}
}