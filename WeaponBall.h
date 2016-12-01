#pragma once
#include "./../../../Origin.h"
#include "./../../CharactorManager.h"

//
//	@brief	飛び道具(魔導士と爆弾士)
class WeaponBall
{
private:
	std::string hitSoundName_;
	unsigned int spaceNo_;
	Collision* col_;
	CD3DXMESH* weaponBall_;
	std::vector<CharactorManager*> damageList_;	//被弾する可能性のあるキャラクターリスト
	bool delFlg_;
	int delCount_;
	unsigned int atk_;
	float dist_;
	float kDist_;
	float kSpeed_;
	D3DXVECTOR3 startPos_;
	D3DXVECTOR3 dir_;
	void Hit();
public:
	WeaponBall();
	~WeaponBall();
	void Init(char* name);
	void SetDir(D3DXVECTOR3 dir);
	void SetScale(float scale);
	void SetStartPos(D3DXVECTOR3 pos);
	void Move_Weapon(float dist,float speed);
	void Time_Del_Weapon(int frame);
	void SetDamageList(std::vector<CharactorManager*> chara,CharaType cType);
	void SetDamageChara(CharactorManager* chara);
	void SetAttack(unsigned int atk);
	void SetKnockBack(float dist, float kDist,float kSpeed);
	void SetHitSound(std::string name);
	void Render(D3DXVECTOR3 pos);
	bool GetDelFlg()const;
	
	//float GetHitDist()const;
	//float GetKnockBackDist()const;
	D3DXVECTOR3 GetPosition()const;
};