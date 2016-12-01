//
//	@file	CharactorManager.h
//	@brief	�L�����N�^�[�Ǘ��N���X
//	@date	2016/11/09
//	@outher	�m�ȍ��c

#pragma once
#include <vector>
#include "./../Origin.h"
#include "./../Mesh/CD3DXSKINMESH.h"
#include "./../Collision.h"
#include "./../Read/MotionRead.h"
#include "./../SoundEffect/Sound.h"

//#include "./../DebugDraw/D3D11_TEXT.h"
enum CharaType
{
	Player1=0,
	Player2,
	Player3,
	Player4,
	PrincessT,
	Enemy,
};


class CharactorManager:public CD3DXSKINMESH
{
protected:
	Motion* motion_;

	CharaType charaType_;
	Collision* collision_;
	unsigned int spaceNo_;		//���W�ɂ���Ԕԍ�
	unsigned int hp_;			//���݂�HP
	unsigned int motionNo_;		//���݂̃��[�V�����ԍ�
	unsigned int motionCount_;	//���[�V�����p�J�E���g
	unsigned int motionFrame_;	//���݂̃��[�V�����̑��t���[����
	bool motionChange_;			//���[�V�����ύX�t���O
	float motionSpeed_;			//���[�V�����X�s�[�h
	float opponentWeight_;		//�i�s�����ɂ���L�����̏d��
	float ownWright_;			//���g�̏d��
	bool aliveFlg_;				//�����t���O
	bool moveAbleFlg_;			//�ړ��\�t���O
	D3DXVECTOR3 ownPush_;		//�����������Ă�������x�N�g��

	float knockBackDis_;
	float knockBackSpeed_;
	bool knockBackFlg_;
	D3DXVECTOR3 knockBackPos_;

	std::vector<CharactorManager*> aroundCharaList_;	//���ӂɂ���L����
	std::vector<CharactorManager*> allCharaList_;		//�S�ẴL����
	virtual void Move(float speed) = 0;					//�ړ�
	virtual void Attack() = 0;							//�U��
	virtual void Dead() = 0;							//���S
	virtual void MoveCharaHit();
	void Rotation(D3DXVECTOR3 dirVec);					//�L�����̉�]
	void AroundCharaCheck();
	void ChangeMotion(Motion* motion,char* name);
	virtual void DamageSound() {};		//�_���[�W�T�E���h

public:
	CharactorManager();
	virtual ~CharactorManager();

	virtual const char* CharaInit(const char* fileName)=0;		//�������E�ǂݍ���
	virtual void CharaUpdate()=0;								//�X�V
	virtual void Reset()=0;
	virtual void CharaRender();									//�`��
	virtual void Move_Update();									//�ړ��̍X�V
	virtual void DamageCalc(unsigned int atk) {};				//�_���[�W�v�Z
	void SetMotionData(Motion* motionData);
	void KnockBack(D3DXVECTOR3 atkPos, float distance,float speed);	//�m�b�N�o�b�N

	void SlipMove(D3DXVECTOR3 slopVec);							//�Ǌ���p�ړ�
	void StopMove();											//�Î~�p
	void SetAroundChara(CharactorManager* charactor);			//���ӂɂ���L�����N�^�[�����X�g�ɃZ�b�g
	void SetAllCharaList(std::vector<CharactorManager*> list);
	void SetOppWeight(float weight);							//�i�s�����ɂ���L�����N�^�[�̏d���Z�b�g
	void SetKnockBack(D3DXVECTOR3 pos, float dist,float speed);
	int GetSpaceNo();
	bool GetAliveFlg() { return aliveFlg_; };
	CharaType GetCharaType()const;

	int GetAroundC() { return aroundCharaList_.size(); };
	unsigned int GetHP()const { return hp_; };
	unsigned int GetMotionNo()const { return motionNo_; };
	float GetMotionSpeed()const { return motionSpeed_; };
	unsigned int GetMotionCount()const { return motionCount_; };
};
