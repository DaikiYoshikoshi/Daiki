#include "./WeaponBall.h"


//
//	@brief	��ѓ���R���X�g���N�^
WeaponBall::WeaponBall()
	:delCount_(0)
{
	weaponBall_ = new CD3DXMESH;

	weaponBall_->m_fScale = 0.5;
	delFlg_ = false;
	col_ = new Collision;
}

//
//	@brief	��ѓ���f�X�g���N�^
WeaponBall::~WeaponBall()
{
	delete weaponBall_;
	weaponBall_ = nullptr;
	delete col_;
	col_ = nullptr;
}

//
//	@brief	�ǂݍ���
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
//	@brief			�����Z�b�g
//	@param (dir)	�����x�N�g��
void WeaponBall::SetDir(D3DXVECTOR3 dir)
{
	dir_ = dir;
}

//
//	@brief			�T�C�Y�Z�b�g
//	@param (scale) �ύX����T�C�Y
void WeaponBall::SetScale(float scale)
{
	weaponBall_->m_fScale = scale;
}

//
// @brief	�����ʒu
void WeaponBall::SetStartPos(D3DXVECTOR3 pos)
{
	startPos_ = pos;
	weaponBall_->m_vPos = pos;
	spaceNo_ = col_->SetSpaceNo(startPos_);
}
//
//	@brief	�U���̓Z�b�g
void WeaponBall::SetAttack(unsigned int atk)
{
	atk_ = atk;
}

//
//	@brief			��ѓ���̈ړ��X�V
//	@param (dist)	�ړ��I������
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
//	@brief	��莞�Ԍ�ɏ�����
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
//	@brief			��e����\���̂���L�����N�^�[���X�g
//	@param (chara)	�_���[�W��H�炤�L����
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
//	@brief	��e����L�����X�V
//	@param (chara)	��e����L����
void WeaponBall::SetDamageChara(CharactorManager* chara)
{
	damageList_.push_back(chara);
}

//
//	@brief �e�������邩�ǂ���
bool WeaponBall::GetDelFlg()const
{
	return delFlg_;
}

//
//	@brief	���W�擾
D3DXVECTOR3 WeaponBall::GetPosition()const
{
	return weaponBall_->m_vPos;
}

//
//	@brief	�U���q�b�g
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
					//�G�Ƀ_���[�W������������SE
					Sound::getInstance().SE_play(hitSoundName_);
				}
			}
		}
	}
}

//
//	@brief			�m�b�N�o�b�N�ݒ�
//	@param (dist)	�U���͈̔�
//	@param (kDist)	�m�b�N�o�b�N���鋗��
//	@param (kSpeed)	�m�b�N�o�b�N�X�s�[�h
void WeaponBall::SetKnockBack(float dist, float kDist,float kSpeed)
{
	dist_ = dist;
	kDist_ = kDist;
	kSpeed_ = kSpeed;
}

//
//	@brief	�q�b�g�T�E���h�ݒ�
void WeaponBall::SetHitSound(std::string name)
{
	hitSoundName_ = name;
}

////
////	@brief	�U���̓�����͈͎擾
//float WeaponBall::GetHitDist()const
//{
//	return dist_;
//}
//
////
////	@brief	�m�b�N�o�b�N���鋗���擾
//float WeaponBall::GetKnockBackDist()const
//{
//	return kDist_;
//}

//
//	@brief	�`��
void WeaponBall::Render(D3DXVECTOR3 pos)
{
	weaponBall_->Render(pos,D3DXVECTOR3(0,0,0),weaponBall_->m_fScale);
}