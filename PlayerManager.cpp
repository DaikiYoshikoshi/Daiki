//
//	@file	PlayerManager.cpp
//	@brief	プレイヤー管理クラス
//	@date	2016/11/09
//	@outher	仁科香苗

#include "PlayerManager.h"

//
//	@brief	コンストラクタ
PlayerManager::PlayerManager()
{
	revivalFlg_ = false;
	callTiming_ = 0;
}

//
//	@brief						Xファイル読み込み
//	@param (m_hWnd)				ウィンドウハンドル
//	@param (m_pDevice)			デバイス
//	@param (m_pDeviceContext)	デバイスコンテキスト
//	@param (fileName)			読み込むキャラ名
const char* PlayerManager::CharaInit(const char* fileName)
{
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
//	@brief			移動処理
//	@param (speed)	移動速度
void PlayerManager::Move(float speed)
{
		//スティックの傾き取得
		D3DXVECTOR3 inputStick;
		inputStick.x = GamePad::getAnalogValue(charaType_, GamePad::AnalogName::AnalogName_LeftStick_X);
		inputStick.z = GamePad::getAnalogValue(charaType_, GamePad::AnalogName::AnalogName_LeftStick_Y);

		//回転処理
		const float rotEpsilon = 0.3;
		if (fabsf(inputStick.x) > rotEpsilon || fabsf(inputStick.z) > rotEpsilon)
		{
			Rotation(inputStick);
		}


		//移動
		const float moveEpsilon = 0.2;	//誤作防止用
		float sp = 0;
		if (fabsf(inputStick.x) > moveEpsilon || fabsf(inputStick.z) > moveEpsilon)
		{
			sp = speed;

			if (motionChange_==true && motionNo_ != motion_->GetMotion("walk")->id_)
			{
				//motionNo_ = motion_->GetMotion("walk")->id_;
				//m_pD3dxMesh->ChangeAnimSet(motion_->GetMotion("walk")->id_);
				//モーション速度
				//motionSpeed_ = 1 / (float)motion_->GetMotion("walk")->frame_;
				ChangeMotion(motion_, "walk");
			}
		}
		else
		{
			if (motionChange_ == true && motionNo_ != motion_->GetMotion("wait")->id_)
			{
				//motionNo_ = motion_->GetMotion("wait")->id_;
				//m_pD3dxMesh->ChangeAnimSet(motionNo_);
				//motionSpeed_ = 1 / (float)motion_->GetMotion("walk")->frame_;
				ChangeMotion(motion_, "wait");
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
//	@brief	ダメージ計算
void PlayerManager::DamageCalc(unsigned int atk)
{
	DamageSound();
	float damage = atk / (1 + ((float)param_->def_ / 100));
	hp_ -= damage;

	if (hp_ <= 0 || param_->hp_<hp_)
	{
		hp_ = 0;
		aliveFlg_ = false;
	}

}

//
//	@brief	死亡処理
void PlayerManager::Dead()
{
	//aliveFlg_ = false;
	if (motionChange_ == true && motionNo_ != motion_->GetMotion("dead")->id_)
	{
		moveAbleFlg_ = false;
		//motionChange_ = false;
		//motionNo_ = motion_->GetMotion("dead")->id_;
		//m_pD3dxMesh->ChangeAnimSet(motionNo_);
		//motionSpeed_ = 1 / (float)motion_->GetMotion("dead")->frame_;
		DeadSound();
		ChangeMotion(motion_, "dead");
	}

	if (motionNo_ == motion_->GetMotion("dead")->id_)
	{
		if (++motionCount_%motionFrame_ == 0)
		{
			//motionChange_ = true;
			motionCount_ = 0;
		}
	}
}


//
//	@brief	復活
void PlayerManager::Revival()
{
	if (motionChange_ == true && motionNo_ != motion_->GetMotion("alive")->id_)
	{
		motionChange_ = false;
		//motionNo_ = motion_->GetMotion("alive")->id_;
		//m_pD3dxMesh->ChangeAnimSet(motionNo_);
		//motionSpeed_ = 1 / (float)motion_->GetMotion("alive")->frame_;
		ChangeMotion(motion_, "alive");
	}

	if (motionNo_ == motion_->GetMotion("alive")->id_)
	{
		if (++motionCount_%motionFrame_ == 0)
		{
			moveAbleFlg_ = true;
			motionChange_ = true;
			motionCount_ = 0;
			hp_ = param_->hp_;
			revivalFlg_ = false;
			aliveFlg_ = true;
		}
	}
}

//
//	@brief	姫呼び
void PlayerManager::Princess_Call()
{
	if (GamePad::checkInput(charaType_, GamePad::InputName::B) && atkNo_==noAtk)
	{
		moveAbleFlg_ = false;
		callPos_ = m_Pos;
		callTiming_ = clock();
		if (motionChange_ == true && motionNo_ != motion_->GetMotion("call")->id_)
		{
			motionChange_ = false;
			//motionNo_ = motion_->GetMotion("call")->id_;
			//m_pD3dxMesh->ChangeAnimSet(motionNo_);
			//motionSpeed_ =  1/(float)motion_->GetMotion("call")->frame_;
			ChangeMotion(motion_, "call");
			
		}
	}

	if (motionNo_ == motion_->GetMotion("call")->id_)
	{
		if (++motionCount_%motionFrame_ == 0)
		{
			//revivalFlg_ = false;
			moveAbleFlg_ = true;
			motionChange_ = true;
			motionCount_ = 0;
			//ChangeMotion(motion_, "wait");
		}
	}

}

//
//	@breif	復活フラグセット
void PlayerManager::SetRevivalFlg()
{
	revivalFlg_ = true;
}

//
//	@brief	生存フラグ取得
bool PlayerManager::GetAliveFlg()const
{
	return aliveFlg_;
}

//
//	@brief	姫を呼んだクロック時間取得
double PlayerManager::GetCallTiming()const
{
	return callTiming_;
}

//
//	@brief	姫を呼んだ瞬間の座標取得
D3DXVECTOR3 PlayerManager::GetCallPos()const
{
	return callPos_;
}