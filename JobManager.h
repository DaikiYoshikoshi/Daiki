//
//	@file	JobManager.h
//	@brief	職業毎の処理
//	@date	2016/11/10
//	@outher	仁科香苗

#pragma once
#include "./../Player/PlayerManager.h"
#include "./../../Read/ParameterRead.h"
#include "./../../Collision.h"
#include "./../../Mesh/CD3DXMESH.h"
#include "./Job/WeaponBall.h"
#include "./../../Read/ParameterRead.h"


//	職業管理クラス
class JobManager :public PlayerManager
{
protected:
	//unsigned int timeCount_;	//攻撃時間
	//unsigned int timeAtkEnd_;		//攻撃終了時間
	int attackCount_;			//攻撃ボタンカウント
	

	Collision* col_;

public:
	JobManager(CharaType charaType);
	virtual ~JobManager();

	virtual void CharaUpdate();
	//template<class> p;
	void SetParameter(JobParameter* param);
	//PlayerParameter GetParam()const;
	bool hit;//デバッグ用

	int GetAtkCnt() { return attackCount_; };

	//デバック用
	PlayerParameter* GetParam()const { return param_; };
};

