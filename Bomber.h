#pragma once
#include "./../../../Origin.h"
#include "./../JobManager.h"

//	”š’emƒNƒ‰ƒX
class Bomber :public JobManager
{
private:
	WeaponBall* bomb_;

	bool bombFlg_;	//”š’e‚ª”­“®’†‚©‚Ç‚¤‚©
	bool invinsibleFlg_;	//–³“G’†‚©‚Ç‚¤‚©
	int invisibleCount_;
	int bombCount_;
	float bombScale_;
	//WeaponBall** bomb_;
	std::vector<WeaponBall*> bombList_;
	virtual void Normal_Attack();
	virtual void Special_Attack();
	virtual void MoveCharaHit();
	virtual void DeadSound();
	virtual void DamegeSound() {};
public:
	Bomber(CharaType charaType);
	virtual ~Bomber();
	virtual const char* CharaInit(const char* fileName);
	virtual void Reset();
	virtual void Attack();
	virtual void Move_Update();
	virtual void DamageCalc(unsigned int atk);					//ƒ_ƒ[ƒWŒvZ
	virtual void CharaRender();
};