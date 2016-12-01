#include "./WeaponBall.h"


//
//	@brief	飛び道具コンストラクタ
WeaponBall::WeaponBall()
	:delCount_(0)
{
	weaponBall_ = new CD3DXMESH;

	weaponBall_->m_fScale = 0.5;
	delFlg_ = false;
	col_ = new Collision;
}

//
//	@brief	飛び道具デストラクタ
WeaponBall::~WeaponBall()
{
	delete weaponBall_;
	weaponBall_ = nullptr;
	delete col_;
	col_ = nullptr;
}

//
//	@brief	読み込み
void WeaponBall::Init(char* name)
{
	char FileName[80] = "./Model/XFiles/Player/";
	strcat(FileName, name);
	if (FAILED(weaponBall_->LoadXMesh(FileName)))
	{
		return;
	}
}

//
//	@brief			方向セット
//	@param (dir)	方向ベクトル
void WeaponBall::SetDir(D3DXVECTOR3 dir)
{
	dir_ = dir;
}

//
//	@brief			サイズセット
//	@param (scale) 変更するサイズ
void WeaponBall::SetScale(float scale)
{
	weaponBall_->m_fScale = scale;
}

//
// @brief	初期位置
void WeaponBall::SetStartPos(D3DXVECTOR3 pos)
{
	startPos_ = pos;
	weaponBall_->m_vPos = pos;
	spaceNo_ = col_->SetSpaceNo(startPos_);
}
//
//	@brief	攻撃力セット
void WeaponBall::SetAttack(unsigned int atk)
{
	atk_ = atk;
}

//
//	@brief			飛び道具の移動更新
//	@param (dist)	移動終了距離
void WeaponBall::Move_Weapon(float dist,float speed)
{
	if (col_->CharaNear(startPos_, weaponBall_->m_vPos, dist))
	{
		weaponBall_->m_vPos += D3DXVECTOR3(dir_.x*speed, 0, dir_.z*speed);
	}
	else
	{
		delFlg_ = true;
	}

	spaceNo_ = col_->SetSpaceNo(weaponBall_->m_vPos);
	
	Hit();
}

//
//	@brief	一定時間後に消える
void WeaponBall::Time_Del_Weapon(int frame)
{
	if (++delCount_ >= frame)
	{
		Hit();
		delFlg_ = true;
		delCount_ = 0;
	}
}

//
//	@brief			被弾する可能性のあるキャラクターリスト
//	@param (chara)	ダメージを食らうキャラ
void WeaponBall::SetDamageList(std::vector<CharactorManager*> chara, CharaType cType)
{
	for (auto c : chara)
	{
		if (c->GetCharaType() != cType)
		{
			if (col_->CheckSpaceNo(spaceNo_,c->GetSpaceNo()))
			{
				damageList_.push_back(c);
			}
		}
	}
}

//
//	@brief	被弾するキャラ更新
//	@param (chara)	被弾するキャラ
void WeaponBall::SetDamageChara(CharactorManager* chara)
{
	damageList_.push_back(chara);
}

//
//	@brief 弾が消えるかどうか
bool WeaponBall::GetDelFlg()const
{
	return delFlg_;
}

//
//	@brief	座標取得
D3DXVECTOR3 WeaponBall::GetPosition()const
{
	return weaponBall_->m_vPos;
}

//
//	@brief	攻撃ヒット
void WeaponBall::Hit()
{
	if (!damageList_.empty())
	{
		for (auto c : damageList_)
		{
			if (col_->CharaNear(weaponBall_->m_vPos, c->m_Pos, dist_))
			{
				c->SetKnockBack(weaponBall_->m_vPos, kDist_,kSpeed_);
				delFlg_ = true;

				if (c->GetCharaType() == Enemy)
				{
					c->DamageCalc(atk_);
					//敵にダメージが入った時のSE
					Sound::getInstance().SE_play(hitSoundName_);
				}
			}
		}
	}
}

//
//	@brief			ノックバック設定
//	@param (dist)	攻撃の範囲
//	@param (kDist)	ノックバックする距離
//	@param (kSpeed)	ノックバックスピード
void WeaponBall::SetKnockBack(float dist, float kDist,float kSpeed)
{
	dist_ = dist;
	kDist_ = kDist;
	kSpeed_ = kSpeed;
}

//
//	@brief	ヒットサウンド設定
void WeaponBall::SetHitSound(std::string name)
{
	hitSoundName_ = name;
}

////
////	@brief	攻撃の当たる範囲取得
//float WeaponBall::GetHitDist()const
//{
//	return dist_;
//}
//
////
////	@brief	ノックバックする距離取得
//float WeaponBall::GetKnockBackDist()const
//{
//	return kDist_;
//}

//
//	@brief	描画
void WeaponBall::Render(D3DXVECTOR3 pos)
{
	weaponBall_->Render(pos,D3DXVECTOR3(0,0,0),weaponBall_->m_fScale);
}